//=============================================================================
//
// 黒林檎処理 [apple_black.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_black.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "repulsiveWave.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define EMIT_SPAN (FPS * 40)	//斥力波放出のスパン

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleBlack::CAppleBlack()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleBlack::CAppleBlack(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleBlack::~CAppleBlack()
{

}

//=============================================================================
// 黒林檎の生成処理
//=============================================================================
CAppleBlack* CAppleBlack::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//ポインタがリンゴの木ではない場合null
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleBlack* pAppleBlack = new CAppleBlack(pTree);
	if (pAppleBlack == nullptr) return nullptr;

	pAppleBlack->SetPos(pos);
	pAppleBlack->Init();
	pAppleBlack->m_nCntEmit = EMIT_SPAN;

	return pAppleBlack;
}

//=============================================================================
// 黒林檎の初期化処理
//=============================================================================
HRESULT CAppleBlack::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::BLACK), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::BLACK);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 黒林檎の終了処理
//=============================================================================
void CAppleBlack::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 黒林檎の更新処理
//=============================================================================
void CAppleBlack::Update(void) {
	CAppleTree* pAppleTree = GetAppleTree();	//リンゴの木の取得

	//カウントが達していて、木が存在している場合
	if (m_nCntEmit >= EMIT_SPAN && pAppleTree != nullptr) {
		//斥力波を放出する
		CRepulsivewave::Create(pAppleTree->GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), 0.0f, 120.0f, 25.0f, 25, 0.2f);

		//林檎の位置から放出のエフェクトを出す
		D3DXVECTOR3 posEffect = GetPos();
		posEffect.y -= 15.0f;	//位置の調整
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::REPULSIVEWAVE, 80.0f, 80.0f, false);

		//マネージャーの取得
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//サウンドへのポインタ
		//サウンドの取得
		if (pManager != nullptr) pSound = pManager->GetSound();
		//放出音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::REPULSIVE_WAVE);

		m_nCntEmit = 0;
	}
	else {
		m_nCntEmit++;
	}

	CGlowApple::Update();
}

//=============================================================================
// 黒林檎の描画処理
//=============================================================================
void CAppleBlack::Draw(void) {
	CGlowApple::Draw();
}