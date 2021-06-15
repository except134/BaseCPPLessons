#include "ExceptEnginePCH.h"

using namespace ee;

XAudioOGGData::XAudioOGGData()  
{
    memset(fileName, 0, sizeof(fileName));
}

XAudioOGGData::~XAudioOGGData()
{
    this->Close();
}

bool XAudioOGGData::Close()
{
	return true;
}

bool XAudioOGGData::Open(const wchar_t* filename)
{
    wcscpy_s(fileName, _countof(fileName), filename);
    return true;
}

bool XAudioOGGData::LoadFromFile(const wchar_t* filename)
{
    if(Open(filename) == false)
		return false;

    Close();

	return true;
}

const wchar_t* XAudioOGGData::GetFileName() const
{
	return fileName;
}

