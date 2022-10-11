//=============================================================================
//
// �v���C���[���� [player.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "objectMotion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CObject2D;
class CWeapon;
class CInput;

//*****************************************************************************
// �v���C���[�N���X
//*****************************************************************************
class CPlayer : public CObjectMotion
{
public:
	//���[�V�����̎��
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//�j���[�g�������[�V����
		WALK,		//����
		PUNCH,		//�p���`
		STAMP,		//���݂�
		RUSH,		//�ːi
		ENUM_MAX
	};

	//���[�V�����̋敪
	enum class MOTION_CATEGORY {
		NONE = 0,	//���̑�
		MOVE,		//�ړ�
		ATTACK,		//�U��
		ENUM_MAX
	};	

	//����̎��
	enum class WEAPONTYPE {
		PUNCH = 0,
		SWORD,
		KATANA,
		ENUM_MAX
	};

	CPlayer();	//�f�t�H���g�R���X�g���N�^
	~CPlayer();	//�f�X�g���N�^

	static CPlayer* Create(D3DXVECTOR3 pos);	//��������
	static void LoadMotionInfo(void);	//���[�V�����������[�h
	static void UnloadMotionInfo(void);	//���[�V���������A�����[�h

	HRESULT Init(void);		//����������
	void Uninit(void);		//�I������
	void Update(void);		//�X�V����
	void Draw(void);		//�`�揈��
	void GameClear(void);	//�Q�[���N���A���̏���
	void GameOver(void);	//�Q�[���I�[�o�[���̏���
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾
	D3DXVECTOR3 GetLastPos(void);	//�Ō�̈ʒu���W�̎擾
	void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	D3DXVECTOR3 GetMove(void);		//�ړ��ʂ̎擾
	float GetRadius(void);			//�����蔻��̔��a�̎擾
	void AddMoveSpeed(float fAddSpeed) { m_fAddMoveSpeed = fAddSpeed; }		//�ړ����x�����Z
	void AddAttackDamage(int nAddDamage) { m_nAddDamage += nAddDamage; }	//�ǉ��_���[�W�����Z

private:
	void StartAttack(void);	//�U���J�n
	void Move(CInput* pInput, float fRotCameraY);	//�ړ�
	void DecMove(void);	//�ړ��ʂ̌���
	void Collision(D3DXVECTOR3& pos);	//�����蔻��
	void EndMotion(void);		//���[�V�����I����
	void MotionAction(void);	//���[�V�������̏���
	void AttackCollision(int nIdxModel, const int nNumCol, float fRadiusCol, D3DXVECTOR3 posOffsetStart, D3DXVECTOR3 posOffsetAdd, int nDamage);	//�U���̓����蔻��
	void AttackBegin(void);	//�U���J�n��
	MOTION_CATEGORY GetMotionCategory(void);	//���[�V�����̋敪�̎擾

	static int m_nNumParts;	//�p�[�c��
	static PARTS_INFO* m_pPartsInfoArray;	//�p�[�c���̔z��̃|�C���^
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//���[�V�������

	D3DXVECTOR3	m_lastPos;	//�Ō�̈ʒu���W
	D3DXVECTOR3 m_destRot;	//�ڕW�̊p�x
	float m_fAddMoveSpeed;	//�ǉ��̈ړ����x
	D3DXVECTOR3 m_move;		//�ړ���
	bool m_bLand;			//�ڒn�����ǂ���
	int m_nAddDamage;		//�ǉ��ŗ^����_���[�W��
	bool m_bValidAttack;	//�U�����L�����ǂ���
	int m_nNumKillEnemy;	//�G��|������
	int m_nCntLockAct;		//�U��������̍d���J�E���g

	int m_nCntGameover;		//�Q�[���I�[�o�[��̃J�E���g
};

#endif // !_PLAYER_H_