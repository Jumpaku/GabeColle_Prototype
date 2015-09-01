#include"Marking.h"

using namespace gc;

Marking::Marking(int size)
	:map_m(size, false)
{}
void Marking::mark(Address_t address)
{
	map_m.at(address) = true;
}
void Marking::unmark(Address_t address)
{
	map_m.at(address) = false;
}
bool Marking::isMarked(Address_t address)const
{
	return map_m.at(address);
}

