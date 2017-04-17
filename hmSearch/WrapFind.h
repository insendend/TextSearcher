#pragma once
#include <io.h>

class Wrap
{
	intptr_t handle;

public:
	// �����������
	Wrap(intptr_t handle = -1) throw()
	{
		this->handle = handle;
	}

	// ����������
	virtual ~Wrap(void)	throw()
	{
		if (this->handle != -1)
			_findclose(handle);
	}

	// �������� �������� ������
	bool operator !(void) const throw()
	{
		return this->handle == -1;
	}

	// ������� handle
	operator intptr_t (void) const throw()
	{
		return this->handle;
	}
};