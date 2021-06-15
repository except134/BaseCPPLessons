#include "ExceptEnginePCH.h"

using namespace ee;

Logger::Logger()
{
}

Logger::~Logger()
{
#ifdef USE_LOGGING
	this->WriteEndHeader();
	this->Close();
#endif
}

bool Logger::WriteStartHeader()
{
#ifdef USE_LOGGING
	Write(L"======================================================================================\n");
	Write(L"  %s, version %s\n", Environment::GetAppName().c_str(), TEXT(EXCEPTENGINE_VERSION_STRING));
	Write(L"======================================================================================\n");
	Write(L"  Executable file:     %s\n", Environment::GetExecutableFullFileName().c_str());
	Write(L"  Build architecture:  %s\n", Environment::GetArch().c_str());
	Write(L"  Build configuration: %s\n", Environment::GetBuild().c_str());
	Write(L"  Build date/time:     %s, %s\n", String::MultibyteToUnicode(__DATE__).c_str(), String::MultibyteToUnicode(__TIME__).c_str());
	Write(L"  Compiled with:       %s\n", Environment::GetCompilatorInfo().c_str());
	Write(L"======================================================================================\n");
	Write(L"  Log created on %s, at %s\n", DateTime::GetCurDate().c_str(), DateTime::GetCurTime().c_str());
	Write(L"======================================================================================\n\n");

#endif
	return true;
}

bool Logger::WriteEndHeader()
{
#ifdef USE_LOGGING
	DateTime dt;
	Write(L"\n\n======================================================================================\n");
	Write(L"  Log closed on %s, at %s\n",dt.GetCurDate().c_str(),dt.GetCurTime().c_str());
	Write(L"======================================================================================\n\n");
#endif
	return true;
}

bool Logger::Start(const std::wstring& strFile)
{
#ifdef USE_LOGGING
	setlocale(LC_ALL, ".ACP");
	this->InitFile(strFile);
	this->WriteStartHeader();
#endif
	return true;
}

