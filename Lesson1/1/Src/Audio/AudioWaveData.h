#pragma once

namespace ee
{
	class XAudioWaveData
	{
	public:
		XAudioWaveData();
		~XAudioWaveData();

		bool LoadFromFile(const wchar_t* FileName);
		const WAVEFORMATEX&     GetWaveFmtEx() const;
		const wchar_t*          GetFileName() const;
		const BYTE*             GetWaveData() const;
		const size_t            GetWaveSize() const;

	private:
		HMMIO               handle;           
		WAVEFORMATEX		WAVEFmtEx;        	
		BYTE*	            waveData;         
		size_t              waveDataSize;
		wchar_t             fileName[512];

	private:
		bool Open(const wchar_t* filename);
		bool Close();                           
		bool ReadToWaveFmtEx(size_t szFmtChunk);
		bool ReadToWaveData(size_t szDataSize);
	};
}

