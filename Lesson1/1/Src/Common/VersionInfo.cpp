#include "ExceptEnginePCH.h"

using namespace ee;

VersionInfo::VersionInfo(VS_FIXEDFILEINFO* pver)
{
	SetFromFFI(pver);
}

VersionInfo::VersionInfo(const VersionInfo& ver)
{
	SetFromVerInfo(ver);
}

VersionInfo::VersionInfo(const wchar_t* strVer)
{
	Reset();
	SetFromString(strVer);
}

VersionInfo::VersionInfo(HMODULE hmod)
{
	SetFromInstance(hmod);
}

VersionInfo::VersionInfo()
{
	Reset();
}

VersionInfo::~VersionInfo()
{

}

void VersionInfo::Reset()
{
	verMajor=0;
	verMinor=0;
	verAux=0;
	verMaintenance=0;
	productName.assign(L"");
	companyName.assign(L"");
	legalCopyright.assign(L"");
}

void VersionInfo::SetFromVerInfo(const VersionInfo& ver)
{
	verMajor=ver.verMajor;
	verMinor=ver.verMinor;
	verAux=ver.verAux;
	verMaintenance=ver.verMaintenance;
	productName=ver.productName;
	companyName=ver.companyName;
	legalCopyright=ver.legalCopyright;
}

void VersionInfo::SetFromFFI(VS_FIXEDFILEINFO* pver)
{
	verMajor=HIWORD(pver->dwFileVersionMS);
	verMinor=LOWORD(pver->dwFileVersionMS);
	verAux=HIWORD(pver->dwFileVersionLS);
	verMaintenance=LOWORD(pver->dwFileVersionLS);
}

void VersionInfo::SetFromString(const wchar_t* strVer)
{
	const wchar_t chDelim='.';
	const wchar_t* pch=strVer;
	
	HPVER *ppVer[]={&verMajor, 
					&verMinor, 
					&verAux, 
					&verMaintenance, 
					nullptr};

	HPVER** ppVerNext=ppVer;

	while(pch&&*pch) {
/*
#ifdef _UNICODE
		**ppVerNext=(HPVER)_wtoi(pch);
		pch=wcschr(pch,chDelim);
#else
*/
		**ppVerNext=_wtoi(pch);
		pch=wcschr(pch,chDelim);
//#endif
		if(pch) 
			pch++;
		ppVerNext++;
		if(!*ppVerNext)
			break;
	}
}

void VersionInfo::SetFromInstance(HMODULE hmod)
{
	wchar_t szFile[MAX_PATH];

	if(hmod==nullptr) {
		Reset();
		//return;
	}
	GetModuleFileName(hmod,szFile,sizeof(szFile));
	GetVersionOfFile(szFile);
}

void VersionInfo::SetFromFilename(const wchar_t* tszFile)
{
	GetVersionOfFile(tszFile);
}

void VersionInfo::SetMajor(HPVER verMajor)
{
	verMajor=verMajor;
}

void VersionInfo::SetMinor(HPVER verMinor)
{
	verMinor=verMinor;
}

void VersionInfo::SetAux(HPVER verAux)
{
	verAux=verAux;
}

void VersionInfo::SetMaintenance(HPVER verMaintenance)
{
	verMaintenance=verMaintenance;
}


void VersionInfo::AsResource(wchar_t* str) const
{
	swprintf_s(str,MAX_PATH,L"%d,%d,%d,%d",verMajor,verMinor,verAux,verMaintenance);
}

wchar_t* VersionInfo::AsString(wchar_t* tszStrOut) const
{
	swprintf_s(tszStrOut,MAX_PATH,L"%d.%d.%d.%d",verMajor,verMinor,verAux,verMaintenance);
	return tszStrOut;
}

wchar_t* VersionInfo::AsStringShort(wchar_t* tszStrOut) const
{
	swprintf_s(tszStrOut,MAX_PATH,L"%d.%d",verMajor,verMinor);
	return tszStrOut;
}

const wchar_t* VersionInfo::AsString()
{
	return AsString(strOut);
}

const wchar_t* VersionInfo::AsString(std::wstring& str) const
{
	wchar_t tszVer[CBVERSTR];
	str=AsString(tszVer);
	return str.data();
}

const wchar_t* VersionInfo::AsStringShort(std::wstring& str) const
{
	wchar_t tszVer[CBVERSTR];
	str=AsStringShort(tszVer);
	return str.data();
}

const wchar_t* VersionInfo::GetCompanyName() const
{
	return companyName.data();
}

const wchar_t* VersionInfo::GetExtProductName(std::wstring* pstr) const
{
	std::wstring str;
	pstr->assign(productName);
	pstr->append(L" ");
	pstr->append(AsStringShort(str));
	return pstr->data();
}

const wchar_t* VersionInfo::GetProductName() const
{
	return productName.data();
}

const wchar_t* VersionInfo::GetLegalInfo() const
{
	return legalCopyright.data();
}

unsigned int VersionInfo::Compare(const VersionInfo& ver) const
{
	HPVER rgVerThis[]={verMajor,verMinor,verAux,verMaintenance};
	HPVER rgVer[]={ver.verMajor,ver.verMinor,ver.verAux,ver.verMaintenance};
	int ixRg=0;

	assert(sizeof(rgVerThis)==sizeof(rgVer));

	while(ixRg<(sizeof(rgVerThis)/sizeof(HPVER))) {
		if(rgVerThis[ixRg]>rgVer[ixRg])
			return 1;
		if(rgVerThis[ixRg]<rgVer[ixRg])
			return -1;
		ixRg++;
	}
	return 0;
}

VersionInfo& VersionInfo::operator=(const VersionInfo& ver)
{
	SetFromVerInfo(ver);
	return *this;
}

bool VersionInfo::operator==(const VersionInfo& ver) const
{
	return (Compare(ver)==0);
}

bool VersionInfo::operator<(const VersionInfo& ver) const
{
	return (Compare(ver)==-1);
}

bool VersionInfo::operator>(const VersionInfo& ver) const
{
	return (Compare(ver)==1);
}

void GetVersionString(wchar_t* lpVI,wchar_t* tszStr,unsigned short* lng,std::wstring* pstr)
{
	DWORD dwBufSize; 
	wchar_t tszVerStrName[CBVERSTR];
	LPVOID lpt;

	swprintf_s(tszVerStrName,L"\\StringFileInfo\\%04x%04x\\%s",lng[0],lng[1],tszStr);
	if(VerQueryValue(lpVI,tszVerStrName,&lpt,(unsigned int*)&dwBufSize))
		pstr->assign((wchar_t*)lpt);
}

BOOL VersionInfo::GetVersionOfFile(const wchar_t* szFile) 
{
	unsigned long dwBufSize; 
	VS_FIXEDFILEINFO* lpFFI; 
	wchar_t* lpVI=GetVersionInfo(szFile); 
	BOOL bRet=false;
	unsigned short* langInfo;
	unsigned int cbLang;

	if(!lpVI)
		return false;
	
	if(bRet=VerQueryValue(lpVI,L"\\",(void**)&lpFFI,(unsigned int*)&dwBufSize)) {
		SetFromFFI(lpFFI);
	}
	
	VerQueryValue(lpVI,L"\\VarFileInfo\\Translation",(void**)&langInfo,&cbLang);
	GetVersionString(lpVI,L"ProductName",langInfo,&productName);
	GetVersionString(lpVI,L"CompanyName",langInfo,&companyName);
	GetVersionString(lpVI,L"LegalCopyright",langInfo,&legalCopyright);
	GlobalFree((HGLOBAL)lpVI); 
	return bRet; 
} 

wchar_t* VersionInfo::GetVersionInfo(const wchar_t* szFile) 
{
	unsigned long dwLen,dwUseless; 
	wchar_t* lpVI; 

	dwLen=GetFileVersionInfoSize((wchar_t*)szFile,&dwUseless); 
	if(dwLen==0) 
		return 0; 

	lpVI=(wchar_t*)GlobalAlloc(GPTR,dwLen); 
	if(lpVI) {
		GetFileVersionInfo((wchar_t*)szFile,0,dwLen,lpVI); 
		return lpVI;
	}   
	return nullptr; 
}

unsigned short VersionInfo::GetVerAux()
{
	return verAux;
}

