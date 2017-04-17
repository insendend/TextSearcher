#pragma once
#include <string>
#include <vector>

namespace FileWork
{
	const int SIZE_BLOCK = 4096;
	
	// инициализация настроек из файла
	void LoadFromFile(std::string &, std::string &);

	// сохранение в файл
	void SaveToFile(const std::string &, const std::string &);

	// поиск и сохранение результатов в файл "find.txt"
	void Search(std::string &, std::vector < std::string > &, std::vector < std::string > &);
}

namespace Menu
{
	// пункты меню
	enum Choice { Auto = 1, ByHand, Exit };

	inline void TextMenu(void)
	{
		std::cout << "1. Auto (from files in project-folder)" << std::endl;
		std::cout << "2. By Hand (to enter path, masks, words)" << std::endl;
		std::cout << "3. Exit" << std::endl << "Your choice -> ";
	}

	// выбор пункта меню
	Choice Menu(void);
}

namespace Input
{
	// инициализация настроек пользователем с клавиатуры
	void UserInput(std::string &, std::string &, std::string &);

	// инициализация всех элементов поиска
	void InitAll(Menu::Choice &, std::string &, std::string &, std::string &);
}

namespace Edit
{
	// добавление маски поиска к пути
	std::string AddMask(const std::string &);

	// разделение буфера на ключевые слова
	void Tokenning(std::string &, std::vector < std::string > &);
}