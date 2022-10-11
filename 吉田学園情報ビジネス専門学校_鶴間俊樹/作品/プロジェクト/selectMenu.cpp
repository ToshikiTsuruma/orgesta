//=============================================================================
//
// �I�����j���[���� [selectMenu.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "selectMenu.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "object2D.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CSelectMenu::CSelectMenu() : m_nNumSelect(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CSelectMenu::CSelectMenu(int nNumSelect, bool bUseBG) : m_nNumSelect(nNumSelect)
{
	m_typeSelect = SELECT_TYPE::VERTICAL;
	m_nIdxCurSelect = 0;

	if (bUseBG) {
		//�w�i�̐���
		m_pMenuBG = CObject2D::Create(D3DXVECTOR3(), CTexture::TEXTURE_TYPE::NONE, 0.0f, 0.0f);	//��Œl��ݒ肷��
	}
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CSelectMenu::~CSelectMenu()
{

}

//=============================================================================
// �I�����j���[�̏���������
//=============================================================================
HRESULT CSelectMenu::Init(void) {
	m_nIdxCurSelect = 0;
	m_bLockChangeSelect = true;
	m_bFirstFrame = true;

	return S_OK;
}

//=============================================================================
// �I�����j���[�̏I������
//=============================================================================
void CSelectMenu::Uninit(void) {
	//���j���[�̔w�i�̔j��
	if (m_pMenuBG != nullptr) {
		m_pMenuBG->Uninit();
		m_pMenuBG = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �I�����j���[�̍X�V����
//=============================================================================
void CSelectMenu::Update(void) {
	//�I���̕ύX
	if(!m_bLockChangeSelect) ChangeSelect();

	//�ŏ��̂P�t���[���������ꍇ
	if (m_bFirstFrame) {
		m_bFirstFrame = false;
		//���͂̃��b�N������
		m_bLockChangeSelect = false;
	}
}

//=============================================================================
// �I���̕ύX
//=============================================================================
void CSelectMenu::ChangeSelect(void) {
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	if (pManager == nullptr) return;
	CInput* pInput = pManager->GetInputCur();	//���݂̓��̓f�o�C�X�ւ̃|�C���^
	if (pInput == nullptr) return;

	bool bChangeSelect = false;	//�ύX�����������ǂ���

	switch (m_typeSelect)
	{
		//�㉺
	case CSelectMenu::SELECT_TYPE::VERTICAL:
		//�I���̕ύX�̓���
		if (pInput->GetTrigger(CInput::CODE::SELECT_UP)) {
			m_nIdxCurSelect--;	//�I�����P�O��
			if (m_nIdxCurSelect < 0) m_nIdxCurSelect = m_nNumSelect - 1;	//�O����������ꍇ�ő�̔ԍ�
			bChangeSelect = true;
		}
		if (pInput->GetTrigger(CInput::CODE::SELECT_DOWN)) {
			m_nIdxCurSelect++;	//�I�����P���
			if (m_nIdxCurSelect >= m_nNumSelect) m_nIdxCurSelect = 0;	//�ő���������ꍇ�O
			bChangeSelect = true;
		}
		break;
		//���E
	case CSelectMenu::SELECT_TYPE::HORIZONTAL:
		//�I���̕ύX�̓���
		if (pInput->GetTrigger(CInput::CODE::SELECT_LFET)) {
			m_nIdxCurSelect--;	//�I�����P�O��
			if (m_nIdxCurSelect < 0) m_nIdxCurSelect = m_nNumSelect - 1;	//�O����������ꍇ�ő�̔ԍ�
			bChangeSelect = true;
		}
		if (pInput->GetTrigger(CInput::CODE::SELECT_RIGHT)) {
			m_nIdxCurSelect++;	//�I�����P���
			if (m_nIdxCurSelect >= m_nNumSelect) m_nIdxCurSelect = 0;	//�ő���������ꍇ�O
			bChangeSelect = true;
		}
		break;
	}

	//�ύX���������Ƃ��̏���
	if (bChangeSelect) {
		BeginChangeSelect();	//�ύX�J�n���̏�����h���N���X�ōs��

		//�}�l�[�W���[�̎擾
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
		//�T�E���h�̎擾
		if (pManager != nullptr) pSound = pManager->GetSound();
		//�I��ύX���̍Đ�
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::SELECT_SWITCH);
	}
}

//=============================================================================
// ���j���[�̔w�i�̐ݒ�
//=============================================================================
void CSelectMenu::SetMenuBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	if (m_pMenuBG == nullptr) return;

	//�w�i�̐ݒ�
	m_pMenuBG->SetTexType(typeTex);
	m_pMenuBG->SetPos(pos);
	m_pMenuBG->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));
}