#include <iostream>
#include <ctime>
#include "Funcs.h"

int main()
{
	try
	{
		// выбор пункта меню
		Menu::Choice choice = Menu::Menu();

		std::string pathFind;	// путь поиска
		std::string maskBuf;	// маски поиска
		std::string wordBuf;	// слова поиска

		// инициализация всех элементов поиска
		Input::InitAll(choice, pathFind, maskBuf, wordBuf);

		// разделение масок и слов поиска и сохранение в вектор
		std::vector < std::string > vecMask, vecWord;
		Edit::Tokenning(maskBuf, vecMask);
		Edit::Tokenning(wordBuf, vecWord);

		time_t tStart;
		time(&tStart);

		// поиск и сохранение результатов в файл "find.txt"
		FileWork::Search(pathFind, vecMask, vecWord);

		time_t tEnd;
		time(&tEnd);

		// вывод на экран время поиска в секундах
		std::cout << std::endl << "Time of searching: " << tEnd - tStart << " sec." << std::endl;

	}
	catch (std::exception &ex)
	{
		std::cout << ex.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "Unknown exception" << std::endl;
	}

	return 0;
}