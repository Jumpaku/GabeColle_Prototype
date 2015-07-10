#include "Memory.h"

#pragma once

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


Vec2 CircleObject::center()const
{
	return Vec2(x_m, y_m);
}
void CircleObject::center(Vec2 v)
{
	x(v.x);
	y(v.y);
}
void CircleObject::x(double x)
{
	x_m = x;
}
void CircleObject::y(double y)
{
	y_m = y;
}


Relation::Relation(Address_t size)
{
	relation_m = std::move(std::vector<std::vector<bool>>(size + 1, std::vector<bool>(size + 1, false)));
}
void Relation::link(Address_t from, Address_t to)
{
	relation_m[from][to] = true;
}
void Relation::unlink(Address_t from, int to)
{
	relation_m[from][to] = false;
}
bool Relation::isUsing(Address_t from, Address_t to)const
{
	return relation_m[from][to];
}


MarkingMap::MarkingMap(int size)
	:map_m(size, false){}
void MarkingMap::mark(Address_t address)
{
	map_m.at(address) = true;
}
void MarkingMap::unmark(Address_t address)
{
	map_m.at(address) = false;
}
bool MarkingMap::isMarked(Address_t address)const
{
	return map_m.at(address);
}


void GarbageCollection::mark(Address_t address, MarkingMap &map, Memory const &memory)
{
	for (Address_t i(0); i < memory.size(); ++i) {
		if (!map.isMarked(i) && memory.getRelation().isUsing(address, i)) {
			map.mark(i);
			mark(i, map, memory);
		}
	}
}
MarkingMap GarbageCollection::mark(Memory const &memory)
{
	MarkingMap map(memory.size());
	mark(0, map, memory);
	return map;
}
void GarbageCollection::sweep(MarkingMap const &map, Memory &memory)
{
	for (Address_t address(1); address < memory.size(); ++address) {
		if (!map.isMarked(address)) {
			memory.free(address);
		}
	}
}
void GarbageCollection::gc(Memory &memory)
{
	sweep(mark(memory), memory);
}


void Memory::check(Address_t address)
{
	if (hasExpired(address)) {
		++error_m.segmentationFault_m;
	}
	else if (address == 0) {
		++error_m.nullptrAccess_m;
	}
	else if (address <= -1 || size() <= address) {
		++error_m.addressOutOfBounds_m;
	}
}
Memory::Memory(int size) : relation_m(size)
{
	memory_m = std::move(std::vector<CircleObject>(size));
	memory_m[0].construct();
}
Memory::Address_t Memory::alloc()
{
	for (Address_t address(1); address < size(); ++address) {
		if (!memory_m[address].exists()) {
			memory_m[address].construct();
			return address;
		}
	}
	++error_m.outOfMemory_m;
	return 0;
}
void Memory::free(Address_t address)
{
	MarkingMap const map = mark();
	if (address != 0) {
		for (Address_t i(0); i < size(); ++i) {
			unlink(address, i);
		}
		for (Address_t i(1); i < size(); ++i) {
			if (!map.isMarked(i))				{
				unlink(i, address);
			}
		}
		memory_m[address].destruct();
	}
}
void Memory::link(Address_t from, Address_t to)
{
	if (!hasExpired(from) && !hasExpired(to)) {
		relation_m.link(from, to);
	}
}
void Memory::unlink(Address_t from, int to)
{
	relation_m.unlink(from, to);
}
MarkingMap Memory::mark()const
{
	return GarbageCollection::mark(*this);
}
void Memory::gc()
{
	GarbageCollection::gc(*this);
}
bool Memory::hasExpired(Address_t address)
{
	return !memory_m[address].exists();
}
CircleObject &Memory::access(Address_t address)
{
	check(address);
	assert(address != 0);
	return memory_m[address];
}
Relation const &Memory::getRelation()const
{
	return relation_m;
}
int Memory::size()const
{
	return memory_m.size();
}
Error Memory::error()const
{
	return error_m;
}
CircleObject &Memory::root()
{
	return memory_m[0];
}

