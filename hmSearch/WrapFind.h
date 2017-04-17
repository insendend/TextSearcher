#pragma once
#include <io.h>

class Wrap
{
	intptr_t handle;

public:
	// конструктор
	Wrap(intptr_t handle = -1) throw()
	{
		this->handle = handle;
	}

	// деструктор
	virtual ~Wrap(void)	throw()
	{
		if (this->handle != -1)
			_findclose(handle);
	}

	// проверка открытия поиска
	bool operator !(void) const throw()
	{
		return this->handle == -1;
	}

	// возврат handle
	operator intptr_t (void) const throw()
	{
		return this->handle;
	}
};