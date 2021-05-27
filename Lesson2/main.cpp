#include <iostream>
#include <limits>

/*
В одном main.cpp файле / проекте:
● Создать и инициализировать переменные пройденных типов данных (short int, int, long long, char, bool, float, double).
● Создать перечисление (enum) с возможными вариантами символов для игры в крестики-нолики.
● Создать массив, способный содержать значения такого перечисления и инициализировать его.
● * Создать структуру (struct) данных «Поле для игры в крестики-нолики» и снабдить его всеми необходимыми свойствами (подумайте что может понадобиться).
● ** Создать структуру (struct) объединяющую: union (int, float, char) и 3-и битовых поля (флага) указывающими какого типа значение в данный момент содержится в объединении (isInt, isFloat, isChar). Продемонстрировать пример использования в коде этой структуры.
Для программирования используйте установленную среду программирования (IDE). Если задания 4 и 5 кажутся сложными постарайтесь сделать первые 3.
*/

/// <summary>
/// Задача 1
/// </summary>

typedef std::numeric_limits<float> flt;
typedef std::numeric_limits<double> dbl;

void Task1()
{
	std::cout << "================= Task 1 =================" << std::endl;

	short int si = 10;
	int i = 20;
	long long ll = 12345678987654321;
	char c = 78;
	bool b = true;
	float f = 3.141592653f;
	double d = 3.14159265358979323;

	std::cout << "short int=" << si << " int=" << i << " long long=" << ll << " char=" << c << " bool=" << std::boolalpha << b << " float=";
	std::cout.precision(flt::max_digits10);
	std::cout << f << " double=";
	std::cout.precision(dbl::max_digits10);
	std::cout << d << std::endl;

	std::cout << std::endl;
}

/// <summary>
/// Задача 2
/// </summary>

const unsigned int FIELD_SIZE = 3;

enum class TicTacToeCharsNum : int
{
	EMPTY,
	CROSS,
	ZERO,
	MAX
};

char TicTacToeChars[TicTacToeCharsNum::MAX] = { ' ','X','O' };

struct Player
{
	char name[255]{ 0 };
	TicTacToeCharsNum gameChar{ TicTacToeCharsNum::MAX };

	void Move(unsigned int x, unsigned int y, TicTacToeCharsNum *field[])
	{
		if (x > FIELD_SIZE || y > FIELD_SIZE) {
			std::cout << "Error! Move out of range!" << std::endl;
			return;
		}

		if (field[x][y] != TicTacToeCharsNum::EMPTY) {
			std::cout << "Sorry! You can not move here! Cell is busy!" << std::endl;
			return;
		}
		else {
			field[x][y] = gameChar;
		}
	}
};

struct TicTacToe
{
	TicTacToeCharsNum field[FIELD_SIZE][FIELD_SIZE]{ TicTacToeCharsNum::EMPTY };

	Player player1{};
	Player player2{};

	char NumToChar(TicTacToeCharsNum num)
	{
		return TicTacToeChars[(int)num];
	}

	TicTacToeCharsNum CharToNum(char ch)
	{
		switch (ch) {
			case ' ': return TicTacToeCharsNum::EMPTY;
			case 'X': return TicTacToeCharsNum::CROSS;
			case 'O': return TicTacToeCharsNum::ZERO;
		}

		return TicTacToeCharsNum::MAX;
	}
};

void Task2()
{
	std::cout << "================= Task 2 =================" << std::endl;

	TicTacToe game;
	game.player1 = { "Player1", TicTacToeCharsNum::CROSS };
	game.player2 = { "Player2", TicTacToeCharsNum::ZERO };

	std::cout << std::endl;
}

/// <summary>
/// Задача 3
/// </summary>

struct Variants
{
	union VariantsType 
	{
		int i;
		float f;
		char c;
	};

	VariantsType var;
	char isInt : 1;
	char isFloat : 1;
	char isChar : 1;

	void operator =(int p) { var.i = p; isInt = 1; isFloat = 0; isChar = 0; }
	void operator =(float p) { var.f = p; isInt = 0; isFloat = 1; isChar = 0; }
	void operator =(char p) { var.c = p; isInt = 0; isFloat = 0; isChar = 1; }

	void PrintState() 
	{
		if (isInt) {
			std::cout << "Variable is int = " << var.i << std::endl;
			return;
		}
		if (isFloat) {
			std::cout << "Variable is float = " << var.f << std::endl;
			return;
		}
		if (isChar) {
			std::cout << "Variable is char = " << var.c << std::endl;
			return;
		}
	}
};

void Task3()
{
	std::cout << "================= Task 3 =================" << std::endl;

	Variants testVar;

	testVar = 10;
	testVar.PrintState();

	testVar = 3.1415f;
	testVar.PrintState();

	testVar = 'g';
	testVar.PrintState();

	std::cout << std::endl;
}

int main()
{
	Task1();
	Task2();
	Task3();
	
	return 0;
}

