//=============================================================================
//
// 落雷処理 [thunder.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "thunder.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "objectList.h"
#include "terrain.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define THUNDER_WIDTH (400.0f)
#define THUNDER_HEIGHT (1000.0f)

#define DAMAGE_THUNDER (40)	//落雷のダメージ
#define COLLISION_RADIUS (120.0f)	//当たり判定の半径

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CThunder::CThunder() : CEffect(EFFECT_TYPE::THUNDERBOLT, false)
{
	//オブジェクトリストの生成
	m_pListAttacked = new CObjectList;
}

//=============================================================================
// デストラクタ
//=============================================================================
CThunder::~CThunder()
{
	//オブジェクトリストの破棄
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// 落雷の生成処理
//=============================================================================
CThunder* CThunder::Create(D3DXVECTOR3 pos) {
	CThunder* pThunder;
	pThunder = new CThunder();
	if (pThunder == nullptr) return nullptr;

	pThunder->SetPos(pos);
	pThunder->SetSize(D3DXVECTOR3(THUNDER_WIDTH, THUNDER_HEIGHT, 0.0f));

	pThunder->Init();

	return pThunder;
}

//=============================================================================
// 落雷の初期化処理
//=============================================================================
HRESULT CThunder::Init(void) {
	//地面との当たり判定
	bool bLand = false;	//接地しているかどうか
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = GetPos();	//敵の位置を取得
	vecStart.y = 5000.0f;	//地形に当たるように適当な高さ
	vecEnd = vecStart;
	vecEnd.y += -1.0f;	//下向きのベクトル

	bLand = CTerrain::Collision(posColTerrain, vecStart, vecEnd);
	//接地時
	if (bLand) {
		SetPos(posColTerrain + D3DXVECTOR3(0.0f, (THUNDER_HEIGHT * 0.7f) / 2.0f, 0.0f));	//位置の移動 ビルボードの中心がposなので下が地面に来るように調整する
	}

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//落雷音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::THUNDERBOLT);

	CEffect::Init();
	return S_OK;
}

//=============================================================================
// 落雷の終了処理
//=============================================================================
void CThunder::Uninit(void) {
	CEffect::Uninit();
}

//=============================================================================
// 落雷の更新処理
//=============================================================================
void CThunder::Update(void) {
	//攻撃の当たり判定
	AttackCollision(OBJTYPE_ENEMY | OBJTYPE_APPLETREE | OBJTYPE_SCAPEGOAT);

	CEffect::Update();
}

//=============================================================================
// 落雷の描画処理
//=============================================================================
void CThunder::Draw(void) {
	//描画
	CEffect::Draw();
}

//=============================================================================
// 攻撃の当たり判定
//=============================================================================
void CThunder::AttackCollision(int nObjtype) {
	CObject* pObject = GetObjectTopAll();	//全オブジェクトのリストの先頭を取得

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//リストの次のオブジェクトのポインタを取得

		//オブジェクトタイプの確認
		bool bMatchType = false;
		if (pObject->GetObjType() & nObjtype) bMatchType = true;

		//すでに攻撃が当たっている場合
		bool bAttacked = false;	//すでに攻撃されているかどうか

		if (m_pListAttacked != nullptr) {
			//リストにすでに追加されている場合
			bAttacked = m_pListAttacked->MatchObject(pObject);
		}

		bool bDeath = pObject->GetDeath();
		bool bEnableCollision = pObject->GetEnableCollision();

		//次のループに飛ばす
		if (!bMatchType || bAttacked || bDeath || !bEnableCollision) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;
		}

		//---------------------------
		//当たり判定
		//---------------------------
		//攻撃可能な範囲にいるかどうか
		D3DXVECTOR2 vecObj = D3DXVECTOR2(pObject->GetPos().x - GetPos().x, pObject->GetPos().z - GetPos().z);	//衝撃波の位置とオブジェクトのベクトル
		float fDistObj = D3DXVec2Length(&vecObj);	//攻撃位置とオブジェクトの距離

		//落雷の範囲内に敵がいない場合当たり判定失敗
		if (fDistObj > COLLISION_RADIUS) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//次のループ
		}

		//---------------------------
		//当たり判定時の処理
		//---------------------------
		//オブジェクトにダメージを与える
		pObject->Damage(DAMAGE_THUNDER, DAMAGE_TYPE::THUNDERBOLT, nullptr);

		//マネージャーの取得
		CManager* pManager = CManager::GetManager();
		//サウンドの取得
		CSound *pSound = nullptr;
		if (pManager != nullptr) pSound = pManager->GetSound();

		//ダメージ音の再生
		//if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);

		//オブジェクトが死亡していない場合攻撃済みリストに追加
		if (!pObject->GetDeath()) {
			if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);
		}

		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}
}