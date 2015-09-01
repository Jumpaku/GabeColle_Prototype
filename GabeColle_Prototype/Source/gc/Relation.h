#pragma once

#include<vector>

namespace gc {

/**
*�����Ɋ֌W�s��������Ă��ăI�u�W�F�N�̎Q�Ɗ֌W���Ǘ�����
*/
class Relation
{
public:
	typedef std::vector<std::vector<bool>> RelationMatrix_t;
	typedef int Address_t;
private:
	RelationMatrix_t relation_m;
public:
	/**
	*k�R���X�g���N�^
	*@param size �������̍ő�̃I�u�W�F�N�g��
	*/
	Relation(Address_t size);

	/**
	*�Ԓnfrom�̃I�u�W�F�N�g����Ԓnto�̃I�u�W�F�N�g�ւ̎Q�Ƃ�����
	*@param from
	*@param to
	*/
	void link(Address_t from, Address_t to);

	/**
	*�Ԓnfrom�̃I�u�W�F�N�g����Ԓnto�̃I�u�W�F�N�g�ւ̎Q�Ƃ��Ȃ���
	*@param from
	*@param to
	*/
	void unlink(Address_t from, Address_t to);

	/**
	*�Ԓnfrom�̃I�u�W�F�N�g����Ԓnto�̃I�u�W�F�N�g�ւ̎Q�Ƃ����邩���ׂ�
	*@param from
	*@param to
	*return from����to�ւ̎Q�Ƃ�����ΐ^
	*/
	bool areLinked(Address_t from, Address_t to)const;
};

}