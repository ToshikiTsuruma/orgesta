//=============================================================================
//
// �Ηь珈�� [apple_green.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_green.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define HEAL_APPLETREE (50)	//�����S�̖؂̉񕜗�
#define HEAL_SCAPEGOAT (20)	//���т̉񕜗�
#define HEAL_SPAN (FPS * 6)	//�񕜂̃X�p��

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleGreen::CAppleGreen()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleGreen::CAppleGreen(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleGreen::~CAppleGreen()
{

}

//=============================================================================
// �Ηь�̐�������
//=============================================================================
CAppleGreen* CAppleGreen::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//�|�C���^�������S�̖؂ł͂Ȃ��ꍇnull
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleGreen* pAppleGreen = new CAppleGreen(pTree);
	if (pAppleGreen == nullptr) return nullptr;

	pAppleGreen->SetPos(pos);
	pAppleGreen->Init();
	pAppleGreen->m_nCntHeal = HEAL_SPAN;

	return pAppleGreen;
}

//=============================================================================
// �Ηь�̏���������
//=============================================================================
HRESULT CAppleGreen::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GREEN), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::GREEN);

	m_nCntHeal = 0;

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// �Ηь�̏I������
//=============================================================================
void CAppleGreen::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// �Ηь�̍X�V����
//=============================================================================
void CAppleGreen::Update(void) {
	CAppleTree* pAppleTree = GetAppleTree();

	//�J�E���g���B���Ă��āA�؂����݂��Ă���ꍇ
	if (m_nCntHeal >= HEAL_SPAN && pAppleTree != nullptr) {
		//�ь�̖؂��񕜂���
		pAppleTree->Heal(HEAL_APPLETREE);
		//���т̉�
		HealScapegoat();

		//�ь�̈ʒu����񕜂̃G�t�F�N�g���o��
		D3DXVECTOR3 posEffect = GetPos();
		posEffect.y -= 15.0f;	//�ʒu�̒���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 80.0f, 80.0f, false);
		//�؂̈ʒu����񕜃G�t�F�N�g���o��
		posEffect = pAppleTree->GetPos();
		posEffect.y += 150.0f;	//�ʒu�̒���
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 300.0f, 300.0f, false);

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
		//�T�E���h�̎擾
		if (pManager != nullptr) pSound = pManager->GetSound();
		//�񕜗ʉ��̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::HEAL);

		m_nCntHeal = 0;
	}
	else {
		m_nCntHeal++;
	}

	CGlowApple::Update();
}

//=============================================================================
// �Ηь�̕`�揈��
//=============================================================================
void CAppleGreen::Draw(void) {
	CGlowApple::Draw();
}

//=============================================================================
// ���т̉�
//=============================================================================
void CAppleGreen::HealScapegoat(void) {
	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾

		//�I�u�W�F�N�g�^�C�v�̊m�F
		bool bMatchType = false;
		if (pObject->GetObjType() & OBJTYPE_SCAPEGOAT) bMatchType = true;
		//���S��Ԃ̎擾
		bool bDeath = pObject->GetDeath();

		//���̃��[�v�ɔ�΂�
		if (!bMatchType || bDeath) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	
		}
		//���т̉�
		pObject->Heal(HEAL_SCAPEGOAT);

		//�񕜃G�t�F�N�g�̐���
		D3DXVECTOR3 posEffect = pObject->GetPos();
		posEffect.y += 150.0f;
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 150.0f, 150.0f, false);

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}