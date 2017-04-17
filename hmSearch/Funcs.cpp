#include <iostream>
#include <fstream>
#include <memory>
#include "WrapFind.h"
#include "Funcs.h"

// выбор пункта меню
Menu::Choice Menu::Menu(void)
{
	Choice choice;
	std::string buf;

	do
	{
		TextMenu();

		getline(std::cin, buf);
		system("cls");

		try
		{
			choice = static_cast <Menu::Choice>(stoi(buf));
		}

		// исключение stoi()
		catch (const std::invalid_argument &ia)
		{
			std::cout << ia.what() << std::endl;
		}

	} while (choice > Choice::Exit || choice < Choice::Auto);

	return choice;	// возвращаем пункт меню
}

// инициализация настроек из файла
void FileWork::LoadFromFile(std::string &from, std::string &buf)
{
	buf.resize(FileWork::SIZE_BLOCK);

	// открытие файла
	std::ifstream ifs(from);

	if (!ifs.is_open())
		throw std::exception("Can't open file");

	// запись содержимого в буфер
	while (!ifs.eof())
	{
		ifs.read(&buf[0], buf.size());

		size_t sizeRead = static_cast <size_t> (ifs.gcount());

		if (!sizeRead)
			throw std::exception("file is empty");

		buf.resize(sizeRead);
	}
}

// поиск и сохранение результатов в файл "find.txt"
void FileWork::Search(std::string &path, std::vector < std::string > &masks, std::vector < std::string > &words)
{
	_finddata_t fd = { 0 };
	std::unique_ptr<Wrap> up;
	up.reset(new Wrap(_findfirst(&Edit::AddMask(path)[0], &fd)));

	// проверка
	if (!*up)
	{
		std::cout << "Access denied in " << path << std::endl;
		throw std::exception("Path is incorrect");
	}

	do
	{
		std::string name(std::string(fd.name));
		if (name == std::string(".") || name == std::string("..") || fd.attrib & _A_SYSTEM)
			continue;

		// формируем полный путь найденного файла/каталога
		std::string fullName(path);
		fullName += (path.size() && path[path.length() - 1] == '\\') ?
			name :
			std::string("\\") + name;

		// если директория - ищем дальше рекурсивно
		if (fd.attrib & _A_SUBDIR)
		{
			try
			{
				Search(fullName, masks, words);
			}
			catch (std::exception &) {}
		}

		// если файл
		else
		{
			// для каждой маски поиска
			for (auto &i : masks)
			{
				// проверяем ее наличие в имени файла
				if (fullName.find(i) != std::string::npos)
				{
					// сохраняем содержимое файла в буфер
					std::string tmp;
					LoadFromFile(fullName, tmp);

					// для каждого слова поиска
					for (auto &i : words)

						// проверяем его наличие буфере
						if (tmp.find(i) != std::string::npos)

							// сохраняем в файл "find.txt"
							SaveToFile(i, fullName);
				}
			}
		}
	} while (!_findnext(*up, &fd));
}

// сохранение в файл
void FileWork::SaveToFile(const std::string &obj, const std::string &from)
{
	// создание файла в директории с проэктом
	std::ofstream ofs(std::string("find.txt"), std::ios::app);

	if (!ofs.is_open())
		throw std::exception("Error creating file \"find.txt\"");

	// запись в файл
	ofs << "Found \"" << obj << "\" in " << from << std::endl;
}

// инициализация настроек пользователем с клавиатуры
void Input::UserInput(std::string &path, std::string &mask, std::string &word)
{
	// ввод пути поиска
	std::cout << "Enter a path to dir: ";
	std::getline(std::cin, path);
	if (path[path.length() - 1] != '\\')
		path += '\\';

	// проверка на корректность пути
	struct stat st;
	if (stat(&path[0], &st) == -1)
		throw std::exception("path is incorrect");

	// ввод масок поиска
	std::cout << "Enter a masks of search: ";
	std::getline(std::cin, mask);

	// ввод слов поиска
	std::cout << "Enter a words for search: ";
	std::getline(std::cin, word);
}

// инициализация всех элементов поиска
void Input::InitAll(Menu::Choice &choice, std::string &path, std::string &mask, std::string &word)
{
	switch (choice)
	{

	// автоматический выбор элементов поиска из файлов
	case Menu::Auto:
	{
		// считывание содержимое из файлов в буферы
		FileWork::LoadFromFile(std::string("masks.txt"), mask);
		FileWork::LoadFromFile(std::string("words.txt"), word);
		FileWork::LoadFromFile(std::string("path.txt"), path);

		break;
	}

	// пользовательский ввод элементов поиска
	case Menu::Manual:
	{
		// пользовательский ввод с клавиатуры
		UserInput(path, mask, word);

		break;
	}

	// пункт меню Exit
	default:
		exit(0);
	}
}

// добавление маски поиска к пути
std::string Edit::AddMask(const std::string &str)
{
	// если строка с путём пустая
	if (!str.size())
		return std::string("C:\\*.*");

	if (str[str.length() - 1] == '\\')
		return str + std::string("*.*");
	else
		return str + std::string("\\*.*");
}

// разделение буфера на ключевые слова
void Edit::Tokenning(std::string &strBuf, std::vector < std::string > &v)
{
	// разделители слов
	const std::string leksemes = std::string("* ,;\r\n\"");

	// позиции начала и конца первой слова
	int posStart = strBuf.find_first_not_of(leksemes);
	int posEnd = strBuf.find_first_of(leksemes, posStart);

	// если найдена лексема, записываем и ищем дальше
	while (posStart != std::string::npos)
	{
		// запись значений между найденными позициями начала и конца 
		v.push_back(strBuf.substr(posStart, posEnd - posStart));

		// ищем позициии следующее слова
		posStart = strBuf.find_first_not_of(leksemes, posEnd);
		posEnd = strBuf.find_first_of(leksemes, posStart);
	}

	// форматируем слова для корректного поиска
	for (auto &i : v)
		i.resize(strlen(i.c_str()));
}