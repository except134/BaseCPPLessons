#pragma once

namespace ee
{
	class Logger : public WriterASCII,public SingletonStatic<Logger>
	{
	private:
		bool WriteStartHeader();
		bool WriteEndHeader();

	public:
		Logger();
		~Logger();

		bool Start(const std::wstring& strFile);
		void FakeInit() {};
	};
}

#define LOG	ee::Logger::Instance()

#ifdef _DEBUG
#	define EE_ASSERT(expr) do { if (!(expr)) { Errors::Fatal(#expr); } } while (0) 
#else  
#	define EE_ASSERT(expr)	do { (void)sizeof(expr); } while(0) 
#endif

