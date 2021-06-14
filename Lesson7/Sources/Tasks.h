#pragma once

namespace nsTask5
{
	void InitializeArray(float*, size_t sz);
	void PrintArray(float*, size_t sz);
	void CalculateArray(float*, size_t sz);
}

class Tasks
{
public:
	void Task1();
	void Task2();
	void Task3();
	void Task4();
	void Task5();

private:
#pragma pack(push,1)
	struct Employer
	{
		int id;
		std::string name;
		int age;
		double salary;
	};
#pragma pack(pop)

private:
	void Task3PrintArray(int* arr, int sz);
	void Task3SortArray(int* arr, int sz);
	std::stringstream Task4OutString(Employer* employer);
};