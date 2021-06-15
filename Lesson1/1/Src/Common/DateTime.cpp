#include "ExceptEnginePCH.h"

using namespace ee;

DateTime::DateTime()
{

}

DateTime::~DateTime()
{

}

std::wstring DateTime::GetCurDate()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	wchar_t buf[MAX_PATH];
	swprintf_s(buf,MAX_PATH,L"%02u.%02u.%4u",sysTime.wDay,sysTime.wMonth,sysTime.wYear);
	return std::wstring(buf);
}

std::wstring DateTime::GetCurTime()
{
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	wchar_t buf[MAX_PATH];
	swprintf_s(buf,MAX_PATH,L"%02u:%02u:%02u",sysTime.wHour,sysTime.wMinute,sysTime.wSecond);
	return std::wstring(buf);
}

std::wstring DateTime::GetCurDateTime()
{
	return std::wstring(GetCurDate()+L" "+GetCurTime());
}

