//=============================================================================
//
// 3Dゲージ処理 [gauge3D.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "gauge3D.h"
#include "billboard.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGauge3D::CGauge3D()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CGauge3D::CGauge3D(int nMaxValue, bool bVertical) : CGauge(nMaxValue, bVertical)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge3D::~CGauge3D()
{

}

//=============================================================================
// 3Dゲージの生成処理
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
// 3Dゲージの初期化処理
//=============================================================================
HRESULT CGauge3D::Init(void) {
	CGauge::Init();
	return S_OK;
}

//=============================================================================
// 3Dゲージの終了処理
//=============================================================================
void CGauge3D::Uninit(void) {
	CGauge::Uninit();
}

//=============================================================================
// 3Dゲージの更新処理
//=============================================================================
void CGauge3D::Update(void) {
	CGauge::Update();
}

//=============================================================================
// 3Dゲージを生成
//=============================================================================
void CGauge3D::CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//ゲージの生成
	CObject* pGauge = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (pGauge != nullptr) pGauge->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
	//ゲージのポインタを設定
	SetGaugePtr(pGauge);
}

//=============================================================================
// 3Dゲージの背景を生成
//=============================================================================
void CGauge3D::CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//背景の生成
	CObject* pGaugeBG = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (pGaugeBG != nullptr) pGaugeBG->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
	//ゲージのポインタを設定
	SetGaugeBGPtr(pGaugeBG);
}

//=============================================================================
// 3Dゲージの枠を生成
//=============================================================================
void CGauge3D::CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	//背景の生成
	CObject* pGaugeFrame = CBillboard::Create(pos, typeTex, fWidth, fHeight);
	if (pGaugeFrame != nullptr) pGaugeFrame->SetDrawPriority(CObject::DRAW_PRIORITY::BILLBOARD_UI);
	//ゲージのポインタを設定
	SetGaugeFramePtr(pGaugeFrame);
}