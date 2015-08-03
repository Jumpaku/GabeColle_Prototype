#pragma once

#include<vector>
#include"Marking.h"
#include"Relation.h"
#include"Memory.h"

namespace gc {

template<class DerivedObject>
class Memory;

/**
*マークアンドスイープ法によるガベージコレクションでは,<br>
*1.ルートから参照を辿れるオブジェクトにマークを付ける<br>
*2.マークの付いていないオブジェクトを破棄する<br>
*という順番で不要なオブジェクトを破棄する<br>
*/
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
	/**
	*memoryのオブジェクトがルートから参照を辿れるか調べる
	*@param memory
	*@return 調べた結果
	*/
	static Marking mark(Memory_t const &memory)
	{
		Marking map(memory.size());
		mark(0, map, memory);
		return map;
	}

	/**
	*mapの情報を元にmemoryの不要なオブジェクトを破棄する
	*@param map
	*@param memory
	*/
	static void sweep(Marking const &map, Memory_t &memory)
	{
		for (Address_t address(1); address < memory.size(); ++address) {
			if (!map.isMarked(address)) {
				memory.free(address);
			}
		}
	}

	/**
	*memoryに対してガベージコレクションを実行する
	*@memory
	*/
	static void gc(Memory_t &memory)
	{
		sweep(mark(memory), memory);
	}
};

}
