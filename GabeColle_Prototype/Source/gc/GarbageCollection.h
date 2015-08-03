#pragma once

#include<vector>
#include"Marking.h"
#include"Relation.h"
#include"Memory.h"


template<class DerivedObject>
class Memory;


template<class DeriverdObject>
class GarbageCollection
{
public:
	typedef Memory<DeriverdObject> Memory_t;
	typedef int Address_t;
private:
	static void mark(Address_t address, Marking &map, Memory_t const &memory)
	{
		for (Address_t i(0); i < memory.size(); ++i) {
			if (!map.isMarked(i) && memory.getRelation().areLinked(address, i)) {
				map.mark(i);
				mark(i, map, memory);
			}
		}
	}
public:
	static Marking mark(Memory_t const &memory)
	{
		Marking map(memory.size());
		mark(0, map, memory);
		return map;
	}
	static void sweep(Marking const &map, Memory_t &memory)
	{
		for (Address_t address(1); address < memory.size(); ++address) {
			if (!map.isMarked(address)) {
				memory.free(address);
			}
		}
	}
	static void gc(Memory_t &memory)
	{
		sweep(mark(memory), memory);
	}
};


