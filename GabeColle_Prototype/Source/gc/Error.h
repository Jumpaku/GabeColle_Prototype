#pragma once 

namespace gc {


/**
*�l�X�ȃG���[�̐����L������
*/
class Error
{
public:
	/**�������ĂȂ��I�u�W�F�N�g�ւ̃A�N�Z�X*/
	int segmentationFault_m = 0;
	/**�k���|�C���^�ւ̃A�N�Z�X*/
	int nullptrAccess_m = 0;
	/**�������s���ŃI�u�W�F�N�g�𐶐��ł��Ȃ�*/
	int outOfMemory_m = 0;
	/**�A�h���X���������͈̔͊O*/
	int addressOutOfBounds_m = 0;
};

}