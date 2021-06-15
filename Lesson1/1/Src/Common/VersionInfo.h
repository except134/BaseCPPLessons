#pragma once

namespace ee
{
	typedef unsigned short HPVER;

#define CBVERSTR 64

	class VersionInfo  
	{
	private:
		wchar_t strOut[MAX_PATH];

		HPVER verMaintenance;
		HPVER verAux;
		HPVER verMinor;
		HPVER verMajor;

		std::wstring productName;
		std::wstring companyName;
		std::wstring legalCopyright;

	protected:
		BOOL GetVersionOfFile(const wchar_t* szFile);
		void Reset();

	public:
		VersionInfo(const VersionInfo& ver);
		VersionInfo(VS_FIXEDFILEINFO*);
		VersionInfo(const wchar_t* strVer);
		VersionInfo(HMODULE);     
		VersionInfo();
		virtual ~VersionInfo();

		void SetFromString(const wchar_t*);
		void SetFromFFI(VS_FIXEDFILEINFO*);
		void SetFromVerInfo(const VersionInfo&);
		void SetMajor(HPVER verMajor);
		void SetMinor(HPVER verMinor);
		void SetAux(HPVER verAux);
		void SetMaintenance(HPVER verMaintenance);
		void SetFromInstance(HMODULE);
		void SetFromFilename(const wchar_t*);

		void AsResource(wchar_t*) const;  
		const wchar_t* AsString(std::wstring& str) const;
		const wchar_t* AsStringShort(std::wstring& str) const;
		wchar_t* AsStringShort(wchar_t*) const;
		wchar_t* AsString(wchar_t*) const;
		const wchar_t* AsString();

		const wchar_t* GetExtProductName(std::wstring*) const;
		const wchar_t* GetProductName() const;
		const wchar_t* GetCompanyName() const;
		const wchar_t* GetLegalInfo() const;

		unsigned int Compare(const VersionInfo& ver) const;

		VersionInfo& operator=(const VersionInfo& ver);
		bool operator==(const VersionInfo& ver) const;
		bool operator<(const VersionInfo& ver) const;
		bool operator>(const VersionInfo& ver) const;

		static wchar_t* GetVersionInfo(const wchar_t* szFile);

		unsigned short GetVerAux();
	};
}

