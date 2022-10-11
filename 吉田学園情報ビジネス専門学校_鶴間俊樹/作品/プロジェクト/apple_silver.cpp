//=============================================================================
//
// ��ь珈�� [apple_silver.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "apple_silver.h"
#include "appleTree.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleSilver::CAppleSilver()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CAppleSilver::CAppleSilver(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleSilver::~CAppleSilver()
{

}

//=============================================================================
// ��ь�̐�������
//=============================================================================
CAppleSilver* CAppleSilver::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//�|�C���^�������S�̖؂ł͂Ȃ��ꍇnull
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleSilver* pAppleSilver = new CAppleSilver(pTree);
	if (pAppleSilver == nullptr) return nullptr;

	//���̃G�t�F�N�g�𐶐�
	pAppleSilver->m_pEffectThunder = CEffect::Create(pos + D3DXVECTOR3(0.0f, -15.0f, 0.0f), CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, true);

	pAppleSilver->SetPos(pos);
	pAppleSilver->Init();

	return pAppleSilver;
}

//=============================================================================
// ��ь�̏���������
//=============================================================================
HRESULT CAppleSilver::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//���f���̐F��ύX
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::SILVER), 0);
	}

	//�����\���̂��߂Ƀp�[�e�B�N���𐶐�
	CreateGlowParticle(APPLE_TYPE::SILVER);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// ��ь�̏I������
//=============================================================================
void CAppleSilver::Uninit(void) {
	//���̃G�t�F�N�g�̔j��
	if (m_pEffectThunder != nullptr) {
		m_pEffectThunder->Uninit();
		m_pEffectThunder = nullptr;
	}

	CGlowApple::Uninit();
}

//=============================================================================
// ��ь�̍X�V����
//=============================================================================
void CAppleSilver::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// ��ь�̕`�揈��
//=============================================================================
void CAppleSilver::Draw(void) {
	CGlowApple::Draw();
}