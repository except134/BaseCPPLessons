#pragma once

namespace ee
{
	class IniFile : NonCopyable
	{
	private:
		IniFile() { };
		virtual ~IniFile() { };

	public:
		static std::wstring GetString(const std::wstring& KeyName, const std::wstring& SectionName, const std::wstring& FileName) 
		{
			wchar_t tmp[MAX_PATH];
			GetPrivateProfileString(SectionName.c_str(), KeyName.c_str(), L"", tmp, MAX_PATH, FileName.c_str());
			return std::wstring(tmp);
		};

		static void	SetString(const std::wstring& KeyName, const std::wstring& Value, const std::wstring& SectionName, const std::wstring& FileName) 
		{
			WritePrivateProfileString(SectionName.c_str(), KeyName.c_str(), Value.c_str(), FileName.c_str());
		};

		static UINT_PTR GetInt(const std::wstring& KeyName, const std::wstring& SectionName, const std::wstring& FileName) 
		{
			std::wstring t = GetString(KeyName.c_str(), SectionName.c_str(), FileName.c_str());
			if (t == L"")
				return 0;
			return String::From<UINT_PTR>(t);
		};

		static void	SetInt(const std::wstring& KeyName, UINT_PTR Value, const std::wstring& SectionName, const std::wstring& FileName) 
		{
			SetString(KeyName.c_str(), std::to_wstring(Value), SectionName.c_str(), FileName.c_str());
		};

		static bool GetBool(const std::wstring& KeyName, const std::wstring& SectionName, const std::wstring& FileName) 
		{
			return String::ParseForBool(GetString(KeyName, SectionName, FileName));
		};

		static void SetBool(const std::wstring& KeyName, bool b, const std::wstring& SectionName, const std::wstring& FileName) 
		{
			SetString(KeyName, String::BoolToStrW(b), SectionName, FileName);
		};
	};
}

