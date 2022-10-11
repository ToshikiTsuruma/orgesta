//=============================================================================
//
// �e���� [bullet.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "objectModel.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CObjectList;

//*****************************************************************************
// �e�N���X
//*****************************************************************************
class CBullet : public CObjectModel
{
public:
	CBullet();	//�f�t�H���g�R���X�g���N�^
	CBullet(CModel::MODELTYPE typeModel);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CBullet();	//�f�X�g���N�^
	static CBullet* Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nObjtypeAttack, float fRadiusCol, int nDamage);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void);	//�`�揈��
	void SetParticleInfo(int nLife, float fSize, float fAddSize, D3DXCOLOR colStart, D3DXCOLOR colEnd);	//�p�[�e�B�N������ݒ�
	void SetNumParticle(int nNum) { m_nNumParticle = nNum; }	//�⊮�̃p�[�e�B�N���̐��̐ݒ�

private:
	bool AttackCollision(void);	//�U���Ώۂւ̓����蔻��

	D3DXVECTOR3 m_move;	//�ړ���
	int m_nLife;	//���C�t
	int m_nObjtypeAttack;	//�U���Ώۂ̃I�u�W�F�N�g�^�C�v
	float m_fRadiusCol;	//�����蔻��̔��a
	int m_nDamage;	//�e�̃_���[�W
	CObjectList* m_pListAttacked;	//�U���ς݂̃I�u�W�F�N�g�̃��X�g

	//�O���p�[�e�B�N���̏��
	bool m_bCreateParticle;	//�p�[�e�B�N���������邩�ǂ���
	int m_nLifeParticle;	//�p�[�e�B�N���̃��C�t
	float m_fSizeParticle;	//�p�[�e�B�N���̃T�C�Y���Z��
	float m_fAddSizeParticle;	//�p�[�e�B�N���̃T�C�Y���Z��
	D3DXCOLOR m_colStartParticle;	//�p�[�e�B�N���̊J�n�F
	D3DXCOLOR m_colEndParticle;		//�p�[�e�B�N���̏I���F
	//�p�[�e�B�N���̕⊮
	bool m_bCreateParticleFirst;	//�p�[�e�B�N���̏���̐������I��������ǂ���
	int m_nNumParticle;				//�ǉ������p�[�e�B�N���̐�
	D3DXVECTOR3 m_posLastParticle;	//�O��̃p�[�e�B�N�������ʒu
};

#endif // !_BULLET_H_