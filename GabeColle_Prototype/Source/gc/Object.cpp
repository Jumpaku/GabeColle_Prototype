
#include "Object.h"

# include <Siv3D.hpp>


bool Object::exists()const
{
	return exists_m;
}
void Object::construct()
{
	exists_m = true;
}
void Object::destruct()
{
	exists_m = false;
}
