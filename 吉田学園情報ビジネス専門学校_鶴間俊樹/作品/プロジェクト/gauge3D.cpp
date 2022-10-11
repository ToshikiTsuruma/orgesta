//=============================================================================
//
// 3D�Q�[�W���� [gauge3D.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "gauge3D.h"
#include "billboard.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CGauge3D::CGauge3D()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CGauge3D::CGauge3D(int nMaxValue, bool bVertical) : CGauge(nMaxValue, bVertical)
{

}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CGauge3D::~CGauge3D()
{

}

//=============================================================================
// 3D�Q�[�W�̐�������
//=============================================================================
CGauge3D* CGauge3D::Create(int nMaxValue, bool bVertical, int nValue, int nDrawLifeMax, bool bExtend) {
	CGauge3D* pGauge3D;
	pGauge3D = new CGauge3D(nMaxValue, bVertical);
	if (pGauge3D == nullptr) return nullptr;

	pGauge3D->Init();
	pGauge3D->SetGaugeValue(nValue);
	pGauge3D->SetMaxDrawLife(nDrawLifeMax);
	pGauge3D->SetExtend(bExtend);

	return pGauge3D;
}

//=============================================================================
// 3D�Q�[�W�̏���������
//=============================================================================
HRESULT CGauge3D::Init(void) {
	CGauge::Init();
	return S_OK;
}

//=============================================================================
// 3D�Q�[�W�̏I������
//=============================================================================
void CGauge3D::Uninit(void) {
	CGauge::Uninit();
}

//=============================================================================
// 3D�Q�[�W�̍X�V����
//=============================================================================
void CGauge3D::Update(void) {
	CGauge::Update();
}

//=============================================================================
// 3D�Q�[�W�𐶐�
//=============================================================================
void CGauge3D::CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//�Q�[�W�̐���
	CObject* pGauge = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (pGauge != nullptr) pGauge->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
	//�Q�[�W�̃|�C���^��ݒ�
	SetGaugePtr(pGauge);
}

//=============================================================================
// 3D�Q�[�W�̔w�i�𐶐�
//=============================================================================
void CGauge3D::CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//�w�i�̐���
	CObject* pGaugeBG = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (pGaugeBG != nullptr) pGaugeBG->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
	//�Q�[�W�̃|�C���^��ݒ�
	SetGaugeBGPtr(pGaugeBG);
}

//=============================================================================
// 3D�Q�[�W�̘g�𐶐�
//=============================================================================
void CGauge3D::CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//�w�i�̐���
	CObject* pGaugeFrame = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (pGaugeFrame != nullptr) pGaugeFrame->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
	//�Q�[�W�̃|�C���^��ݒ�
	SetGaugeFramePtr(pGaugeFrame);
}