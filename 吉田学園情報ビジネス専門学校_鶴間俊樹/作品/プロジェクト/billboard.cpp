//=============================================================================
//
// ビルボード処理 [billboard.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "billboard.h"
#include "manager.h"
#include "renderer.h"

#include "camera.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
#define MAX_DRAW_DISTANCE_DEFAULT (2000.0f)	//描画可能な最大の距離のデフォルト値

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBillboard::CBillboard()
{
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	if (pDevice != nullptr) {
		//頂点バッファの生成
		if (m_pVtxBuff == NULL) {
			pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4,
				D3DUSAGE_WRITEONLY,
				0,
				D3DPOOL_MANAGED,
				&m_pVtxBuff,
				NULL);
		}
	}

	SetDrawPriority(DRAW_PRIORITY::EFFECT);	//描画順の設定

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_fWidth = 0.0f;
	m_fRatioWidth = 1.0f;
	m_fHeight = 0.0f;
	m_fRatioHeight = 1.0f;
	m_offsetPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bRotateYOnly = false;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_bEnableFog = false;
	m_bZtestAlways = true;
	m_bZwriteEnable = false;
	m_bDrawAllDist = false;
	m_fDistDrawMax = MAX_DRAW_DISTANCE_DEFAULT;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBillboard::~CBillboard()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CBillboard* CBillboard::Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight) {
	CBillboard* pBillboard;
	pBillboard = new CBillboard;
	if (pBillboard == nullptr) return nullptr;

	pBillboard->m_pos = pos;
	pBillboard->SetTexType(type);
	pBillboard->m_fWidth = fWidth;
	pBillboard->m_fHeight = fHeight;
	pBillboard->Init();

	return pBillboard;
}

//=============================================================================
// ビルボードの初期化処理
//=============================================================================
HRESULT CBillboard::Init(void) {
	//頂点バッファの設定
	if (m_pVtxBuff != nullptr) {
		VERTEX_3D *pVtx;	//頂点バッファのポインタ
		//頂点バッファをロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		//頂点の設定

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f);

		//法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}

	return S_OK;
}

//=============================================================================
// ビルボードの終了処理
//=============================================================================
void CBillboard::Uninit(void) {
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ビルボードの更新処理
//=============================================================================
void CBillboard::Update(void) {
	m_pos += m_move;
}

//=============================================================================
// ビルボードの描画処理
//=============================================================================
void CBillboard::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;	//nullの場合終了
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;	//nullの場合終了
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;		//nullの場合終了
	//カメラの取得
	CCamera* pCamera = pManager->GetCamera();

	//ビルボードはZテクスチャに書き込まない
	if (pRenderer->GetDrawZTex()) return;

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXVECTOR3 rotCamera = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if(pCamera != nullptr) rotCamera = pCamera->GetRot();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//Y回転のみ反映させる場合
	if (m_bRotateYOnly) {
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rotCamera.y, 0.0f, 0.0f);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}
	//すべての軸の回転を反映させる場合
	else {
		//向きを反映
		D3DXMatrixRotationYawPitchRoll(&mtxRot, rotCamera.y, -rotCamera.x, 0.0f);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	}

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//シェーダにマトリックスを設定
	pRenderer->SetEffectMatrixWorld(m_mtxWorld);

	//頂点定義を設定
	pRenderer->SetVtxDecl3D();	

	//テクスチャの取得
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());
	//テクスチャの設定
	pRenderer->SetEffectTexture(pTexture);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	if (m_bZtestAlways) {
		//Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
	}
	if (!m_bZwriteEnable) {
		//Zバッファの更新
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	}

	//マテリアルの設定
	pRenderer->SetEffectMaterialDiffuse(m_col);
	pRenderer->SetEffectMaterialEmissive(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	pRenderer->SetEffectMaterialSpecular(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	pRenderer->SetEffectMaterialPower(0.0f);

	DWORD dwPassFlag = PASS_3D;	//開始するパスのフラグ
	//テクスチャがある場合フラグを追加
	if (pTexture != nullptr) dwPassFlag |= PASS_TEXTURE;

	//パスの開始
	pRenderer->BeginPassEffect(dwPassFlag);

	//ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0,
		2);

	pRenderer->EndPassEffect();//エフェクト終了

	if (m_bZtestAlways) {
		//Zテスト
		pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	}
	if (!m_bZwriteEnable) {
		//Zバッファの更新
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
}

//=============================================================================
// 位置座標の設定
//=============================================================================
void CBillboard::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// 位置座標の取得
//=============================================================================
D3DXVECTOR3 CBillboard::GetPos(void) {
	return m_pos;
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CBillboard::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// 移動量の取得
//=============================================================================
D3DXVECTOR3 CBillboard::GetMove(void) { return m_move; }

//=============================================================================
// サイズの設定
//=============================================================================
void CBillboard::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;

	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f + (m_fHeight * m_fRatioHeight), 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f + (m_fHeight * m_fRatioHeight), 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// サイズの取得
//=============================================================================
D3DXVECTOR3 CBillboard::GetSize(void) {
	return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f);
}

//=============================================================================
// 幅の割合の設定
//=============================================================================
void CBillboard::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f + (m_fHeight * m_fRatioHeight), 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f + (m_fHeight * m_fRatioHeight), 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 幅の割合の設定
//=============================================================================
void CBillboard::SetRatioHeight(float fRatio) {
	m_fRatioHeight = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f + (m_fHeight * m_fRatioHeight), 0.0f) + m_offsetPos;
		pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f + (m_fHeight * m_fRatioHeight), 0.0f) + m_offsetPos;
		pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
		pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 位置のオフセットの設定
//=============================================================================
void CBillboard::SetOffsetPos(D3DXVECTOR3 pos) {
	m_offsetPos = pos;

	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-m_fWidth / 2.0f, m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[1].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[2].pos = D3DXVECTOR3(-m_fWidth / 2.0f, -m_fHeight / 2.0f, 0.0f) + m_offsetPos;
	pVtx[3].pos = D3DXVECTOR3(-m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), -m_fHeight / 2.0f, 0.0f) + m_offsetPos;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void CBillboard::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_3D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2(startU, startV);
	pVtx[1].tex = D3DXVECTOR2(endU, startV);
	pVtx[2].tex = D3DXVECTOR2(startU, endV);
	pVtx[3].tex = D3DXVECTOR2(endU, endV);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 色の設定
//=============================================================================
void CBillboard::SetColor(D3DXCOLOR col) {
	m_col = col;
}

//=============================================================================
// フォグを有効にするかどうかの設定
//=============================================================================
void CBillboard::SetEnableFog(bool bEnable) {
	m_bEnableFog = bEnable;
}

//=============================================================================
// 深度関係なく描画できるかどうかの設定
//=============================================================================
void CBillboard::SetZtestAlways(bool bAlways) {
	m_bZtestAlways = bAlways;
}

//=============================================================================
// 深度が高いポリゴンを遮るかどうかの設定
//=============================================================================
void CBillboard::SetZwriteEnable(bool bEnable) {
	m_bZwriteEnable = bEnable;
}

//=============================================================================
// 全ての距離で描画可能かどうかの設定
//=============================================================================
void CBillboard::SetDrawAllDistance(bool bDraw) {
	m_bDrawAllDist = bDraw;
}

//=============================================================================
// 描画可能な距離の最大値の設定
//=============================================================================
void CBillboard::SetDistanceDrawMax(float fDist) {
	m_fDistDrawMax = fDist;
}