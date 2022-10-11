//=============================================================================
//
// グローアップル処理 [glowApple.cpp]
// Author : 鶴間俊樹
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
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGlowApple::CGlowApple()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CGlowApple::CGlowApple(CAppleTree* pTree) : CObjectModel(CModel::MODELTYPE::OBJ_APPLE, false)
{
	SetObjType(OBJTYPE_APPLE);
	m_pAppleTree = pTree;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGlowApple::~CGlowApple()
{

}

//=============================================================================
// グローアップルの初期化処理
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
// グローアップルの終了処理
//=============================================================================
void CGlowApple::Uninit(void) {
	//発光表現用パーティクルの破棄
	if (m_pGlowParticle != nullptr) {
		m_pGlowParticle->Uninit();
		m_pGlowParticle = nullptr;
	}

	CObjectModel::Uninit();
}

//=============================================================================
// グローアップルの更新処理
//=============================================================================
void CGlowApple::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// グローアップルの描画処理
//=============================================================================
void CGlowApple::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// 死亡
//=============================================================================
void CGlowApple::Dead(void) {
	//死亡エフェクト
	//CEffect::Create(GetPos(), CEffect::EFFECT_TYPE::DAMAGE_ENEMY, 50.0f, 50.0f, false);
	//終了処理
	Uninit();
}

//=============================================================================
// 発光表現用パーティクルの生成
//=============================================================================
void CGlowApple::CreateGlowParticle(APPLE_TYPE type) {
	//すでに生成されている場合終了
	if (m_pGlowParticle != nullptr) return;

	//中心位置の取得
	D3DXVECTOR3 posGlow = GetPos();
	posGlow.y -= 18.0f;	//位置調整

	//色の決定
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

	//パーティクルの生成
	m_pGlowParticle = CParticle::Create(posGlow, -1, 32.0f, 0.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), colGlow, colGlow);
}

//=============================================================================
// 種類ごとの林檎の色を取得
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
//種類ごとの林檎の説明テキスト
//=============================================================================
void CGlowApple::GetAppleText(char* pStr, APPLE_TYPE type) {
	switch (type)
	{
	case CGlowApple::APPLE_TYPE::RED:
		strcpy(pStr, "Red Apple : 体力の最大値が増える");
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		strcpy(pStr, "Green Apple : 数秒ごとに回復する");
		break;
	case CGlowApple::APPLE_TYPE::WHITE:
		strcpy(pStr, "White Apple :  攻撃時の衝撃波の数が増える");
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		strcpy(pStr, "Black Apple : 木の中心から斥力波を放つ");
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		strcpy(pStr, "Silver Apple : 攻撃時の落雷の数が増える");
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		strcpy(pStr, "Gold Apple : 一番近くの敵にリンゴ弾を撃つ");
		break;
	}
}