//=============================================================================
//
// �t�H���g���� [font.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "font.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CFont::CFont()
{
	SetDrawPriority(DRAW_PRIORITY::UI);

	wsprintf(m_sText, "text");
	m_rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nFlagAnchor = DT_LEFT;	//�f�t�H���g�͍�����
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CFont::~CFont()
{

}

//=============================================================================
// �t�H���g�̐�������
//=============================================================================
CFont* CFont::Create(const char* pStrText, int nFlagAnchor, float fSide, float fTop, int nFontWidth) {
	CFont* pFont;
	pFont = new CFont;
	if (pFont == nullptr) return nullptr;

	wsprintf(pFont->m_sText, pStrText);
	pFont->m_nFlagAnchor = nFlagAnchor;
	//������ʒu�ɂ���Đݒ肷��ʒu�̎�ނ�ς���
	if (nFlagAnchor == DT_RIGHT) {
		pFont->m_rect.right = (LONG)fSide;
	}
	else {
		pFont->m_rect.left = (LONG)fSide;
	}
	pFont->m_rect.top = (LONG)fTop;
	pFont->m_nFontWidth = nFontWidth;
	pFont->Init();

	return pFont;
}

//=============================================================================
// �t�H���g�̏���������
//=============================================================================
HRESULT CFont::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return S_OK;

	if (m_pFont == nullptr) {
		//�t�H���g�̐���
		D3DXCreateFont(pDevice, m_nFontWidth, 0, FW_REGULAR, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "�l�r ����", &m_pFont);
	}

	return S_OK;
}

//=============================================================================
// �t�H���g�̏I������
//=============================================================================
void CFont::Uninit(void) {
	//���_�o�b�t�@�̔j��
	if (m_pFont != nullptr) {
		m_pFont->Release();
		m_pFont = nullptr;
	}
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �t�H���g�̍X�V����
//=============================================================================
void CFont::Update(void) {

}

//=============================================================================
// �t�H���g�̕`�揈��
//=============================================================================
void CFont::Draw(void) {
	// �e�L�X�g�`��
	m_pFont->DrawText(nullptr, m_sText, -1, &m_rect, m_nFlagAnchor, m_col);
}

//=============================================================================
// �t�H���g�̕�����̐ݒ�
//=============================================================================
void CFont::SetText(const char* pText) {
	if (pText == nullptr) return;

	if (strlen(pText) < MAX_FONT_TEXT) {
		strcpy(m_sText, pText);
	}
}

//=============================================================================
// rect�̐ݒ�
//=============================================================================
void CFont::SetRect(RECT rect) {
	m_rect = rect;
}

//=============================================================================
// rect�̎擾
//=============================================================================
RECT CFont::GetRect(void) {
	return m_rect;
}