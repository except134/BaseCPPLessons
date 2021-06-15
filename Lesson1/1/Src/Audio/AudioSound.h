#pragma once

namespace ee
{
	class XAudioSound
	{
	public:
		XAudioSound();
		virtual ~XAudioSound();

		bool			IsLoop() const { return isLoop; }
		bool			IsPlaying() const;
		void			SetLoopFlag(bool loopFlag);
		virtual bool	Play();
		bool			Stop();
		bool			RePlay();
		bool			Pause();
		bool			ExitLoop();
		bool			SetPitch(float value);
		float			GetPitch()const;
		bool			AdjustPitch(float value);
		void			SetMaxPitch(float value);
		bool			CreateSound(XAudioWaveData* pWavData);
		float			GetVolume()const;
		bool			SetVolume(float value);
		void			SetMaxVolume(float value);
		bool			AdjustVolume(float value);
		bool			SetPan(float value);
		bool			UsingReverb(bool flag);

	protected:
		bool			Submit();

	protected:
		XAudioWaveData*			pOriginData;  
		IXAudio2SourceVoice*	pSourceVoice; 
		bool                    isLoop;       
		float                   maxVolume;    
		float                   maxPitch;     
	};
}
