#include "ExceptEnginePCH.h"

using namespace ee;

XAudioWaveData::XAudioWaveData() : 
	handle(nullptr)
{
	ZeroMemory(&WAVEFmtEx, sizeof(WAVEFmtEx));
    memset(fileName, 0, sizeof(fileName));
}

XAudioWaveData::~XAudioWaveData()
{
    if(waveData)
        delete waveData;

    this->Close();
}

bool XAudioWaveData::Close()
{
    if(handle == nullptr)
        return true;

	if(mmioClose(handle, 0) == MMIOERR_CANNOTWRITE) {
		return false;
	}
    handle = nullptr;
	return true;
}

bool XAudioWaveData::Open(const wchar_t* filename)
{
    MMIOINFO mmio_info;
    memset(&mmio_info, 0, sizeof(MMIOINFO));

    if(!(handle = mmioOpen((wchar_t*)filename, &mmio_info, MMIO_READ))) {
        return false;
    }
    wcscpy_s(fileName, _countof(fileName), filename);
    return true;
}

bool XAudioWaveData::ReadToWaveFmtEx(size_t szFmtChunk)
{
    LONG wReadSize = mmioRead(handle, reinterpret_cast<HPSTR>(&WAVEFmtEx), (LONG)szFmtChunk);

    if(wReadSize != szFmtChunk)
        return false;

    return true;
}

bool XAudioWaveData::ReadToWaveData(size_t szDataSize)
{
    waveDataSize = szDataSize;
    waveData = new BYTE[waveDataSize];

    if(mmioRead(handle, reinterpret_cast<HPSTR>(waveData), (LONG)waveDataSize) != waveDataSize) {
		return false;
	}

	return true;
}

bool XAudioWaveData::LoadFromFile(const wchar_t* filename)
{
    MMCKINFO			riffChunk;    
    MMCKINFO			fmtChunk;     
    MMCKINFO			dataChunk;    
   
    auto CheckChankHeader = [&](MMCKINFO* chunk1, MMCKINFO* chunk2, char c0, char c1, char c2, char c3, UINT flag) {
        FOURCC* rcc = nullptr;
        if(flag == MMIO_FINDRIFF)
            rcc = &chunk1->fccType;
        if(flag == MMIO_FINDCHUNK)
            rcc = &chunk1->ckid;
        (*rcc) = mmioFOURCC(c0, c1, c2, c3);

        MMRESULT result = mmioDescend(handle, chunk1, chunk2, flag);
        if(result != MMSYSERR_NOERROR) {
            return false;
        }
        return true;
    };

    if(Open(filename) == false)
		return false;

    if(CheckChankHeader(&riffChunk, nullptr, 'W', 'A', 'V', 'E', MMIO_FINDRIFF) == false)
		return false;

    if(CheckChankHeader(&fmtChunk, &riffChunk, 'f', 'm', 't', ' ', MMIO_FINDCHUNK) == false)
		return false;

	if(ReadToWaveFmtEx(fmtChunk.cksize) == false)
		return false;

	mmioAscend(handle, &fmtChunk, SEEK_SET);

    if(CheckChankHeader(&dataChunk, &riffChunk, 'd', 'a', 't', 'a', MMIO_FINDCHUNK) == false)
        return false;

	if(ReadToWaveData(dataChunk.cksize) == FALSE)
		return false;

    Close();

	return true;
}

const wchar_t* XAudioWaveData::GetFileName() const
{
	return fileName;
}

const WAVEFORMATEX& XAudioWaveData::GetWaveFmtEx() const
{
	return WAVEFmtEx;
}

const BYTE* XAudioWaveData::GetWaveData() const
{
    return waveData;
}

const size_t XAudioWaveData::GetWaveSize() const
{
	return waveDataSize;
}
