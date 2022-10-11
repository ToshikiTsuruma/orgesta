//=============================================================================
//
// 銀林檎処理 [apple_silver.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_silver.h"
#include "appleTree.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleSilver::CAppleSilver()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleSilver::CAppleSilver(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleSilver::~CAppleSilver()
{

}

//=============================================================================
// 銀林檎の生成処理
//=============================================================================
CAppleSilver* CAppleSilver::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//ポインタがリンゴの木ではない場合null
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleSilver* pAppleSilver = new CAppleSilver(pTree);
	if (pAppleSilver == nullptr) return nullptr;

	//雷のエフェクトを生成
	pAppleSilver->m_pEffectThunder = CEffect::Create(pos + D3DXVECTOR3(0.0f, -15.0f, 0.0f), CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, true);

	pAppleSilver->SetPos(pos);
	pAppleSilver->Init();

	return pAppleSilver;
}

//=============================================================================
// 銀林檎の初期化処理
//=============================================================================
HRESULT CAppleSilver::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::SILVER), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::SILVER);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 銀林檎の終了処理
//=============================================================================
void CAppleSilver::Uninit(void) {
	//雷のエフェクトの破棄
	if (m_pEffectThunder != nullptr) {
		m_pEffectThunder->Uninit();
		m_pEffectThunder = nullptr;
	}

	CGlowApple::Uninit();
}

//=============================================================================
// 銀林檎の更新処理
//=============================================================================
void CAppleSilver::Update(void) {

	CGlowApple::Update();
}

//=============================================================================
// 銀林檎の描画処理
//=============================================================================
void CAppleSilver::Draw(void) {
	CGlowApple::Draw();
}