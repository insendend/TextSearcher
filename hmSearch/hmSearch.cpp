#include <iostream>
#include <ctime>
#include "Funcs.h"

int main()
{
	try
	{
		// ����� ������ ����
		Menu::Choice choice = Menu::Menu();

		std::string pathFind;	// ���� ������
		std::string maskBuf;	// ����� ������
		std::string wordBuf;	// ����� ������

		// ������������� ���� ��������� ������
		Input::InitAll(choice, pathFind, maskBuf, wordBuf);

		// ���������� ����� � ���� ������ � ���������� � ������
		std::vector < std::string > vecMask, vecWord;
		Edit::Tokenning(maskBuf, vecMask);
		Edit::Tokenning(wordBuf, vecWord);

		time_t tStart;
		time(&tStart);

		// ����� � ���������� ����������� � ���� "find.txt"
		FileWork::Search(pathFind, vecMask, vecWord);

		time_t tEnd;
		time(&tEnd);

		// ����� �� ����� ����� ������ � ��������
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