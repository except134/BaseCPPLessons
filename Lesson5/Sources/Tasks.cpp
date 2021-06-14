#include "Lesson5PCH.h"

void Tasks::Task1MassiveWriter(double* var, size_t sz)
{
	for (int i = 0; i < sz; i++) {
		std::cout << var[i] << " ";
	}
}

void Tasks::Task1()
{
	std::cout << "=================== Задача 1 ===================" << std::endl << std::endl;

	double massive[] = { 1.12, 2.03, 3.14, 5.55, 6.67, 7.87 };

	Task1MassiveWriter(massive, sizeof(massive)/sizeof(massive[0]));

	std::cout << std::endl << std::endl;
}

void Tasks::Task2MassiveReverser(int* var, size_t sz)
{
	for (int i=0; i<sz; i++)
		var[i] = !var[i];
}

void Tasks::Task2()
{
	std::cout << "=================== Задача 2 ===================" << std::endl << std::endl;

	int massive[] = { 0,1,1,0,1,0,1,0,1,1,1,0,0 };

	for(auto i : massive)
		std::cout << i << " ";
	std::cout << std::endl;

	Task2MassiveReverser(massive, sizeof(massive) / sizeof(massive[0]));

	for (auto i : massive)
		std::cout << i << " ";
	std::cout << std::endl;

	std::cout << std::endl;
}

void Tasks::Task3MassiveFiller(int* var, size_t sz)
{
	for (int i = 0; i < sz; i++) {
		var[i] = i * 3 + 1;
	}
}

void Tasks::Task3()
{
	std::cout << "=================== Задача 3 ===================" << std::endl << std::endl;

	int massive[8];
	Task3MassiveFiller(massive, sizeof(massive) / sizeof(massive[0]));

	for (auto i : massive)
		std::cout << i << " ";
	std::cout << std::endl;

	std::cout << std::endl;
}

void Tasks::Task4MassiveSlider(int* var, size_t sz, int slide)
{
	std::valarray<int> valArray(var, sz);
	valArray = valArray.cshift(slide);
	
	for (int i = 0; i < sz; i++) {
		var[i] = valArray[i];
	}
}

void Tasks::Task4()
{
	std::cout << "=================== Задача 4 ===================" << std::endl << std::endl;

	int massive[] = { 1,2,3,4,5,6,7,8,9,0 };

	for (auto i : massive)
		std::cout << i << " ";
	std::cout << std::endl;

	Task4MassiveSlider(massive, sizeof(massive) / sizeof(massive[0]), 2);

	for (auto i : massive)
		std::cout << i << " ";
	std::cout << std::endl;

	std::cout << std::endl;
}

bool Tasks::Task5MassiveBalancer(int* var, size_t sz)
{
	int leftSum = 0;
	int rightSum = 0;
	for (int i = 0, j = 0; i < sz - j; i++) {
		leftSum += var[i];
		if (leftSum > rightSum && i < sz - j - 1)
			rightSum += var[sz - 1 - j++];
	}
	return rightSum == leftSum;
}

void Tasks::Task5()
{
	std::cout << "=================== Задача 5 ===================" << std::endl << std::endl;

	int massive1[]{ 1, 1, 1, 2, 1 };

	bool result = Task5MassiveBalancer(massive1, sizeof(massive1) / sizeof(massive1[0]));

	std::cout << "Массив [1,1,1,2,1] = " << std::boolalpha << result << std::endl;

	int massive2[]{ 2, 1, 1, 2, 1 };

	result = Task5MassiveBalancer(massive2, sizeof(massive2) / sizeof(massive2[0]));

	std::cout << "Массив [2,1,1,2,1] = " << std::boolalpha << result << std::endl;

	std::cout << std::endl;
}
