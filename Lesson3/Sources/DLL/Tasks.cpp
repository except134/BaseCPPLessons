#include "Lesson3DLLPCH.h"

/*
�������� ���������, ����������� ��������� a* (b + (c / d)) � ��������� ��������� � ��������� ������, ��� a, b, c, d � 
������������� ���������.����������� static_cast ��� �Style cast � float ����� ��������� ������ �������.
���� ����� �����.���� ��� ������ ��� ����� 21, �� �������� �� ����� ������� ����� ���� ������ � ������ 21. 
���� �� �������� ����� ������, ��� 21, ���������� ������� ��������� ������� ����� ���� ������ � 21. 
��� ���������� ������� ������� ������������ ��������� ��������(? : ).
* ������� ��������� ������������� ������, �������� 3�3�3 � ��������� �� �������� ������ ������� � ��� ������ �������� 
������������� ������� �� ����� �������� ����������� ������ ������������� ����[1][1][1].
* *�������� ���������, ����������� ��������� �� ������� �������, � ���������� ��� �� ��������� 
� ���������������� � ������ cpp �����.����������� extern.
*/

int a{};
int b{};
int c{};
int d{};

void Task1()
{
	std::cout << "=================== Task 1 ===================" << std::endl;

	std::cout << "Enter 1 number: ";
	std::cin >> a;
	std::cout << "Enter 2 number: ";
	std::cin >> b;
	std::cout << "Enter 3 number: ";
	std::cin >> c;

	while (d == 0) {
		std::cout << "Enter 4 number (not 0): ";
		std::cin >> d;
	}

	float result = static_cast<float>(a * (b + (c / d)));

	std::cout << "Result is: " << result << std::endl << std::endl;

}

void Task2()
{
	std::cout << "=================== Task 2 ===================" << std::endl;

	std::cout << "Enter integer number: ";
	std::cin >> a;

	b = a <= 21 ? 21 - a : (a - 21) << 1;

	std::cout << "Result is: " << b << std::endl << std::endl;
}

void Task3()
{
	std::cout << "=================== Task 3 ===================" << std::endl;

	int myArray[3][3][3]{};
	int* ptrArray = &myArray[0][0][0];
	*(ptrArray + sizeof(int)*3+1) = 1;

	std::cout << "Result is: " << myArray[1][1][1] << std::endl << std::endl;


}

