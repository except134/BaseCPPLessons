#include "ExceptEnginePCH.h"

using namespace ee;

bool ee::XAudio2Master::isInstance = false;

XAudio2Master::XAudio2Master() :
	pXAudioInterface(nullptr),
    pMasterVoice(nullptr)
{
    if(isInstance) {
		LOG.Write(L"AudioMaster not instanciated yet.");
        return;
    }

    isInstance = true;
    Initialize();
}

XAudio2Master::~XAudio2Master()
{
    isInstance = false;
    Release();
}

IXAudio2* XAudio2Master::GetInterface()const
{
    return pXAudioInterface;
}

IXAudio2MasteringVoice* XAudio2Master::GetMasteringVoice()const
{
    return pMasterVoice;
}

bool XAudio2Master::Initialize()
{
    if(FAILED(CoInitializeEx(NULL, COINIT_MULTITHREADED)) && FAILED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED))) {
		LOG.Write(L"AudioMaster could not CoInitialize COM library.");
        return false;
    }

	UINT32 flags = 0;

#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

    if(FAILED(XAudio2Create(&pXAudioInterface, 0)))
        return false;

    if(FAILED(pXAudioInterface->CreateMasteringVoice(&pMasterVoice)))
        return false;

	DWORD channelMask = SPEAKER_STEREO;
	pMasterVoice->GetChannelMask(&channelMask);

    X3DAudioInitialize(channelMask, X3DAUDIO_SPEED_OF_SOUND, x3DHandle);

    return true;
}

void XAudio2Master::Release()
{
    if(pMasterVoice)
        pMasterVoice->DestroyVoice();
    if(pXAudioInterface)
        pXAudioInterface->Release();
    CoUninitialize();
}

XAudio2Master::_3DSOUND_PARMETER XAudio2Master::Calculate3DSound(const X3DAUDIO_LISTENER *pListener, const X3DAUDIO_EMITTER  *pEmitter)
{
    X3DAUDIO_DSP_SETTINGS dsp = { 0 };

    FLOAT32 matrix[12] = { 0 };
    dsp.SrcChannelCount = 1;
    dsp.DstChannelCount = 2;
    dsp.pMatrixCoefficients = matrix;

    X3DAudioCalculate( x3DHandle, 
                       pListener, 
                       pEmitter, 
                       X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER , 
                       &dsp);

    _3DSOUND_PARMETER result;
    result.doppler = dsp.DopplerFactor;
    result.dstChannnel = dsp.DstChannelCount;
    result.EmitterToListenerAngle = dsp.EmitterToListenerAngle;
    result.EmitterToListenerDistance = dsp.EmitterToListenerDistance;
    result.EmitterVelocityComponent = dsp.EmitterVelocityComponent;
    result.ListenerVelocityComponent = dsp.ListenerVelocityComponent;
    result.LPFDirectCoefficient = dsp.LPFDirectCoefficient;
    result.LPFReverbCoefficient = dsp.LPFReverbCoefficient;
    memcpy(result.matrix, dsp.pMatrixCoefficients, sizeof(matrix));
    result.ReverbLevel = dsp.ReverbLevel;
    result.srcChannel = dsp.SrcChannelCount;

    return result;
}

