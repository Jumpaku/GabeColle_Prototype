#include"Relation.h"

using namespace gc;

Relation::Relation(Address_t size)
{
	relation_m = std::move(std::vector<std::vector<bool>>(size, std::vector<bool>(size, false)));
}
void Relation::link(Address_t from, Address_t to)
{
	relation_m[from][to] = true;
}
void Relation::unlink(Address_t from, int to)
{
	relation_m[from][to] = false;
}
bool Relation::areLinked(Address_t from, Address_t to)const
{
	return relation_m[from][to];
}
