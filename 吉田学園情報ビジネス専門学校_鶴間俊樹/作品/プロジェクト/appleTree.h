//=============================================================================
//
// �ь�̖؂̏��� [appleTree.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _APPLE_TREE_H_
#define _APPLE_TREE_H_

#include "main.h"
#include "objectModel.h"
#include "glowApple.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define APPLETREE_POS_Y (430.0f)
#define MAX_LIFE_APPLETREE (2000)		//�̗͂̍ő�l
#define MAX_NUM_CREATE_APPLE (6)	//�����\�ȗь�̍ő吔

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CAppleMenu;
class CGauge2D;
class CObject2D;

//*****************************************************************************
// �ь�̖؃N���X
//*****************************************************************************
class CAppleTree : public CObjectModel
{
public:
	CAppleTree();		//�f�t�H���g�R���X�g���N�^
	virtual ~CAppleTree();		//�f�X�g���N�^
	static CAppleTree* Create(D3DXVECTOR3 pos);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void Heal(int nHeal);		//�̗͂��񕜂���
	void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead);		//�_���[�W
	void Dead(void);		//���S������

	static CGlowApple* CreateApple(CGlowApple::APPLE_TYPE typeApple, D3DXVECTOR3 posCreate, CAppleTree* pParentTree);	//�ь�̐���
	void YieldApple(CGlowApple::APPLE_TYPE typeApple);	//�ь炪��������
	static D3DXVECTOR3 GetOffsetPosApple(int nIdxApple);	//�ь琶���ʒu�̎擾
	void SetMaxLife(int nMaxLife);	//�̗͂̍ő�l�̐ݒ�
	void AddMaxLife(int nAddLife);	//�̗͂̍ő�l�̑���
	void AddGrow(int nAddValue);	//�����x�𑝂₷
	CGlowApple::APPLE_TYPE GetCreateAppleType(int nIdx);	//�����S�̎�ނ̎擾
	int GetNumApple(CGlowApple::APPLE_TYPE type);	//�����S�̐����擾

	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//�����蔻��̏��̎擾

private:
	void GrowUp(void);	//����

	int m_nNumApple;	//���������ь�̐�
	CGlowApple* m_apCreateApple[MAX_NUM_CREATE_APPLE];	//�������������S�̃|�C���^
	CGlowApple::APPLE_TYPE m_aTypeCreateApple[MAX_NUM_CREATE_APPLE];	//�������������S�̎��

	//�̗�
	int m_nMaxLife;			//�̗͂̍ő�l
	int m_nLife;			//�̗�
	CGauge2D* m_pGaugeLife;	//�̗̓Q�[�W
	CObject2D* m_pIconHP;	//HP�̃A�C�R��
	//����
	int m_nGrowValue;		//�����x
	int m_nGrowValueMax;	//�����x�̍ő�i�����̕K�v�ʁj
	CGauge2D* m_pGaugeGrow;	//�����Q�[�W

	bool m_bDead;			//���S
	int m_nCntDead;			//���S��̃J�E���g

	CAppleMenu* m_pMenuApple;	//�ь�̃��j���[

};

#endif // !_APPLE_TREE_H_