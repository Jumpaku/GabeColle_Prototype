#pragma once

#include<vector>

namespace gc {


/**
*�}�[�N�A���h�X�C�[�v�@�ɂ��K�x�[�W�R���N�V�����ł�,<br>
*1.���[�g����Q�Ƃ�H���I�u�W�F�N�g�Ƀ}�[�N��t����<br>
*2.�}�[�N�̕t���Ă��Ȃ��I�u�W�F�N�g��j������<br>
*�Ƃ������Ԃŕs�v�ȃI�u�W�F�N�g��j������<br>
*Marking�N���X�͔Ԓn�Ƃ��̐�̃I�u�W�F�N�g�Ƀ}�[�N���t���Ă��邩�ǂ����̊֌W���Ǘ�����<br>
*@see GarbageCollection
*/
class Marking
{
public:
	typedef std::vector<bool> MarkingMap_t;
	typedef int Address_t;
private:
	MarkingMap_t map_m;
public:
	/**
	*�R���X�g���N�^
	*@param size �������̍ő�̃I�u�W�F�N�g��
	*/
	Marking(int size);
	
	/**
	*address�̃I�u�W�F�N�g�Ƀ}�[�N��t����
	*@param address
	*/
	void mark(Address_t address);
	
	/**
	*address�̃I�u�W�F�N�g�Ƀ}�[�N���O��
	*@param address
	*/
	void unmark(Address_t address);

	/**
	*address�̃I�u�W�F�N�g�Ƀ}�[�N���t���Ă��邩���ׂ�
	*@param address
	*@return �I�u�W�F�N�g�Ƀ}�[�N���t���Ă��鎞�^
	*/
	bool isMarked(Address_t address)const;
};

}