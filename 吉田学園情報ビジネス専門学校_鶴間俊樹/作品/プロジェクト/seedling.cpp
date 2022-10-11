//=============================================================================
//
// 苗木の処理 [seedling.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "seedling.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "scapegoat.h"
#include "terrain.h"
#include "particleEffect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DEFAULT_NUM_NEED_ENEMY (50)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSeedling::CSeedling() : CObjectModel(CModel::MODELTYPE::OBJ_SEEDLING, false)
{
	SetObjType(OBJTYPE_SEEDLING);	//オブジェクトタイプの設定
	m_nNumNeedEnemy = DEFAULT_NUM_NEED_ENEMY;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSeedling::~CSeedling()
{

}

//=============================================================================
// 苗木の生成処理
//=============================================================================
CSeedling* CSeedling::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot) {
	CSeedling* pSeedling;
	pSeedling = new CSeedling();
	if (pSeedling == nullptr) return nullptr;

	CTerrain::Collision(pos);
	
	pSeedling->SetPos(pos);
	pSeedling->SetRot(rot);

	pSeedling->Init();

	return pSeedling;
}

//=============================================================================
// 苗木の初期化処理
//=============================================================================
HRESULT CSeedling::Init(void) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();

	return S_OK;
}

//=============================================================================
// 苗木の終了処理
//=============================================================================
void CSeedling::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// 苗木の更新処理
//=============================================================================
void CSeedling::Update(void) {
	CObjectModel::Update();
}

//=============================================================================
// 苗木の描画処理
//=============================================================================
void CSeedling::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// 生贄を生成
//=============================================================================
void CSeedling::CreateScapegoat(void) {
	//生贄の生成
	CScapegoat::Create(GetPos(), GetRot());

	//パーティクルエフェクト
	CParticleEffect::PARTICLE_INFO particleInfo;
	particleInfo.addMove = D3DXVECTOR3(0.0f, 0.0f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -0.5f; particleInfo.fSizeStart = 30.0f; particleInfo.fSpeedMove = 5.0f; particleInfo.nLife = 60;
	//球状にパーティクルを生成
	CParticleEffect::EmitSphere(particleInfo, GetPos() + D3DXVECTOR3(0.0f, 150.0f, 0.0f), 12, 12, -0.06f);

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//サウンドへのポインタ
	//サウンドの取得
	if (pManager != nullptr) pSound = pManager->GetSound();
	//生成音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CREATE_APPLE);

	//苗木の破棄
	Uninit();
}