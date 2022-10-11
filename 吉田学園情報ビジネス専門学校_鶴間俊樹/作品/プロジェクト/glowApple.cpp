//=============================================================================
//
// �O���[�A�b�v������ [glowApple.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "glowApple.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "appleTree.h"
#include "effect.h"
#include "particle.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGlowApple::CGlowApple()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CGlowApple::CGlowApple(CAppleTree* pTree) : CObjectModel(CModel::MODELTYPE::OBJ_APPLE, false)
{
	SetObjType(OBJTYPE_APPLE);
	m_pAppleTree = pTree;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGlowApple::~CGlowApple()
{

}

//=============================================================================
// �O���[�A�b�v���̏���������
//=============================================================================
HRESULT CGlowApple::Init(void) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetMaterialPower(4.0f, 0);
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// �O���[�A�b�v���̏I������
//=============================================================================
void CGlowApple::Uninit(void) {
	//�����\���p�p�[�e�B�N���̔j��
	if (m_pGlowParticle != nullptr) {
		m_pGlowParticle->Uninit();
		m_pGlowParticle = nullptr;
	}

	CObjectModel::Uninit();
}

//=============================================================================
// �O���[�A�b�v���̍X�V����
//=============================================================================
void CGlowApple::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// �O���[�A�b�v���̕`�揈��
//=============================================================================
void CGlowApple::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// ���S
//=============================================================================
void CGlowApple::Dead(void) {
	//���S�G�t�F�N�g
	//CEffect::Create(GetPos(), CEffect::EFFECT_TYPE::DAMAGE_ENEMY, 50.0f, 50.0f, false);
	//�I������
	Uninit();
}

//=============================================================================
// �����\���p�p�[�e�B�N���̐���
//=============================================================================
void CGlowApple::CreateGlowParticle(APPLE_TYPE type) {
	//���łɐ�������Ă���ꍇ�I��
	if (m_pGlowParticle != nullptr) return;

	//���S�ʒu�̎擾
	D3DXVECTOR3 posGlow = GetPos();
	posGlow.y -= 18.0f;	//�ʒu����

	//�F�̌���
	D3DXCOLOR colGlow = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		colGlow = D3DXCOLOR(0.6f, 1.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		colGlow = D3DXCOLOR(0.6f, 1.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::WHITE:
		colGlow = D3DXCOLOR(0.6f, 1.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		colGlow = D3DXCOLOR(0.5f, 0.0f, 1.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		colGlow = D3DXCOLOR(0.0f, 0.7f, 1.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		colGlow = D3DXCOLOR(1.0f, 0.4f, 0.0f, 1.0f);
		break;
	}

	//�p�[�e�B�N���̐���
	m_pGlowParticle = CParticle::Create(posGlow, -1, 32.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), colGlow, colGlow);
}

//=============================================================================
// ��ނ��Ƃ̗ь�̐F���擾
//=============================================================================
D3DXCOLOR CGlowApple::GetAppleColor(APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		return D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		return D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::WHITE:
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		return D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		return D3DXCOLOR(0.4f, 0.4f, 0.6f, 1.0f);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		return D3DXCOLOR(1.0f, 0.7f, 0.0f, 1.0f);
		break;
	default:
		return D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.5f);
		break;
	}
}

//=============================================================================
//��ނ��Ƃ̗ь�̐����e�L�X�g
//=============================================================================
void CGlowApple::GetAppleText(char* pStr, APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		strcpy(pStr, "Red Apple : �̗͂̍ő�l��������");
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		strcpy(pStr, "Green Apple : ���b���Ƃɉ񕜂���");
		break;
	case CGlowApple::APPLE_TYPE::WHITE:
		strcpy(pStr, "White Apple :  �U�����̏Ռ��g�̐���������");
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		strcpy(pStr, "Black Apple : �؂̒��S����˗͔g�����");
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		strcpy(pStr, "Silver Apple : �U�����̗����̐���������");
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		strcpy(pStr, "Gold Apple : ��ԋ߂��̓G�Ƀ����S�e������");
		break;
	}
}