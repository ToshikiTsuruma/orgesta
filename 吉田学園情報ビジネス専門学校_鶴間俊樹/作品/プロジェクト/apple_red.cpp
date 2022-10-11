//=============================================================================
//
// 赤林檎処理 [apple_red.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_red.h"
#include "appleTree.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define ADD_TREE_LIFE (MAX_LIFE_APPLETREE / 4)	//体力増加量

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleRed::CAppleRed()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleRed::CAppleRed(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleRed::~CAppleRed()
{

}

//=============================================================================
// 赤林檎の生成処理
//=============================================================================
CAppleRed* CAppleRed::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//ポインタがリンゴの木ではない場合null
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleRed* pAppleRed = new CAppleRed(pTree);
	if (pAppleRed == nullptr) return nullptr;

	pAppleRed->SetPos(pos);
	pAppleRed->Init();

	//林檎の木の体力の上限値を上げる
	CAppleTree* pAppleTree = pAppleRed->GetAppleTree();
	if (pAppleTree != nullptr) {
		pAppleTree->AddMaxLife(ADD_TREE_LIFE);
		pAppleTree->Heal(ADD_TREE_LIFE);
	}

	return pAppleRed;
}

//=============================================================================
// 赤林檎の初期化処理
//=============================================================================
HRESULT CAppleRed::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::RED), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::RED);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 赤林檎の終了処理
//=============================================================================
void CAppleRed::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 赤林檎の更新処理
//=============================================================================
void CAppleRed::Update(void) {
	CGlowApple::Update();
}

//=============================================================================
// 赤林檎の描画処理
//=============================================================================
void CAppleRed::Draw(void) {
	CGlowApple::Draw();
}