//=============================================================================
//
// �G�l�~�[���� [enemy.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "objectMotion.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_DIGIT_ADD_TIME_ENEMY (2)	//�G��|�����Ƃ��ɒǉ�����鎞�Ԃ̍ő包��
#define ENEMY_ATTACK_TARGET (OBJTYPE_APPLETREE | OBJTYPE_SCAPEGOAT)

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CGauge3D;

//*****************************************************************************
// �G�l�~�[�N���X
//*****************************************************************************
class CEnemy : public CObjectMotion
{
public:
	enum class MOTION_CATEGORY {
		NEUTRAL = 0,	//�j���[�g����
		MOVE,			//�ړ�
		ATTACK,			//�U��
		DEAD,			//���S
		ENUM_MAX
	};	//���[�V�����̋敪

	CEnemy();	//�f�t�H���g�R���X�g���N�^
	CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, float fRotSpeed, int nLife, float fDistAttack, float fHeightLifeGauge);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CEnemy();	//�f�X�g���N�^

	virtual HRESULT Init(void);		//����������
	virtual void Uninit(void);		//�I������
	virtual void Update(void);		//�X�V����
	virtual void Draw(void);		//�`�揈��
	void ReleasePtr(CObject* pReleaseObj) { if(m_pAttackTarget == pReleaseObj) m_pAttackTarget = nullptr; }	//���̃I�u�W�F�N�g���ێ����Ă���A�j�������\��̃I�u�W�F�N�g�̃|�C���^��null�ɂ���

	void SetMove(D3DXVECTOR3 move);	//�ړ��ʂ̐ݒ�
	void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead);		//�_���[�W
	void Dead(void);		//���S������
	void SetKillScore(int nScore) { m_nKillScore = nScore; }	//�|�����X�R�A�̐ݒ�

	static void SetGoldRush(bool bRush) { m_bGoldRush = bRush; }	//�S�[���h���b�V���̐ݒ�

	virtual MOTION_CATEGORY GetMotionCategory(void) = 0;	//���[�V�����̋敪�̎擾
	virtual void SetMoveMotion(void) = 0;	//�ړ����̃��[�V������ݒ�
	virtual void SetDeadMotion(void) = 0;	//���S���̃��[�V������ݒ�
	virtual void AttackStart(void) = 0;		//�U���J�n��
	virtual void AttackStop(void) = 0;		//�U����~
	virtual void MotionEnd(void) = 0;	//���[�V�����I����
	virtual void MotionAct(void) = 0;	//���[�V�������̍s��

protected:
	CObject* GetAttackTarget(void) { return m_pAttackTarget; }	//�U���ڕW�̎擾

private:
	void SearchAttackTarget(void);	//�U���ڕW��T��
	void SetRotToTarget(void);	//�U���ڕW�̕����ւ̊p�x�ɐݒ�
	void AttackDistance(void);	//�U���\�ȋ������ǂ������v�Z
	void Attack(void);	//�U���̏���

	const float m_fMoveSpeed;	//�ړ����x
	const float m_fRotateSpeed;	//��]���x
	const float m_fDistAttack;	//�U�����鋗��
	const int m_nMaxLife;	//�ő�̗̑�

	D3DXVECTOR3 m_move;		//�ړ���
	bool m_bMove;			//�ړ��\���ǂ���
	float m_rotDestY;		//�ڕW��Y���̊p�x
	bool m_bLand;			//�ڒn���Ă��邩�ǂ���
	CObject* m_pAttackTarget;	//�U���ڕW
	bool m_bAttackDist;		//�U���\�ȋ������ǂ���
	int m_nLife;			//�̗�
	bool m_bDead;			//���S
	int m_nCntDead;			//���S��̃J�E���g
	int m_nKillScore;		//�|�����Ƃ��̃X�R�A

	static bool m_bGoldRush;	//���ׂĂ̓G�����ɂȂ邩�ǂ���
	bool m_bGoldEnemy;		//���F�̓G���ǂ���

	CGauge3D* m_pGaugeLife;	//�̗̓Q�[�W
	const float m_fHeightLifeGauge;	//�̗̓Q�[�W�̍����̍���
};

#endif // !_ENEMY_H_