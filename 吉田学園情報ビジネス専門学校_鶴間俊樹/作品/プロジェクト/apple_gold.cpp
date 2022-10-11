//=============================================================================
//
// ���ь珈�� [apple_gold.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_gold.h"
#include "appleTree.h"
#include "bullet.h"
#include "manager.h"
#include "sound.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define SPAN_BULLET (30)	//�e�̐����Ԋu
#define BULLET_MOVE_SPEED (30.0f)	//�e�̈ړ����x
#define	MAX_DISTANCE_BULLET (1500.0f)	//�e�����ˉ\�ȍő�̋���
#define	BULLET_DAMAGE (5)	//�e�̃_���[�W

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleGold::CAppleGold()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleGold::CAppleGold(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleGold::~CAppleGold()
{

}

//=============================================================================
// ���ь�̐�������
//=============================================================================
CAppleGold* CAppleGold::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//�|�C���^�������S�̖؂ł͂Ȃ��ꍇnull
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleGold* pAppleGold = new CAppleGold(pTree);
	if (pAppleGold == nullptr) return nullptr;

	pAppleGold->SetPos(pos);
	pAppleGold->Init();

	return pAppleGold;
}

//=============================================================================
// ���ь�̏���������
//=============================================================================
HRESULT CAppleGold::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GOLD), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::GOLD);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ���ь�̏I������
//=============================================================================
void CAppleGold::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// ���ь�̍X�V����
//=============================================================================
void CAppleGold::Update(void) {
	if (m_nCntBullet == SPAN_BULLET) {
		m_nCntBullet = 0;
		//�e�̔���
		ShootBullet();
	}
	else {
		m_nCntBullet++;
	}

	CGlowApple::Update();
}

//=============================================================================
// ���ь�̕`�揈��
//=============================================================================
void CAppleGold::Draw(void) {
	CGlowApple::Draw();
}

//=============================================================================
// �e�̔���
//=============================================================================
void CAppleGold::ShootBullet(void) {
	bool bSuccessGetPos = false;		//�ʒu�̎擾�������������ǂ���

	D3DXVECTOR3 posApple = GetPos();	//�ь�̈ʒu
	posApple.y -= 15.0f;	//�ʒu�̒���
	D3DXVECTOR3 posEnemy;	//�G�̈ʒu
	float fDistEnemy;		//�G�ւ̋���

	//��ԋ߂��G�̈ʒu���擾
	bSuccessGetPos = GetNearObject(posApple, OBJTYPE_ENEMY, OBJFLAG_ENABLECOLLISION, &posEnemy, &fDistEnemy, nullptr) != nullptr;

	//�ʒu�̎擾���s�����ꍇ�������Ȃ�
	if (!bSuccessGetPos || fDistEnemy > MAX_DISTANCE_BULLET) return;

	D3DXVECTOR3 vecEnemy = posEnemy - posApple;	//�G�ւ̃x�N�g��
	vecEnemy.y += 50.0f;	//�G�̍����ɍ��킹��

	//���K��
	D3DXVec3Normalize(&vecEnemy, &vecEnemy);

	//�e�̈ړ����x
	D3DXVECTOR3 moveBullet = vecEnemy * BULLET_MOVE_SPEED;	//���K�������G�ւ̕����Ɉړ����x����Z

	//�e�̐���
	CBullet* pBullet = CBullet::Create(CModel::MODELTYPE::OBJ_APPLE_BULLET, posApple, moveBullet, 120, OBJTYPE_ENEMY, 15.0f, BULLET_DAMAGE);
	//�e�̏����ݒ�
	if (pBullet != nullptr) {
		pBullet->SetParticleInfo(10, 25.0f, -2.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	//�O���̃p�[�e�B�N���̐ݒ�
		pBullet->SetNumParticle(3);	//�⊮�p�̃p�[�e�B�N���̐���ݒ�
	}
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�e���ˉ��̍Đ�
	if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOOT_BULLET);
}