//=============================================================================
//
// �c�؂̏��� [seedling.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "seedling.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "scapegoat.h"
#include "terrain.h"
#include "particleEffect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define DEFAULT_NUM_NEED_ENEMY (50)

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSeedling::CSeedling() : CObjectModel(CModel::MODELTYPE::OBJ_SEEDLING, false)
{
	SetObjType(OBJTYPE_SEEDLING);	//�I�u�W�F�N�g�^�C�v�̐ݒ�
	m_nNumNeedEnemy = DEFAULT_NUM_NEED_ENEMY;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSeedling::~CSeedling()
{

}

//=============================================================================
// �c�؂̐�������
//=============================================================================
CSeedling* CSeedling::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CSeedling* pSeedling;
	pSeedling = new CSeedling();
	if (pSeedling == nullptr) return nullptr;

	CTerrain::Collision(pos);
	
	pSeedling->SetPos(pos);
	pSeedling->SetRot(rot);

	pSeedling->Init();

	return pSeedling;
}

//=============================================================================
// �c�؂̏���������
//=============================================================================
HRESULT CSeedling::Init(void) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();

	return S_OK;
}

//=============================================================================
// �c�؂̏I������
//=============================================================================
void CSeedling::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// �c�؂̍X�V����
//=============================================================================
void CSeedling::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// �c�؂̕`�揈��
//=============================================================================
void CSeedling::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// ���т𐶐�
//=============================================================================
void CSeedling::CreateScapegoat(void) {
	//���т̐���
	CScapegoat::Create(GetPos(), GetRot());

	//�p�[�e�B�N���G�t�F�N�g
	CParticleEffect::PARTICLE_INFO particleInfo;
	particleInfo.addMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -0.5f; particleInfo.fSizeStart = 30.0f; particleInfo.fSpeedMove = 5.0f; particleInfo.nLife = 60;
	//����Ƀp�[�e�B�N���𐶐�
	CParticleEffect::EmitSphere(particleInfo, GetPos() + D3DXVECTOR3(0.0f, 150.0f, 0.0f), 12, 12, -0.06f);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�������̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CREATE_APPLE);

	//�c�؂̔j��
	Uninit();
}