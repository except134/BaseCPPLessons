#include "Lesson4PCH.h"

void Tasks::Task1()
{
	std::cout << "=================== ������ 1 ===================" << std::endl << std::endl;

	int num1 = 0, num2 = 0;

	std::cout << "������� ����� ����� 1: ";
	std::cin >> num1;
	std::cout << std::endl;

	std::cout << "������� ����� ����� 2: ";
	std::cin >> num2;
	std::cout << std::endl;

	int summary = num1 + num2;

	bool result = summary >= 10 && summary <= 20 ? true : false;

	std::cout << std::boolalpha << result << std::endl;

	std::cout << std::endl;
}

void Tasks::Task2()
{
	std::cout << "=================== ������ 2 ===================" << std::endl << std::endl;

	const int num1 = 5;
	const int num2 = 5;

	bool result = (num1==10 && num2==10) || (num1+num2 == 10) ? true : false;

	std::cout << std::boolalpha << result << std::endl;

	std::cout << std::endl;
}

void Tasks::Task3()
{
	std::cout << "=================== ������ 3 ===================" << std::endl << std::endl;

	for (int i = 1; i <= 50; i += 2) {
		std::cout << i << " ";
	}

	std::cout << std::endl << std::endl;
}

void Tasks::Task4()
{
	std::cout << "=================== ������ 4 ===================" << std::endl << std::endl;

	std::cout << "������� ����� �����: ";

	int num = 0;

	std::cin >> num;

	bool result{ false };

	for (int i = 2; i <= num; i++) {
		if (num % i == 0)
		{
			result = true;
			break;
		}
	}

	std::cout << std::boolalpha << result << std::endl;

	std::cout << std::endl;
}

void Tasks::Task5()
{
	std::cout << "=================== ������ 5 ===================" << std::endl << std::endl;

	std::cout << std::endl;
}
