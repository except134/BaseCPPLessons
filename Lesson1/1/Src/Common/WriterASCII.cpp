#include "ExceptEnginePCH.h"

using namespace ee;

WriterASCII::WriterASCII(EOutputType eType,const std::wstring& strName) : fileOut(nullptr),hWndOut(nullptr),nIndentation(0)
{
#ifdef USE_LOGGING
	switch(eType) {
		case OUTPUT_STDOUT:
			InitStdout();
		break;
		case OUTPUT_FILE:
			InitFile(strName);
		break;
		case OUTPUT_WINDOW:
			InitWindow(strName);
		break;
		case OUTPUT_NONE:
		default:
		break;
	}
#endif
}

bool WriterASCII::InitStdout()
{
#ifdef USE_LOGGING
	if(Ready()) {
		Close();
	}
	fileOut=stdout;
	if(fileOut==nullptr) {
		return false;
	}
	eOutputType=OUTPUT_STDOUT;
#endif
	return true;
}

bool WriterASCII::InitFile(const std::wstring& strFile)
{
#ifdef USE_LOGGING
	if(strFile==L"") {
		eOutputType=OUTPUT_NONE;
		return false;
	}

	if(Ready()) {
		Close();
	}

	SafeFopen(fileOut,strFile,L"wt");

	if(!fileOut) {
		wchar_t buf[MAX_PATH];
		swprintf_s(buf,L"Couldn't open %s for output logging.",strFile.c_str());
		throw std::exception(String::UnicodeToMultibyte(buf).c_str());
		return false;
	}
	eOutputType=OUTPUT_FILE;
#endif
	return true;
}

bool WriterASCII::InitWindow(const std::wstring& strTitle)
{
#ifdef USE_LOGGING
	if(Ready()) {
		Close();
	}

	hWndOut=CreateWindow(L"EDIT",strTitle.data(),WS_VISIBLE|WS_HSCROLL|WS_VSCROLL|ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_MULTILINE|ES_NOHIDESEL|ES_WANTRETURN|ES_READONLY,20,20,640,480,nullptr,nullptr,nullptr,nullptr);

	if(hWndOut==nullptr) {
		throw std::exception("Error creating a log window.");
		return false;
	}

	UpdateWindow(hWndOut);
	eOutputType=OUTPUT_WINDOW;
#endif
	return true;
}

bool WriterASCII::Close()
{
#ifdef USE_LOGGING
	switch(eOutputType) {
		case OUTPUT_NONE:
		break;
		case OUTPUT_STDOUT:
		break;
		case OUTPUT_FILE:
			SafeFclose(fileOut);
		break;
		case OUTPUT_WINDOW:
			if(hWndOut) {
				DestroyWindow(hWndOut);
				hWndOut=nullptr;
			}
		break;
		default:
		break;
	}

	eOutputType=OUTPUT_NONE;
	nIndentation=0;
#endif
	return true;
}

bool WriterASCII::Write(const wchar_t* szMessage,...) const
{
#ifdef USE_LOGGING
	if(eOutputType==OUTPUT_NONE) {
		return true;
	}

	if(Ready()==false) {
		return false;
	}

	static wchar_t szFormattedMessage[MAX_PATH];

	va_list arguments;
	va_start(arguments,szMessage);
	vswprintf_s(szFormattedMessage,szMessage,arguments);
	va_end(arguments);

#ifdef _DEBUG
	OutputDebugString(szFormattedMessage);
#endif

	size_t nCount,nLength=wcslen(szFormattedMessage);

	for(nCount=0;nCount<nLength;nCount++) {
		if(szFormattedMessage[nCount]==L'\n') {
			WriteChar(L'\n');
			if(eOutputType==OUTPUT_WINDOW) {
				WriteChar(L'\r');
			}
			WriteIndentation();
		} else {
			WriteChar(szFormattedMessage[nCount]);
		}
	}
	Flush();
#endif
	return true;
}

bool WriterASCII::Write(const wchar_t* file,const wchar_t* func,const int ln,const wchar_t* szMessage,...)	const
{
#ifdef USE_LOGGING
	if(eOutputType==OUTPUT_NONE) {
		return true;
	}

	if(Ready()==false) {
		return false;
	}

	static wchar_t szFormattedMessage[MAX_PATH];

	va_list arguments;
	va_start(arguments,szMessage);
	vswprintf_s(szFormattedMessage,szMessage,arguments);
	va_end(arguments);
	return Write(L"%s, file: %s, function: %s, line: %d",szFormattedMessage,file,func,ln);
#else
	return true;
#endif
}

bool WriterASCII::WriteChar(wchar_t chCar) const
{
#ifdef USE_LOGGING
	int nWindowTextLength;

	switch(eOutputType) {
		case OUTPUT_NONE:
			return true;
		case OUTPUT_STDOUT:
		case OUTPUT_FILE: {
			fwprintf(fileOut, L"%c", chCar);
			//fwrite(&chCar, 1, 1, fileOut);
		}break;
		case OUTPUT_WINDOW:
			nWindowTextLength=GetWindowTextLength(hWndOut)+1;
			std::wstring newText;
			newText.reserve(nWindowTextLength+1);
			GetWindowText(hWndOut,(wchar_t *)newText.data(),nWindowTextLength);
			newText[nWindowTextLength]=chCar;
			SetWindowText(hWndOut,newText.data());
		break;
	}
#endif
	return true;
}

bool WriterASCII::WriteIndentation() const
{
#ifdef USE_LOGGING
	if(eOutputType==OUTPUT_NONE) {
		return true;
	}

	for(int nCount=0;nCount<nIndentation;nCount++) {
		WriteChar(' ');
	}
#endif
	return true;
}

bool WriterASCII::Flush() const
{
#ifdef USE_LOGGING
	if(Ready()==false) {
		return false;
	}

	switch(eOutputType) {
		case OUTPUT_NONE:
			return true;
		case OUTPUT_STDOUT:
		case OUTPUT_FILE:
			if(fileOut) {
				fflush(fileOut);
			}
		break;
		case OUTPUT_WINDOW:
			if(hWndOut)
				UpdateWindow(hWndOut);
		break;
	}
#endif
	return true;
}

bool WriterASCII::Write(const std::wstring& szMessage) const
{
	return Write(szMessage.c_str());
}

bool WriterASCII::Write(const wchar_t* file,const wchar_t* func,const int ln,const std::wstring& szMessage) const
{
	return Write(file,func,ln,szMessage.c_str());
}

