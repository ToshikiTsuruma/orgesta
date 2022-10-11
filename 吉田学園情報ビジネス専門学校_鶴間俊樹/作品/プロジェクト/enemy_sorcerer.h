//=============================================================================
//
// ���p�t�G���� [enemy_sorcerer.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ENEMY_SORCERER_H_
#define _ENEMY_SORCERER_H_

#include "enemy.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CBillboard;

//*****************************************************************************
// ���p�t�G�N���X
//*****************************************************************************
class CEnemySorcerer : public CEnemy
{
public:
	//���[�V�����̎��
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//�j���[�g�������[�V����
		ATTACK,	//�U��
		DEAD,	//���S
		ENUM_MAX
	};

	CEnemySorcerer();	//�f�t�H���g�R���X�g���N�^
	~CEnemySorcerer();	//�f�X�g���N�^

	static CEnemySorcerer* Create(D3DXVECTOR3 pos);	//��������
	static void LoadMotionInfo(void);	//���[�V�����������[�h
	static void UnloadMotionInfo(void);	//���[�V���������A�����[�h

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾

	MOTION_CATEGORY GetMotionCategory(void);	//���[�V�����̋敪�̎擾
	void SetMoveMotion(void) {}		//�ړ����̃��[�V������ݒ�
	void SetDeadMotion(void);	//���S���̃��[�V������ݒ�
	void AttackStart(void);		//�U���J�n��
	void AttackStop(void);		//�U����~
	void MotionEnd(void);	//���[�V�����I����
	void MotionAct(void);	//���[�V�������̍s��

private:
	static int m_nNumParts;	//�p�[�c��
	static PARTS_INFO* m_pPartsInfoArray;	//�p�[�c���̔z��̃|�C���^
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//���[�V�������

	CBillboard* m_pFireBoll;	//�΂̒e
};

#endif // !_ENEMY_SORCERER_H_