#include "ExceptEnginePCH.h"

using namespace ee;

XAudio3DSound::XAudio3DSound() :
	XAudioSound(),
    using3DSound(true)
{
    memset(&emitterLocal, 0, sizeof(emitterLocal));
    memset(&listenerLocal, 0, sizeof(listenerLocal));

    SetEmitterFront(0, 0, 1);
    SetEmitterTop(0, 1, 0);
    SetEmitterPostion(0, 0, -1);

    SetListenerFront(0, 0, -1);
    SetListenerTop(0, 1, 0);
    SetListenerPostion(0, 0, 1);

    emitterLocal.ChannelCount = 1;
    emitterLocal.CurveDistanceScaler = 1.0f;
    emitterLocal.DopplerScaler = 1.0f;
    emitterLocal.InnerRadius = 1;
}

bool XAudio3DSound::Play()
{
    Apply();
    return XAudioSound::Play();
}

XAudio3DSound::~XAudio3DSound()
{
}

void XAudio3DSound::SetListenerFront(float x, float y, float z)
{
    listenerLocal.front_x = x;
    listenerLocal.front_y = y;
    listenerLocal.front_z = z;
}

void XAudio3DSound::SetListenerTop(float x, float y, float z)
{
    listenerLocal.up_x = x;
    listenerLocal.up_y = y;
    listenerLocal.up_z = z;
}

void XAudio3DSound::SetListenerPostion(float x, float y, float z)
{
    listenerLocal.postion_x = x;
    listenerLocal.postion_y = y;
    listenerLocal.postion_z = z;
}

void XAudio3DSound::SetListenervelocity(float x, float y, float z)
{
    listenerLocal.velocity_x = x;
    listenerLocal.velocity_y = y;
    listenerLocal.velocity_z = z;
}

void XAudio3DSound::SetEmitterFront(float x, float y, float z)
{
    emitterLocal.front_x = x;
    emitterLocal.front_y = y;
    emitterLocal.front_z = z;
}

void XAudio3DSound::SetEmitterTop(float x, float y, float z)
{
    emitterLocal.up_x = x;
    emitterLocal.up_y = y;
    emitterLocal.up_z = z;
}

void XAudio3DSound::SetEmitterPostion(float x, float y, float z)
{
    emitterLocal.postion_x = x;
    emitterLocal.postion_y = y;
    emitterLocal.postion_z = z;
}

void XAudio3DSound::SetEmittervelocity(float x, float y, float z)
{
    emitterLocal.velocity_x = x;
    emitterLocal.velocity_y = y;
    emitterLocal.velocity_z = z;
}

void XAudio3DSound::SetEmitterDopplerLevel(float value)
{
    emitterLocal.DopplerScaler = value;
}

void XAudio3DSound::SetEmitterRadius(float value)
{
    emitterLocal.InnerRadius = value;
}

bool XAudio3DSound::Apply()
{
    if(pSourceVoice == nullptr)
        return false;

    XAudio2Master& xAudioSystem = XAudio2Master::Instance();

    X3DAUDIO_LISTENER listener;
    memset(&listener, 0, sizeof(listener));
    {
        listener.OrientFront.x = listenerLocal.front_x;
        listener.OrientFront.y = listenerLocal.front_y;
        listener.OrientFront.z = listenerLocal.front_z;

        listener.OrientTop.x = listenerLocal.up_x;
        listener.OrientTop.y = listenerLocal.up_y;
        listener.OrientTop.z = listenerLocal.up_z;

        listener.Position.x = listenerLocal.postion_x;
        listener.Position.y = listenerLocal.postion_y;
        listener.Position.z = listenerLocal.postion_z;

        listener.Velocity.x = listenerLocal.velocity_x;
        listener.Velocity.y = listenerLocal.velocity_y;
        listener.Velocity.z = listenerLocal.velocity_z;
    }

    X3DAUDIO_EMITTER  emitter;
    memset(&emitter, 0, sizeof(emitter));
    {
        emitter.OrientFront.x = emitterLocal.front_x;
        emitter.OrientFront.y = emitterLocal.front_y;
        emitter.OrientFront.z = emitterLocal.front_z;

        emitter.OrientTop.x = emitterLocal.up_x;
        emitter.OrientTop.y = emitterLocal.up_y;
        emitter.OrientTop.z = emitterLocal.up_z;

        emitter.Position.x = emitterLocal.postion_x;
        emitter.Position.y = emitterLocal.postion_y;
        emitter.Position.z = emitterLocal.postion_z;

        emitter.Velocity.x = emitterLocal.velocity_x;
        emitter.Velocity.y = emitterLocal.velocity_y;
        emitter.Velocity.z = emitterLocal.velocity_z;

        emitter.ChannelCount = emitterLocal.ChannelCount;
        emitter.ChannelRadius = emitterLocal.ChannelRadius;
        emitter.CurveDistanceScaler = emitterLocal.CurveDistanceScaler;
        emitter.DopplerScaler = emitterLocal.DopplerScaler;
        emitter.InnerRadius = emitterLocal.InnerRadius;
        emitter.InnerRadiusAngle = emitterLocal.InnerRadiusAngle;
    }

    XAudio2Master::_3DSOUND_PARMETER param = xAudioSystem.Calculate3DSound(&listener, &emitter);

    if(pSourceVoice) {
        pSourceVoice->SetOutputMatrix(nullptr, param.srcChannel, param.dstChannnel, param.matrix);
        pSourceVoice->SetFrequencyRatio(param.doppler);
    }
    
    return true;
}

