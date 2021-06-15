#pragma once

namespace ee
{
	class XAudio2Master : public SingletonStatic<XAudio2Master>
	{
	public:
		struct _3DSOUND_PARMETER
		{
			int		srcChannel;
			int		dstChannnel;
			float	matrix[12];
			float	doppler;
			float	EmitterToListenerAngle;
			float	EmitterToListenerDistance;
			float	EmitterVelocityComponent;
			float	ListenerVelocityComponent;
			float	ReverbLevel;
			float	LPFReverbCoefficient;
			float	LPFDirectCoefficient;
		};

		XAudio2Master();
		virtual ~XAudio2Master();

		IXAudio2*				GetInterface() const;
		_3DSOUND_PARMETER		Calculate3DSound(const X3DAUDIO_LISTENER *pListener, const X3DAUDIO_EMITTER  *pEmitter);
		IXAudio2MasteringVoice* GetMasteringVoice() const;

	protected:
		bool Initialize();
		void Release();

	protected:
		X3DAUDIO_HANDLE         x3DHandle;
		IXAudio2 *              pXAudioInterface;                
		IXAudio2MasteringVoice*	pMasterVoice;                    
		static bool				isInstance;
	};
}
