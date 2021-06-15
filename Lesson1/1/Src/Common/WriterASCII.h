#pragma once

namespace ee
{
	class WriterASCII
	{
	protected:
		enum EOutputType {
			OUTPUT_NONE=0,
			OUTPUT_STDOUT,
			OUTPUT_FILE,
			OUTPUT_WINDOW
		};

	private:
		EOutputType				eOutputType;
		FILE*					fileOut;
		HWND					hWndOut;
		int						nIndentation;

		bool WriteIndentation	() const;
		bool Flush				() const;
		bool WriteChar			(wchar_t chCar)	const;

	public:
		WriterASCII				() : eOutputType(OUTPUT_NONE),fileOut(nullptr),hWndOut(nullptr),nIndentation(0) {};
		WriterASCII				(EOutputType eType,const std::wstring& strName);
		~WriterASCII			() { Close(); };

		bool InitStdout			();
		bool Close				();
		bool InitFile			(const std::wstring& strFile);
		bool InitWindow			(const std::wstring& strTitle);
		bool Write				(const wchar_t* szMessage,...) const;
		bool Write				(const wchar_t* file,const wchar_t* func,const int ln,const wchar_t* szMessage,...) const;
		bool Write				(const std::wstring& szMessage) const;
		bool Write				(const wchar_t* file,const wchar_t* func,const int ln,const std::wstring& szMessage) const;
		bool Ready				()					const	{ return eOutputType!=OUTPUT_NONE; };
		int  Indentation		()					const	{ return nIndentation; };
		void SetIndentation		(int nIndentChars)			{ nIndentation=nIndentChars; };
		void IncIndentation		(int nInc)					{ nIndentation+=nInc; };
		void DecIndentation		(int nDec)					{ nIndentation-=nDec; if(nIndentation<0){nIndentation=0;} };
	};
}



