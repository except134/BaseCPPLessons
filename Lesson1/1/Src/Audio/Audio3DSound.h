#pragma once

namespace ee
{
	class XAudio3DSound : public XAudioSound
	{
	public:
		struct AUDIO_LISTENER
		{
			float front_x, front_y, front_z;
			float up_x, up_y, up_z;
			float postion_x, postion_y, postion_z;
			float velocity_x, velocity_y, velocity_z;
		};

		struct AUDIO_EMITTER : AUDIO_LISTENER
		{
			float			InnerRadius;
			float			InnerRadiusAngle;

			unsigned int	ChannelCount;
			float			ChannelRadius;

			float			CurveDistanceScaler;
			float			DopplerScaler;
		};

		XAudio3DSound();
		~XAudio3DSound();

		virtual bool	Play() override;
		void			SetListenerFront(float x, float y, float z);
		void			SetListenerTop(float x, float y, float z);
		void			SetListenerPostion(float x, float y, float z);
		void			SetListenervelocity(float x, float y, float z);
		void			SetEmitterFront(float x, float y, float z);
		void			SetEmitterTop(float x, float y, float z);
		void			SetEmitterPostion(float x, float y, float z);
		void			SetEmittervelocity(float x, float y, float z);
		void			SetEmitterDopplerLevel(float value);
		void			SetEmitterRadius(float value);
		bool			Apply();

	protected:
		bool			using3DSound;
		AUDIO_EMITTER   emitterLocal;
		AUDIO_LISTENER  listenerLocal;
	};
}
