#include "Lesson7PCH.h"

// Создайте проект из 2х cpp файлов и заголовочного(main.cpp, mylib.cpp, mylib.h) 
// во втором модуле mylib объявить 3 функции: для инициализации массива(типа float), 
// печати его на экран и подсчета количества отрицательных и положительных элементов.Вызывайте эти 3 - и функции из main для работы с массивом.

void Tasks::Task1()
{
	std::cout << "=================== Задача 1 ===================" << std::endl << std::endl;

	std::cout << "Реализована в задаче 5." << std::endl;

	std::cout << std::endl << std::endl;
}

// Описать макрокоманду(через директиву define), проверяющую, входит ли переданное ей число(введенное с клавиатуры) 
// в диапазон от нуля(включительно) до переданного ей второго аргумента(исключительно) и возвращает true или false, 
// вывести на экран «true» или «false».

#define TASK2_MACROS(a, b) a>=0 && a<b ? true : false

void Tasks::Task2()
{
	std::cout << "=================== Задача 2 ===================" << std::endl << std::endl;

	std::cout << "Введите число для проверки: ";
	int num;
	std::cin >> num;

	std::cout << "Введите максимальное число: ";
	int numMax;
	std::cin >> numMax;

	bool result = TASK2_MACROS(num, numMax);

	std::cout << "Число " << num << (result ? " входит" : " не входит") << " в диапозон 0-" << numMax-1;

	std::cout << std::endl << std::endl;
}

// Задайте массив типа int.Пусть его размер задается через директиву препроцессора #define.
// Инициализируйте его через ввод с клавиатуры.Напишите для него свою функцию сортировки(например Пузырьком).
// Реализуйте перестановку элементов как макрокоманду SwapINT(a, b).Вызывайте ее из цикла сортировки.

#define ARR_SIZE 5

#define SwapINT(a, b) std::swap(a, b)

void Tasks::Task3PrintArray(int* arr, int sz)
{
	for (int i = 0; i < sz; i++)
		std::cout << arr[i] << " ";
	std::cout << std::endl;
}

void Tasks::Task3SortArray(int* arr, int sz)
{
	for (int i = 0; i < sz - 1; i++) {
		for (int j = 0; j < sz - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				SwapINT(arr[j], arr[j + 1]);
			}
		}
	}
}

void Tasks::Task3()
{
	std::cout << "=================== Задача 3 ===================" << std::endl << std::endl;

	int arr[ARR_SIZE];

	std::cout << "Введите числa через пробел для массива из " << ARR_SIZE << " элементов: " << std::endl;

	for (int i = 0; i < ARR_SIZE; i++) {
		std::cout << "Введите число " << i+1 << ": ";
		std::cin >> arr[i];
	}

	std::cout << "Массив до сортировки: ";
	Task3PrintArray(arr, ARR_SIZE);

	Task3SortArray(arr, ARR_SIZE);

	std::cout << "Массив после сортировки: ";
	Task3PrintArray(arr, ARR_SIZE);

	std::cout << std::endl << std::endl;
}

// Объявить структуру Сотрудник с различными полями.Сделайте для нее побайтовое выравнивание с помощью директивы pragma pack.
// Выделите динамически переменную этого типа.Инициализируйте ее.Выведите ее на экран и ее размер с помощью sizeof.Сохраните эту структуру в текстовый файл.

std::stringstream Tasks::Task4OutString(Employer* employer)
{
	std::stringstream ss;
	ss << "\tID       = " << employer->id << std::endl;
	ss << "\tИмя      = " << employer->name << std::endl;
	ss << "\tВозраст  = " << employer->age << std::endl;
	ss << "\tЗарплата = " << employer->salary << std::endl;
	return ss;
}

void Tasks::Task4()
{
	std::cout << "=================== Задача 4 ===================" << std::endl << std::endl;

	std::unique_ptr<Employer> employer = std::make_unique<Employer>();
	employer->id = 1;
	employer->name = "My Name";
	employer->age = 33;
	employer->salary = 1234.56;

	std::cout << "Размер структуры работника: " << sizeof(Employer) << " байта" << std::endl;

	std::cout << "Содержимое структуры работника: " << std::endl;
	std::cout << Task4OutString(employer.get()).str();

	std::fstream fout("emloyer.txt", std::ios::trunc | std::ios::out);
	fout << Task4OutString(employer.get()).str();
	fout.close();

	std::cout << std::endl << std::endl;
}

// Сделайте задание 1 добавив свой неймспейс во втором модуле(первое задание тогда можно не делать).
// Создайте проект из 2х cpp файлов и заголовочного(main.cpp, mylib.cpp, mylib.h) 
// во втором модуле mylib объявить 3 функции: для инициализации массива(типа float), 
// печати его на экран и подсчета количества отрицательных и положительных элементов.Вызывайте эти 3 - и функции из main для работы с массивом.

namespace nsTask5
{
	std::mt19937 Generator((unsigned int)(std::chrono::high_resolution_clock::now().time_since_epoch().count() ^ (size_t)(new char)));

	float GenerateRandomFloat(float m, float s)
	{
		std::normal_distribution<float> distr(m, s); 
		return distr(Generator);
	}

	void InitializeArray(float* var, size_t sz)
	{
		for (int i = 0; i < sz; i++) {
			var[i] = GenerateRandomFloat(0, 1);
		}
	}

	void PrintArray(float* var, size_t sz) 
	{
		for (int i = 0; i < sz; i++) {
			std::cout << var[i] << " ";
		}
		std::cout << std::endl;
	}
	
	void CalculateArray(float* var, size_t sz) 
	{
		int negnums = 0, notnegnums = 0;
		for (int i = 0; i < sz; i++) {
			var[i] < 0 ? negnums++ : notnegnums++;
		}

		std::cout << "Положительных элементов " << notnegnums << ", отрицательных " << negnums << std::endl;
	}
}

void Tasks::Task5()
{
	std::cout << "=================== Задача 5 ===================" << std::endl << std::endl;

	const int Size = 10;
	float arr[Size];

	std::cout << "Инициализируем массив случайными float числами на " << Size << " элементов." << std::endl;
	nsTask5::InitializeArray(arr, Size);

	std::cout << "Полученный массив: ";
	nsTask5::PrintArray(arr, Size);

	std::cout << "Подсчитываем положительные и отрицательные элементы массива: " << std::endl;
	nsTask5::CalculateArray(arr, Size);

	std::cout << std::endl << std::endl;
}
