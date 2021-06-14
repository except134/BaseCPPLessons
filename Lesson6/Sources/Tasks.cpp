#include "Lesson6PCH.h"

namespace fs = std::filesystem;

std::mt19937 Generator(std::chrono::high_resolution_clock::now().time_since_epoch().count() ^ (size_t)(new char));

int GenerateRandomInt(int a, int b) 
{
	return Generator() % (b - a + 1) + a;
}

char GenerateRandomChar(char a, char b)
{
	return Generator() % (b - a + 1) + a;
}

// 1. Выделить в памяти динамический одномерный массив типа int.Размер массива запросить у пользователя.Инициализировать его числами степенями двойки : 1, 2, 4, 8, 16, 32, 64, 128 …
// Вывести массив на экран.Не забыть освободить память. = ) Разбить программу на функции.
void Tasks::Task1()
{
	std::cout << "=================== Задача 1 ===================" << std::endl << std::endl;

	int num{};
	std::cout << "Введите размер массива: ";
	std::cin >> num;

	std::unique_ptr<int[]> myArray = std::make_unique<int[]>(num);

	myArray[0] = 1;
	for (int i = 1, startnum = 2; i < num; i++, startnum*=2) {
		myArray[i] = startnum;
	}

	for (int i = 0; i < num; i++) {
		std::cout << myArray[i] << " ";
	}

	std::cout << std::endl << std::endl;
}

// 2. Динамически выделить матрицу 4х4 типа int.Инициализировать ее псевдослучайными числами через функцию rand.Вывести на экран.
// Разбейте вашу программу на функции которые вызываются из main.

void Tasks::Task2()
{
	std::cout << "=================== Задача 2 ===================" << std::endl << std::endl;

	int** matrix = new int*[4];
	for (int i = 0; i < 4; i++) {
		matrix[i] = new int[4];
	}

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = GenerateRandomInt(100, 200);

			std::cout << matrix[i][j] << " ";
		}

		std::cout << std::endl;
	}

	for (int i = 0; i < 4; i++) {
		delete [] matrix[i];
	}
	delete []matrix;

	std::cout << std::endl << std::endl;
}

// 3. Написать программу c 2 - я функциями, которая создаст два текстовых файла(*.txt), примерно по 50 - 100 символов в каждом(особого значения не имеет с каким именно содержимым).
// Имена файлов запросить у польлзователя.

void Tasks::Tasks3CreateFile1(const std::string& fname)
{
	std::fstream str(fname, std::ios::out | std::ios::trunc);

	for (int i = 0; i < 50; i++) {
		str << GenerateRandomChar('A', 'z');
	}

	str.close();
}

void Tasks::Tasks3CreateFile2(const std::string& fname)
{
	std::fstream str(fname, std::ios::out | std::ios::trunc);

	for (int i = 0; i < 100; i++) {
		str << GenerateRandomChar('A', 'z');
	}

	str.close();
}

void Tasks::Task3()
{
	std::cout << "=================== Задача 3 ===================" << std::endl << std::endl;

	std::cout << "Введите имя первого файла: ";
	std::string inf1;
	std::cin >> inf1;
	Tasks3CreateFile1(inf1);

	std::cout << "Введите имя второго файла: ";
	std::string inf2;
	std::cin >> inf2;
	Tasks3CreateFile2(inf2);

	std::cout << std::endl << std::endl;
}

//4. Написать функцию, «склеивающую» эти файлы в третий текстовый файл(имя файлов спросить у пользователя).

void Tasks::Task4FileGlue(const std::string& fname1, const std::string& fname2)
{
	std::string resultFileName = GetFileNameNoExtensionFromFilePath(fname1) + GetFileNameFromFilePath(fname2);

	std::ifstream fin1(fname1, std::ios::binary);
	std::ifstream fin2(fname2, std::ios::binary);
	std::ofstream fout(resultFileName, std::ios::binary | std::ios::out | std::ios::trunc);

	std::ostreambuf_iterator<char> output(fout);

	std::copy((std::istreambuf_iterator<char>(fin1)), std::istreambuf_iterator<char>(), output);
	std::copy((std::istreambuf_iterator<char>(fin2)), std::istreambuf_iterator<char>(), output);

	fin1.close();
	fin2.close();
	fout.close();

	std::cout << "Склеивание файлов завершено. Резултатирующий файл: " << resultFileName << std::endl;
}

std::string Tasks::GetFileNameFromFilePath(std::string_view strPath)
{
	return fs::path(strPath).filename().generic_string();
}

std::string Tasks::GetFileNameNoExtensionFromFilePath(std::string_view filepath)
{
	auto fileName = GetFileNameFromFilePath(filepath);
	auto lastindex = fileName.find_last_of('.');
	auto fileNameNoExt = fileName.substr(0, lastindex);

	return fileNameNoExt;
}

bool Tasks::FileExists(std::string_view strFile)
{
	try
	{
		return fs::exists(strFile);
	}
	catch (fs::filesystem_error& e)
	{
		std::cout << "Ошибка!!! Файл " << strFile.data() << " не существует.";
		return false;
	}
}

void Tasks::Task4()
{
	std::cout << "=================== Задача 4 ===================" << std::endl << std::endl;

	std::cout << "Введите имя первого файла: ";
	std::string inf1;
	std::cin >> inf1;
	if (!FileExists(inf1))
		return;

	std::cout << "Введите имя второго файла: ";
	std::string inf2;
	std::cin >> inf2;
	if (!FileExists(inf2))
		return;

	Task4FileGlue(inf1, inf2);

	std::cout << std::endl << std::endl;
}

// 5. Написать программу, которая проверяет присутствует ли указанное пользователем при запуске программы слово в указанном пользователем файле(для простоты работаем только с латиницей).

void Tasks::Task5(const std::string& argv)
{
	std::cout << "=================== Задача 5 ===================" << std::endl << std::endl;

	std::string searchStr = argv;

	if (searchStr.empty()) {
		std::cout << "Введите искомое слово: ";
		std::cin >> searchStr;
	}

	std::string searchFile{};
	std::cout << "Введите имя файла, в котором будет осуществляться поиск: ";
	std::cin >> searchFile;

	std::fstream file(searchFile);

	std::string nextToken;
	bool result = false;
	while (file >> nextToken) {
		if (nextToken == searchStr) {
			result = true;
			break;
		}
	}

	std::cout << "Слово " << searchStr << " в файле " << searchFile;
	
	if(result)
		std::cout << " найдено" << std::endl;
	else
		std::cout << " не найдено" << std::endl;


	std::cout << std::endl << std::endl;
}
