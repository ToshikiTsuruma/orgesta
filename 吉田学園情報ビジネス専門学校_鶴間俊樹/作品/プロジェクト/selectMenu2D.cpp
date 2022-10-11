//=============================================================================
//
// 2D�I�����j���[���� [selectMenu2D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "selectMenu2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSelectMenu2D::CSelectMenu2D() : CSelectMenu(0, false)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CSelectMenu2D::CSelectMenu2D(int nNumSelect, bool bUseBG) : CSelectMenu(nNumSelect , bUseBG)
{
	CreateSelectUI();
	m_pPosSelectIconOffsetArray = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelectMenu2D::~CSelectMenu2D()
{
	int nNumSelect = GetNumSelect();

	//UI���f���̔j��
	if (m_ppSelectUIArray != nullptr) {
		for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
		{
			m_ppSelectUIArray[nCntModel]->Uninit();
			m_ppSelectUIArray[nCntModel] = nullptr;
		}
	}
}

//=============================================================================
// 2D�I�����j���[�̐�������
//=============================================================================
CSelectMenu2D* CSelectMenu2D::Create(int nNumSelect, bool bUseBG) {
	CSelectMenu2D* pSelectMenu2D;
	pSelectMenu2D = new CSelectMenu2D(nNumSelect, bUseBG);
	if (pSelectMenu2D == nullptr) return nullptr;

	pSelectMenu2D->Init();

	return pSelectMenu2D;
}

//=============================================================================
// 2D�I�����j���[�̏���������
//=============================================================================
HRESULT CSelectMenu2D::Init(void) {
	CSelectMenu::Init();
	return S_OK;
}

//=============================================================================
// 2D�I�����j���[�̏I������
//=============================================================================
void CSelectMenu2D::Uninit(void) {
	int nNumSelect = GetNumSelect();

	//�I����UI�̔j��
	if (m_ppSelectUIArray != nullptr) {
		for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
		{
			if (m_ppSelectUIArray[nCntModel] == nullptr) continue;
			m_ppSelectUIArray[nCntModel]->Uninit();
			m_ppSelectUIArray[nCntModel] = nullptr;
		}
		m_ppSelectUIArray = nullptr;
	}

	//�I�𒆂̃A�C�R���̔j��
	if (m_pSelectIcon != nullptr) {
		m_pSelectIcon->Uninit();
		m_pSelectIcon = nullptr;
	}

	//�I���̃I�t�Z�b�g�̔j��
	if (m_pPosSelectIconOffsetArray != nullptr) {
		delete[] m_pPosSelectIconOffsetArray;
		m_pPosSelectIconOffsetArray = nullptr;
	}

	CSelectMenu::Uninit();
}

//=============================================================================
// 2D�I�����j���[�̍X�V����
//=============================================================================
void CSelectMenu2D::Update(void) {


	CSelectMenu::Update();
}

//=============================================================================
// �I����UI�̐���
//=============================================================================
void CSelectMenu2D::CreateSelectUI(void) {
	//���łɐ����ς݂̏ꍇ�I��
	if (m_ppSelectUIArray != nullptr) return;

	//�I�����̐��̎擾
	int nNumSelect = GetNumSelect();
	m_ppSelectUIArray = new CObject2D*[nNumSelect];

	//�����ł��Ȃ������ꍇ�I��
	if (m_ppSelectUIArray == nullptr) return;

	for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
	{
		//�I����UI�̐���
		m_ppSelectUIArray[nCntModel] = CObject2D::Create();	//��ɊO���Őݒ肷��̂Œl�͓K��
	}
}

//=============================================================================
// �I���̕ύX���J�n���ꂽ�Ƃ��̏���
//=============================================================================
void CSelectMenu2D::BeginChangeSelect(void) {
	if (m_ppSelectUIArray == nullptr) return;

	//�I�𒆂̃A�C�R��������ꍇ
	if (m_pSelectIcon != nullptr && m_pPosSelectIconOffsetArray != nullptr) {
		//���݂̑I����UI�̈ʒu���擾
		D3DXVECTOR3 posSelectUI;
		int nIdxSelect = GetIdxCurSelect();
		if (m_ppSelectUIArray[nIdxSelect] != nullptr) posSelectUI = m_ppSelectUIArray[nIdxSelect]->GetPos();

		//�I�𒆃A�C�R����I�𒆂̑I����UI�ׂ̗ɔz�u����
		m_pSelectIcon->SetPos(posSelectUI + m_pPosSelectIconOffsetArray[nIdxSelect]);
	}
	//�I�𒆂̃A�C�R�����Ȃ��ꍇ
	else {
		//���ׂĂ̑I�����𔒂ɖ߂�
		for (int nCnt = 0; nCnt < GetNumSelect(); nCnt++)
		{
			if (m_ppSelectUIArray[nCnt] != nullptr) m_ppSelectUIArray[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		//�I����UI�̐F�̕ύX
		int nIdxSelect = GetIdxCurSelect();
		if (m_ppSelectUIArray[nIdxSelect] != nullptr) m_ppSelectUIArray[nIdxSelect]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=============================================================================
// �I����UI�̐ݒ�
//=============================================================================
void CSelectMenu2D::SetSelectUI(int nIdx, D3DXVECTOR3 pos, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex) {
	if (m_ppSelectUIArray == nullptr || nIdx < 0 || nIdx >= GetNumSelect()) return;	//�z�񂩂�擾�ł��Ȃ��ꍇ�I��
	//�ݒ肷��UI�̃I�u�W�F�N�g��null�̏ꍇ�I��
	if (m_ppSelectUIArray[nIdx] == nullptr) return;

	m_ppSelectUIArray[nIdx]->SetPos(pos);
	m_ppSelectUIArray[nIdx]->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));
	m_ppSelectUIArray[nIdx]->SetTexType(typeTex);
}

//=============================================================================
// �I�𒆃A�C�R���̐���
//=============================================================================
void CSelectMenu2D::CreateSelectIcon(D3DXVECTOR3 posOffset, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex) {
	//���łɐ����ς݂̏ꍇ�͏I��
	if (m_pSelectIcon != nullptr) return;

	//�I���̃I�t�Z�b�g�������ς݂̏ꍇ�j��
	if (m_pPosSelectIconOffsetArray != nullptr) {
		delete[] m_pPosSelectIconOffsetArray;
		m_pPosSelectIconOffsetArray = nullptr;
	}

	const int nNumSelect = GetNumSelect();	//�I�����̐�

	//�I�𒆃A�C�R���̐���
	m_pSelectIcon = CObject2D::Create(D3DXVECTOR3(), typeTex, fWidth, fHeight);

	//�I���̃I�t�Z�b�g�̔z��𐶐�
	m_pPosSelectIconOffsetArray = new D3DXVECTOR3[nNumSelect];

	//���ׂĂ̑I���̃I�t�Z�b�g�̐ݒ�
	if (m_pPosSelectIconOffsetArray != nullptr) {
		for (int nCnt = 0; nCnt < nNumSelect; nCnt++)
		{
			m_pPosSelectIconOffsetArray[nCnt] = posOffset;
		}
	}
	
	//�I�𒆃A�C�R���̈ʒu�̍X�V
	UpdateSelectIconPos();
}

//=============================================================================
// �I���A�C�R���̈ʒu�̃I�t�Z�b�g��ݒ�
//=============================================================================
void CSelectMenu2D::SetIconPosOffset(int nIdx, D3DXVECTOR3 posOffset) {
	const int nNumSelect = GetNumSelect();	//�I�����̐�

	//null�̏ꍇ�͏I��
	if (m_pPosSelectIconOffsetArray == nullptr || nIdx < 0 || nIdx >= nNumSelect) return;

	//�I�t�Z�b�g�̐ݒ�
	m_pPosSelectIconOffsetArray[nIdx] = posOffset;

	//�I�𒆃A�C�R���̈ʒu�̍X�V
	UpdateSelectIconPos();
}

//=============================================================================
// �I���A�C�R���̈ʒu���X�V
//=============================================================================
void CSelectMenu2D::UpdateSelectIconPos(void) {
	//�I�𒆃A�C�R���̈ʒu�̐ݒ�
	if (m_ppSelectUIArray != nullptr && m_pSelectIcon != nullptr && m_pPosSelectIconOffsetArray != nullptr) {
		//���݂̑I����UI�̈ʒu���擾
		D3DXVECTOR3 posSelectUI;
		int nIdxSelect = GetIdxCurSelect();
		if (m_ppSelectUIArray[nIdxSelect] != nullptr) posSelectUI = m_ppSelectUIArray[nIdxSelect]->GetPos();

		//�I�𒆃A�C�R����I�𒆂̑I����UI�ׂ̗ɔz�u����
		m_pSelectIcon->SetPos(posSelectUI + m_pPosSelectIconOffsetArray[nIdxSelect]);
	}
}