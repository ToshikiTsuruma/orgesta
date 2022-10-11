//=============================================================================
//
// 衝撃波処理 [shockWave.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "shockWave.h"
#include "manager.h"
#include "renderer.h"
#include "objectList.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define DAMAGE_SHOCKWAVE (10)	//衝撃波のダメージ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CShockwave::CShockwave() : CMeshcylinder()
{

}

//=============================================================================
//オーバーロードされたコンストラクタ
//=============================================================================
CShockwave::CShockwave(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fHeight) : CMeshcylinder(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50, fRadiusTop, fRadiusBottom, 1, fHeight, true)
{
	SetDrawPriority(DRAW_PRIORITY::EFFECT);

	m_fAddRadius = 0.0f;
	m_fAddHeight = 0.0f;
	m_fRotateSpeed = 0.0f;
	//オブジェクトリストの生成
	m_pListAttacked = new CObjectList;
}

//=============================================================================
// デストラクタ
//=============================================================================
CShockwave::~CShockwave()
{
	//オブジェクトリストの破棄
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// 衝撃波の生成処理
//=============================================================================
CShockwave* CShockwave::Create(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed) {
	CShockwave* pShockwave;
	pShockwave = new CShockwave(pos, fRadiusTop, fRadiusBottom, fHeight);
	if (pShockwave == nullptr) return nullptr;

	//衝撃波のテクスチャを設定
	pShockwave->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_SHOCKWAVE);

	pShockwave->m_nLife = nLife;
	pShockwave->m_fAddRadius = fAddRadius;
	pShockwave->m_fAddHeight = fAddHeight;
	pShockwave->m_fRotateSpeed = fRotSpeed;

	pShockwave->Init();

	return pShockwave;
}

//=============================================================================
// 衝撃波の初期化処理
//=============================================================================
HRESULT CShockwave::Init(void) {

	CMeshcylinder::Init();
	return S_OK;
}

//=============================================================================
// 衝撃波の終了処理
//=============================================================================
void CShockwave::Uninit(void) {
	CMeshcylinder::Uninit();
}

//=============================================================================
// 衝撃波の更新処理
//=============================================================================
void CShockwave::Update(void) {
	//半径を増加
	AddRadius(m_fAddRadius);

	//高さを増加
	AddHeight(m_fAddHeight);

	//回転させる
	Rotate();

	//攻撃の当たり判定
	AttackCollision(OBJTYPE_ENEMY);

	CMeshcylinder::Update();

	//ライフの減少
	m_nLife--;
	if (m_nLife <= 0) Uninit();
}

//=============================================================================
// 衝撃波の描画処理
//=============================================================================
void CShockwave::Draw(void) {
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
	//加算合成
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//描画
	CMeshcylinder::Draw();

	//カリングの設定を戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	//ライトを有効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//加算合成を戻す
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	//Zバッファの更新
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//=============================================================================
// 回転する
//=============================================================================
void CShockwave::Rotate(void) {
	SetRot(GetRot() + D3DXVECTOR3(0.0f, m_fRotateSpeed, 0.0f));
}

//=============================================================================
// 攻撃の当たり判定
//=============================================================================
void CShockwave::AttackCollision(int nObjtype) {
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
		D3DXVECTOR3 posObj = pObject->GetPos();	//オブジェクトの位置

		//攻撃可能な範囲にいるかどうか
		D3DXVECTOR2 vecObj = D3DXVECTOR2(posObj.x - GetPos().x, posObj.z - GetPos().z);	//衝撃波の位置とオブジェクトのベクトル
		float fDistObj = D3DXVec2Length(&vecObj);	//攻撃位置とオブジェクトの距離

		//衝撃波の範囲内に敵がいない場合当たり判定失敗	斥力波でジャンプすることもあるので衝撃波より上なら当たらない 埋もれた場合の微調整
		if (fDistObj > GetRadius() || posObj.y - GetPos().y > GetHeight() + 20.0f) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	//次のループ
		}

		//---------------------------
		//当たり判定時の処理
		//---------------------------
		//オブジェクトにダメージを与える
		pObject->Damage(DAMAGE_SHOCKWAVE, DAMAGE_TYPE::SHOCKWAVE, nullptr);

		//オブジェクトが死亡していない場合攻撃済みリストに追加
		if (!pObject->GetDeath()) {
			if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);
		}

		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}
}