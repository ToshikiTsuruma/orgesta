//=============================================================================
//
// ���ь珈�� [apple_black.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_black.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "repulsiveWave.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define EMIT_SPAN (FPS * 40)	//�˗͔g���o�̃X�p��

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleBlack::CAppleBlack()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleBlack::CAppleBlack(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleBlack::~CAppleBlack()
{

}

//=============================================================================
// ���ь�̐�������
//=============================================================================
CAppleBlack* CAppleBlack::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//�|�C���^�������S�̖؂ł͂Ȃ��ꍇnull
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleBlack* pAppleBlack = new CAppleBlack(pTree);
	if (pAppleBlack == nullptr) return nullptr;

	pAppleBlack->SetPos(pos);
	pAppleBlack->Init();
	pAppleBlack->m_nCntEmit = EMIT_SPAN;

	return pAppleBlack;
}

//=============================================================================
// ���ь�̏���������
//=============================================================================
HRESULT CAppleBlack::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::BLACK), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::BLACK);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ���ь�̏I������
//=============================================================================
void CAppleBlack::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// ���ь�̍X�V����
//=============================================================================
void CAppleBlack::Update(void) {
	CAppleTree* pAppleTree = GetAppleTree();	//�����S�̖؂̎擾

	//�J�E���g���B���Ă��āA�؂����݂��Ă���ꍇ
	if (m_nCntEmit >= EMIT_SPAN && pAppleTree != nullptr) {
		//�˗͔g����o����
		CRepulsivewave::Create(pAppleTree->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), 0.0f, 120.0f, 25.0f, 25, 0.2f);

		//�ь�̈ʒu������o�̃G�t�F�N�g���o��
		D3DXVECTOR3 posEffect = GetPos();
		posEffect.y -= 15.0f;	//�ʒu�̒���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::REPULSIVEWAVE, 80.0f, 80.0f, false);

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
		//�T�E���h�̎擾
		if (pManager != nullptr) pSound = pManager->GetSound();
		//���o���̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::REPULSIVE_WAVE);

		m_nCntEmit = 0;
	}
	else {
		m_nCntEmit++;
	}

	CGlowApple::Update();
}

//=============================================================================
// ���ь�̕`�揈��
//=============================================================================
void CAppleBlack::Draw(void) {
	CGlowApple::Draw();
}