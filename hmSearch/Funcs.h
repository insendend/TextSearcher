#pragma once
#include <string>
#include <vector>

namespace FileWork
{
	const int SIZE_BLOCK = 4096;
	
	// ������������� �������� �� �����
	void LoadFromFile(std::string &, std::string &);

	// ���������� � ����
	void SaveToFile(const std::string &, const std::string &);

	// ����� � ���������� ����������� � ���� "find.txt"
	void Search(std::string &, std::vector < std::string > &, std::vector < std::string > &);
}

namespace Menu
{
	// ������ ����
	enum Choice { Auto = 1, ByHand, Exit };

	inline void TextMenu(void)
	{
		std::cout << "1. Auto (from files in project-folder)" << std::endl;
		std::cout << "2. By Hand (to enter path, masks, words)" << std::endl;
		std::cout << "3. Exit" << std::endl << "Your choice -> ";
	}

	// ����� ������ ����
	Choice Menu(void);
}

namespace Input
{
	// ������������� �������� ������������� � ����������
	void UserInput(std::string &, std::string &, std::string &);

	// ������������� ���� ��������� ������
	void InitAll(Menu::Choice &, std::string &, std::string &, std::string &);
}

namespace Edit
{
	// ���������� ����� ������ � ����
	std::string AddMask(const std::string &);

	// ���������� ������ �� �������� �����
	void Tokenning(std::string &, std::vector < std::string > &);
}