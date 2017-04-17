#include <iostream>
#include <fstream>
#include <memory>
#include "WrapFind.h"
#include "Funcs.h"

// ����� ������ ����
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

		// ���������� stoi()
		catch (const std::invalid_argument &ia)
		{
			std::cout << ia.what() << std::endl;
		}

	} while (choice > Choice::Exit || choice < Choice::Auto);

	return choice;	// ���������� ����� ����
}

// ������������� �������� �� �����
void FileWork::LoadFromFile(std::string &from, std::string &buf)
{
	buf.resize(FileWork::SIZE_BLOCK);

	// �������� �����
	std::ifstream ifs(from);

	if (!ifs.is_open())
		throw std::exception("Can't open file");

	// ������ ����������� � �����
	while (!ifs.eof())
	{
		ifs.read(&buf[0], buf.size());

		size_t sizeRead = static_cast <size_t> (ifs.gcount());

		if (!sizeRead)
			throw std::exception("file is empty");

		buf.resize(sizeRead);
	}
}

// ����� � ���������� ����������� � ���� "find.txt"
void FileWork::Search(std::string &path, std::vector < std::string > &masks, std::vector < std::string > &words)
{
	_finddata_t fd = { 0 };
	std::unique_ptr<Wrap> up;
	up.reset(new Wrap(_findfirst(&Edit::AddMask(path)[0], &fd)));

	// ��������
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

		// ��������� ������ ���� ���������� �����/��������
		std::string fullName(path);
		fullName += (path.size() && path[path.length() - 1] == '\\') ?
			name :
			std::string("\\") + name;

		// ���� ���������� - ���� ������ ����������
		if (fd.attrib & _A_SUBDIR)
		{
			try
			{
				Search(fullName, masks, words);
			}
			catch (std::exception &) {}
		}

		// ���� ����
		else
		{
			// ��� ������ ����� ������
			for (auto &i : masks)
			{
				// ��������� �� ������� � ����� �����
				if (fullName.find(i) != std::string::npos)
				{
					// ��������� ���������� ����� � �����
					std::string tmp;
					LoadFromFile(fullName, tmp);

					// ��� ������� ����� ������
					for (auto &i : words)

						// ��������� ��� ������� ������
						if (tmp.find(i) != std::string::npos)

							// ��������� � ���� "find.txt"
							SaveToFile(i, fullName);
				}
			}
		}
	} while (!_findnext(*up, &fd));
}

// ���������� � ����
void FileWork::SaveToFile(const std::string &obj, const std::string &from)
{
	// �������� ����� � ���������� � ��������
	std::ofstream ofs(std::string("find.txt"), std::ios::app);

	if (!ofs.is_open())
		throw std::exception("Error creating file \"find.txt\"");

	// ������ � ����
	ofs << "Found \"" << obj << "\" in " << from << std::endl;
}

// ������������� �������� ������������� � ����������
void Input::UserInput(std::string &path, std::string &mask, std::string &word)
{
	// ���� ���� ������
	std::cout << "Enter a path to dir: ";
	std::getline(std::cin, path);
	if (path[path.length() - 1] != '\\')
		path += '\\';

	// �������� �� ������������ ����
	struct stat st;
	if (stat(&path[0], &st) == -1)
		throw std::exception("path is incorrect");

	// ���� ����� ������
	std::cout << "Enter a masks of search: ";
	std::getline(std::cin, mask);

	// ���� ���� ������
	std::cout << "Enter a words for search: ";
	std::getline(std::cin, word);
}

// ������������� ���� ��������� ������
void Input::InitAll(Menu::Choice &choice, std::string &path, std::string &mask, std::string &word)
{
	switch (choice)
	{

	// �������������� ����� ��������� ������ �� ������
	case Menu::Auto:
	{
		// ���������� ���������� �� ������ � ������
		FileWork::LoadFromFile(std::string("masks.txt"), mask);
		FileWork::LoadFromFile(std::string("words.txt"), word);
		FileWork::LoadFromFile(std::string("path.txt"), path);

		break;
	}

	// ���������������� ���� ��������� ������
	case Menu::Manual:
	{
		// ���������������� ���� � ����������
		UserInput(path, mask, word);

		break;
	}

	// ����� ���� Exit
	default:
		exit(0);
	}
}

// ���������� ����� ������ � ����
std::string Edit::AddMask(const std::string &str)
{
	// ���� ������ � ���� ������
	if (!str.size())
		return std::string("C:\\*.*");

	if (str[str.length() - 1] == '\\')
		return str + std::string("*.*");
	else
		return str + std::string("\\*.*");
}

// ���������� ������ �� �������� �����
void Edit::Tokenning(std::string &strBuf, std::vector < std::string > &v)
{
	// ����������� ����
	const std::string leksemes = std::string("* ,;\r\n\"");

	// ������� ������ � ����� ������ �����
	int posStart = strBuf.find_first_not_of(leksemes);
	int posEnd = strBuf.find_first_of(leksemes, posStart);

	// ���� ������� �������, ���������� � ���� ������
	while (posStart != std::string::npos)
	{
		// ������ �������� ����� ���������� ��������� ������ � ����� 
		v.push_back(strBuf.substr(posStart, posEnd - posStart));

		// ���� �������� ��������� �����
		posStart = strBuf.find_first_not_of(leksemes, posEnd);
		posEnd = strBuf.find_first_of(leksemes, posStart);
	}

	// ����������� ����� ��� ����������� ������
	for (auto &i : v)
		i.resize(strlen(i.c_str()));
}