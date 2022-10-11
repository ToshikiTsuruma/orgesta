//=============================================================================
//
// 斥力波処理 [repulsiveWave.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "repulsiveWave.h"
#include "manager.h"
#include "renderer.h"
#include "objectList.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CRepulsivewave::CRepulsivewave() : CMeshcylinder()
{

}

//=============================================================================
//オーバーロードされたコンストラクタ
//=============================================================================
CRepulsivewave::CRepulsivewave(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, fRadiusInside, fRadiusOutside, 1, 0.0f, true)
{
	SetDrawPriority(DRAW_PRIORITY::EFFECT);

	m_fAddRadius = 0.0f;
	m_fRotateSpeed = 0.0f;
	//オブジェクトリストの生成
	m_pListAttacked = new CObjectList;
}

//=============================================================================
// デストラクタ
//=============================================================================
CRepulsivewave::~CRepulsivewave()
{
	//オブジェクトリストの破棄
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// 斥力波の生成処理
//=============================================================================
CRepulsivewave* CRepulsivewave::Create(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside, float fAddRadius, int nLife, float fRotSpeed) {
	CRepulsivewave* pRepulsivewave;
	pRepulsivewave = new CRepulsivewave(pos, fRadiusInside, fRadiusOutside);
	if (pRepulsivewave == nullptr) return nullptr;

	//衝撃波のテクスチャを設定
	pRepulsivewave->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_REPULSIVEWAVE);

	pRepulsivewave->m_nLife = nLife;
	pRepulsivewave->m_fAddRadius = fAddRadius;
	pRepulsivewave->m_fRotateSpeed = fRotSpeed;

	pRepulsivewave->Init();

	return pRepulsivewave;
}

//=============================================================================
// 斥力波の初期化処理
//=============================================================================
HRESULT CRepulsivewave::Init(void) {

	CMeshcylinder::Init();
	return S_OK;
}

//=============================================================================
// 斥力波の終了処理
//=============================================================================
void CRepulsivewave::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// 斥力波の更新処理
//=============================================================================
void CRepulsivewave::Update(void) {
	//回転させる
	Rotate();

	CMeshcylinder::Update();

	//ライフが尽きた場合
	if (m_nLife <= 0) {
		//内側だけ半径を広げる
		AddRadiusTop(m_fAddRadius);
		//内側が外側より大きくなったら破棄
		if (GetRadius() <= GetRadiusTop()) Uninit();
		return;
	}

	//半径を増加
	AddRadius(m_fAddRadius);

	//攻撃の当たり判定
	AttackCollision(OBJTYPE_ENEMY);


	//ライフの減少
	m_nLife--;
}

//=============================================================================
// 斥力波の描画処理
//=============================================================================
void CRepulsivewave::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	//Zテクスチャに書き込まない
	if (pRenderer->GetDrawZTex()) return;

	//カリングなし
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//描画
	CMeshcylinder::Draw();

	//カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 回転する
//=============================================================================
void CRepulsivewave::Rotate(void) {
	SetRot(GetRot() + D3DXVECTOR3(0.0f, m_fRotateSpeed, 0.0f));
}

//=============================================================================
// 攻撃の当たり判定
//=============================================================================
void CRepulsivewave::AttackCollision(int nObjtype) {
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

		//斥力波の範囲内に敵がいない場合当たり判定失敗
		if (fDistObj > GetRadius()) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//次のループ
		}

		//---------------------------
		//当たり判定時の処理
		//---------------------------
		D3DXVec2Normalize(&vecObj, &vecObj);	//オブジェクト方向へのベクトルを正規化
		D3DXVECTOR3 moveObj = D3DXVECTOR3(vecObj.x * 4.0f, 15.0f, vecObj.y * 4.0f);	//オブジェクトの移動量
		//オブジェクトを吹き飛ばす
		pObject->SetMove(moveObj);

		//攻撃済みリストに追加
		if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);

		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}
}