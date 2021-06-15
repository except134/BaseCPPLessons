#pragma once

namespace ee
{
	class XAudioOGGData
	{
	public:
		XAudioOGGData();
		~XAudioOGGData();

		bool				LoadFromFile(const wchar_t* FileName);
		const wchar_t*      GetFileName() const;

		wchar_t             fileName[512];

	private:
		bool				Open(const wchar_t* filename);
		bool				Close();                           
	};
}

