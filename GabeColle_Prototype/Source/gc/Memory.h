#pragma once

#include <Siv3D.hpp>
#include"GarbageCollection.h"
#include"Object.h"
#include"Error.h"

namespace gc {

template<class DeriverdObject>
class GarbageCollection;

/**
*���z�I�ȃ�����
*/
template<class DerivedObject>
class Memory
{
public:
	/**�Ԓn�̌^(int)*/
	typedef int Address_t;
private:
	std::vector<DerivedObject> memory_m;
	Relation relation_m;
	Error error_m;
private:
	void check(Address_t address)
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
public:
	/**
	*�R���X�g���N�^
	*@param size �����ł���ő�̃I�u�W�F�N�g��
	*/
	Memory(int size) : relation_m(size)
	{
		memory_m = std::move(std::vector<DerivedObject>(size));
		memory_m[0].construct();
	}

	/**
	*�I�u�W�F�N�g��1��������
	*@return ���������I�u�W�F�N�g�̔Ԓn
	*/
	Address_t alloc()
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

	/**
	*address���w�肷��Ԓn�̃I�u�W�F�N�g��j������
	@param address �j������I�u�W�F�N�g�̔Ԓn
	*/
	void free(Address_t address)
	{
		Marking const map = mark();
		if (address != 0) {
			for (Address_t i(0); i < size(); ++i) {
				unlink(address, i);
			}
			for (Address_t i(1); i < size(); ++i) {
				if (!map.isMarked(i)) {
					unlink(i, address);
				}
			}
			memory_m[address].destruct();
		}
	}

	/**
	*�Ԓnfrom�̃I�u�W�F�N�g�ƔԒnto�̃I�u�W�F�N�g���q��
	*@param from
	*@param to
	*/
	void link(Address_t from, Address_t to)
	{
		if (!hasExpired(from) && !hasExpired(to)) {
			relation_m.link(from, to);
		}
	}

	/**
	*�Ԓnfrom�̃I�u�W�F�N�g�ƔԒnto�̃I�u�W�F�N�g��؂藣��
	*@param from
	*@param to
	*/
	void unlink(Address_t from, int to)
	{
		relation_m.unlink(from, to);
	}

	/**
	*�I�u�W�F�N�g�����[�g����Q�Ƃ�H��邩�ǂ����𒲂ׂ�
	*@return ���ׂ�����
	*@see Marking
	*/
	Marking mark()const
	{
		return GarbageCollection<CircleObject>::mark(*this);
	}

	/**
	*���[�g����H��Ȃ��I�u�W�F�N�g��j������
	*/
	void gc()
	{
		GarbageCollection<CircleObject>::gc(*this);
	}

	/**
	*address�̃I�u�W�F�N�g���j������Ă��邩���ׂ�
	*@return �j������Ă���ꍇ�^
	*/
	bool hasExpired(Address_t address)
	{
		return !memory_m[address].exists();
	}

	/**
	*address�̃I�u�W�F�N�g���Q�Ƃ���
	*@param address
	*@return �I�u�W�F�N�g�̎Q��
	*/
	DerivedObject &access(Address_t address)
	{
		check(address);
		assert(0 < address && address < size());
		return memory_m[address];
	}

	/**
	*@return ���݂̃I�u�W�F�N�g���m�̂̊֌W
	*@see Relation
	*/
	Relation const &getRelation()const
	{
		return relation_m;
	}

	/**
	*@return �����ł���ő�̃I�u�W�F�N�g��
	*/
	int size()const
	{
		return memory_m.size();
	}

	/**
	*@return ���܂łɔ��������G���[
	*@see Error
	*/
	Error error()const
	{
		return error_m;
	}

	/**
	*@return ���[�g�I�u�W�F�N�g�̎Q��
	*/
	DerivedObject &root()
	{
		return memory_m[0];
	}
};

}