#include "Obj.h"
#include "string.h"

namespace lyd
{
	Obj::Obj()
	{
	}

	Obj::~Obj()
	{
	}

	void Obj::setName(const char* name)
	{
		strncpy(_name, name, sizeof(_name));
	}

	char* Obj::getName()
	{
		char* go = new char[64];
		strncpy(go, _name, 64);
		return go;
	}
}