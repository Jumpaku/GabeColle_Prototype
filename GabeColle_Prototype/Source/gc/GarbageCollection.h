#pragma once

#include<vector>
#include"Marking.h"
#include"Relation.h"
#include"Memory.h"

namespace gc {

template<class DerivedObject>
class Memory;

/**
*�}�[�N�A���h�X�C�[�v�@�ɂ��K�x�[�W�R���N�V�����ł�,<br>
*1.���[�g����Q�Ƃ�H���I�u�W�F�N�g�Ƀ}�[�N��t����<br>
*2.�}�[�N�̕t���Ă��Ȃ��I�u�W�F�N�g��j������<br>
*�Ƃ������Ԃŕs�v�ȃI�u�W�F�N�g��j������<br>
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
	*memory�̃I�u�W�F�N�g�����[�g����Q�Ƃ�H��邩���ׂ�
	*@param memory
	*@return ���ׂ�����
	*/
	static Marking mark(Memory_t const &memory)
	{
		Marking map(memory.size());
		mark(0, map, memory);
		return map;
	}

	/**
	*map�̏�������memory�̕s�v�ȃI�u�W�F�N�g��j������
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
	*memory�ɑ΂��ăK�x�[�W�R���N�V���������s����
	*@memory
	*/
	static void gc(Memory_t &memory)
	{
		sweep(mark(memory), memory);
	}
};

}
