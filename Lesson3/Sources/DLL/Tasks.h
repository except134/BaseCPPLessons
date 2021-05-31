#pragma once

#ifdef DLL_EXPORT
#define LESSON3_API __declspec(dllexport)
#else
#define LESSON3_API __declspec(dllimport)
#endif

extern "C++" {
	extern int LESSON3_API a;
	extern int LESSON3_API b;
	extern int LESSON3_API c;
	extern int LESSON3_API d;

	void LESSON3_API Task1();
	void LESSON3_API Task2();
	void LESSON3_API Task3();
};
