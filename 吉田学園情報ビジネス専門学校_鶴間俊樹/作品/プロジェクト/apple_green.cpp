//=============================================================================
//
// 緑林檎処理 [apple_green.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_green.h"
#include "appleTree.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define HEAL_APPLETREE (50)	//リンゴの木の回復量
#define HEAL_SCAPEGOAT (20)	//生贄の回復量
#define HEAL_SPAN (FPS * 6)	//回復のスパン

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleGreen::CAppleGreen()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleGreen::CAppleGreen(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleGreen::~CAppleGreen()
{

}

//=============================================================================
// 緑林檎の生成処理
//=============================================================================
CAppleGreen* CAppleGreen::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//ポインタがリンゴの木ではない場合null
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleGreen* pAppleGreen = new CAppleGreen(pTree);
	if (pAppleGreen == nullptr) return nullptr;

	pAppleGreen->SetPos(pos);
	pAppleGreen->Init();
	pAppleGreen->m_nCntHeal = HEAL_SPAN;

	return pAppleGreen;
}

//=============================================================================
// 緑林檎の初期化処理
//=============================================================================
HRESULT CAppleGreen::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GREEN), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::GREEN);

	m_nCntHeal = 0;

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 緑林檎の終了処理
//=============================================================================
void CAppleGreen::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 緑林檎の更新処理
//=============================================================================
void CAppleGreen::Update(void) {
	CAppleTree* pAppleTree = GetAppleTree();

	//カウントが達していて、木が存在している場合
	if (m_nCntHeal >= HEAL_SPAN && pAppleTree != nullptr) {
		//林檎の木を回復する
		pAppleTree->Heal(HEAL_APPLETREE);
		//生贄の回復
		HealScapegoat();

		//林檎の位置から回復のエフェクトを出す
		D3DXVECTOR3 posEffect = GetPos();
		posEffect.y -= 15.0f;	//位置の調整
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 80.0f, 80.0f, false);
		//木の位置から回復エフェクトを出す
		posEffect = pAppleTree->GetPos();
		posEffect.y += 150.0f;	//位置の調整
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 300.0f, 300.0f, false);

		//マネージャーの取得
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//サウンドへのポインタ
		//サウンドの取得
		if (pManager != nullptr) pSound = pManager->GetSound();
		//回復量音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::HEAL);

		m_nCntHeal = 0;
	}
	else {
		m_nCntHeal++;
	}

	CGlowApple::Update();
}

//=============================================================================
// 緑林檎の描画処理
//=============================================================================
void CAppleGreen::Draw(void) {
	CGlowApple::Draw();
}

//=============================================================================
// 生贄の回復
//=============================================================================
void CAppleGreen::HealScapegoat(void) {
	CObject* pObject = GetObjectTopAll();	//全オブジェクトのリストの先頭を取得

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//リストの次のオブジェクトのポインタを取得

		//オブジェクトタイプの確認
		bool bMatchType = false;
		if (pObject->GetObjType() & OBJTYPE_SCAPEGOAT) bMatchType = true;
		//死亡状態の取得
		bool bDeath = pObject->GetDeath();

		//次のループに飛ばす
		if (!bMatchType || bDeath) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	
		}
		//生贄の回復
		pObject->Heal(HEAL_SCAPEGOAT);

		//回復エフェクトの生成
		D3DXVECTOR3 posEffect = pObject->GetPos();
		posEffect.y += 150.0f;
		CEffect::Create(posEffect, CEffect::EFFECT_TYPE::HEAL_APPLE, 150.0f, 150.0f, false);

		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}
}