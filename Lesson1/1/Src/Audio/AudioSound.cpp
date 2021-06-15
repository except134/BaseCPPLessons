#include "ExceptEnginePCH.h"

using namespace ee;

XAudioSound::XAudioSound() :
	pSourceVoice(nullptr),
    pOriginData(nullptr),
    isLoop(false),
    maxVolume(2.0f),
    maxPitch(2.0f)
{
}

XAudioSound::~XAudioSound()
{
    if(pSourceVoice) {
        pSourceVoice->FlushSourceBuffers();

        XAUDIO2_VOICE_STATE xState;
        pSourceVoice->GetState(&xState);
        while(xState.BuffersQueued != 0) {
            pSourceVoice->GetState(&xState);
            Sleep(5);
        }
        pSourceVoice->DestroyVoice();
    }
}

bool XAudioSound::Submit()
{
    XAUDIO2_BUFFER buffer;
    memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
    buffer.AudioBytes = (UINT32)pOriginData->GetWaveSize();
    buffer.pAudioData = pOriginData->GetWaveData();
    buffer.LoopCount = IsLoop() ? XAUDIO2_LOOP_INFINITE : 0;

    if(pSourceVoice)
        pSourceVoice->SubmitSourceBuffer(&buffer);

    return true;
}

bool XAudioSound::IsPlaying() const
{
    XAUDIO2_VOICE_STATE xState;
    pSourceVoice->GetState(&xState);

    return xState.BuffersQueued != 0;
}

void XAudioSound::SetLoopFlag(bool loopFlag)
{
    isLoop = loopFlag;
    Submit();
}

bool XAudioSound::Play()
{
    if(pSourceVoice) {
        XAUDIO2_VOICE_STATE xState;
        pSourceVoice->GetState(&xState);

        if(xState.BuffersQueued == 0) {
            pSourceVoice->FlushSourceBuffers();
            Submit();
        }

        pSourceVoice->Start(0);
    }
    return true;
}

bool XAudioSound::ExitLoop()
{
    if(pSourceVoice) {
        pSourceVoice->ExitLoop();
    } else {
        return false;
    }
    return true;
}

bool XAudioSound::Stop()
{
    if(pSourceVoice) {
        pSourceVoice->Stop(0);
        pSourceVoice->FlushSourceBuffers();
    }
    return true;
}

bool XAudioSound::RePlay()
{
    if(pSourceVoice) {
        Stop();
        Play();
    }
    return true;
}

bool XAudioSound::Pause()
{
    if(pSourceVoice) {
        pSourceVoice->Stop(0);
    }
    return true;
}

bool XAudioSound::CreateSound(XAudioWaveData* pWaveData)
{
    pOriginData = pWaveData;

	XAudio2Master& xAudioMaster = XAudio2Master::Instance();
    IXAudio2* pHandle = xAudioMaster.GetInterface();
    HRESULT hr = pHandle->CreateSourceVoice(&pSourceVoice, &pOriginData->GetWaveFmtEx());

    if(FAILED(hr))
        return false;

    IUnknown* pReverbAPO;
    XAudio2CreateReverb(&pReverbAPO);

    XAUDIO2_EFFECT_DESCRIPTOR desc;
    desc.InitialState = false;
    desc.pEffect = pReverbAPO;
    desc.OutputChannels = 1;

    XAUDIO2_EFFECT_CHAIN chain;
    chain.EffectCount = 1;
    chain.pEffectDescriptors = &desc;
    pSourceVoice->SetEffectChain( &chain );
    Submit();

    return true;
}

bool XAudioSound::SetPan(float value)
{
    if(pSourceVoice == nullptr)
        return false;

    if(value >  1.0f )
		value =  1.0f;

    if(value < -1.0f )
		value = -1.0f;

    XAUDIO2_VOICE_DETAILS details;
    if(pSourceVoice)
        pSourceVoice->GetVoiceDetails(&details);

    int sz = details.InputChannels * 2;

    float *matrix = new float[sz];
    float halfPan = value / 2.0f;

	for(int i = 0; i < sz; ++i) {
        if(i%2 == 0)
            matrix[i] = 0.5f - halfPan;
        else if(i%2 == 1)
            matrix[i] = 0.5f + halfPan;
    }

    if(pSourceVoice)
        pSourceVoice->SetOutputMatrix(nullptr, details.InputChannels, 2, matrix);

	delete[] matrix;
    
	return true;
}

float XAudioSound::GetVolume() const
{
    float voluem = 0;

	if(pSourceVoice) {
        pSourceVoice->GetVolume(&voluem);
    }

    return voluem;
}

bool XAudioSound::SetVolume(float value)
{
    if(pSourceVoice) {
        pSourceVoice->SetVolume(std::max<float>(std::min(value, maxVolume), 0));
        return true;
    }

    return false;
}

void XAudioSound::SetMaxVolume(float value)
{
    maxVolume = value;
}

bool XAudioSound::AdjustVolume(float value)
{
    return SetVolume(GetVolume() + value);
}

bool XAudioSound::SetPitch(float value)
{
    if(pSourceVoice) { 
        pSourceVoice->SetFrequencyRatio(std::max(std::min(value, maxPitch), XAUDIO2_MIN_FREQ_RATIO));
    } else {
        return false;
    }
    return true;
}

float XAudioSound::GetPitch() const
{
    float pitch = XAUDIO2_MIN_FREQ_RATIO;

    if(pSourceVoice) {
        pSourceVoice->GetFrequencyRatio(&pitch);
    }

    return pitch;
}

bool XAudioSound::AdjustPitch(float value)
{
    return SetPitch(GetPitch() + value);
}

void XAudioSound::SetMaxPitch(float value)
{
    maxPitch = std::max(std::min(value, 2.0f), XAUDIO2_MIN_FREQ_RATIO);
}

bool XAudioSound::UsingReverb(bool flag)
{
    if(pSourceVoice) {
        if(flag)
            pSourceVoice->EnableEffect(0);
        else
            pSourceVoice->DisableEffect(0);
    } else {
        return false;
    }

    return true;
}

