//=============================================================================
//
// 2Dオブジェクト処理 [object2D.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "object2D.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObject2D::CObject2D()
{
	m_pVtxBuff = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fRatioWidth = 1.0f;
	m_fHeight = 0.0f;
	m_fRatioHeight = 1.0f;
	m_fAngle = 0.0f;
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	m_bUseZBuffTexture = false;

	SetDrawPriority(DRAW_PRIORITY::UI);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObject2D::~CObject2D()
{

}

//=============================================================================
// 2Dオブジェクトの初期設定なしの生成処理
//=============================================================================
CObject2D* CObject2D::Create(void) {
	CObject2D* pObject2D;
	pObject2D = new CObject2D;
	if (pObject2D == nullptr) return nullptr;

	pObject2D->Init();

	return pObject2D;
}

//=============================================================================
// 2Dオブジェクトの生成処理
//=============================================================================
CObject2D* CObject2D::Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight) {
	CObject2D* pObject2D;
	pObject2D = new CObject2D;
	if (pObject2D == nullptr) return nullptr;

	pObject2D->m_pos = pos;
	pObject2D->m_fWidth = fWidth;
	pObject2D->m_fHeight = fHeight;
	pObject2D->SetTexType(type);
	pObject2D->Init();

	return pObject2D;
}

//=============================================================================
// 2Dオブジェクトの初期化処理
//=============================================================================
HRESULT CObject2D::Init(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	//頂点バッファを生成する前に破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//頂点バッファの生成
	if (pDevice != nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			&m_pVtxBuff,
			nullptr);
	}

	VERTEX_2D *pVtx;	//頂点バッファ
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y - m_fHeight / 2.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);

		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

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
// 2Dオブジェクトの終了処理
//=============================================================================
void CObject2D::Uninit(void) {
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr) {
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
	//オブジェクトの破棄
   	Release();
}

//=============================================================================
// 2Dオブジェクトの更新処理
//=============================================================================
void CObject2D::Update(void) {

	//頂点バッファの更新
	//SetVtxPos();
}

//=============================================================================
// 2Dオブジェクトの描画処理
//=============================================================================
void CObject2D::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;	//nullの場合終了
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;	//nullの場合終了
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;		//nullの場合終了

	//特に影響はなさそうだが念のためZ値テクスチャには書き込まない
	if (pRenderer->GetDrawZTex()) return;

	//頂点バッファがnullの場合終了
	if (m_pVtxBuff == nullptr) return;

	//テクスチャの取得
	LPDIRECT3DTEXTURE9 pTexture = CTexture::GetTexture(GetTexType());
	//Zバッファのテクスチャを使用する場合取得
	if (m_bUseZBuffTexture) pTexture = pRenderer->GetZBuffTex();

	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));	//頂点バッファをデバイスのデータストリームに設定	

	//アルファテスト　有効／無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//アルファ値の参照値
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	//Zバッファの更新を無効　描画順次第で3Dオブジェクトを2Dオブジェクトの前面に出せるようにするため
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	//頂点定義を設定
	pRenderer->SetVtxDecl2D();
	//テクスチャの設定
	pRenderer->SetEffectTexture(pTexture);

	DWORD dwPassFlag = PASS_2D;	//開始するパスのフラグ
	//テクスチャがある場合フラグを追加
	if (pTexture != nullptr) dwPassFlag |= PASS_TEXTURE;

	pRenderer->BeginPassEffect(dwPassFlag);	//パスの開始
	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0,
		2);

	pRenderer->EndPassEffect();	//パスの終了

	//Zバッファの更新を有効
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	//アルファテスト　有効／無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//アルファ値の参照値
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);
}

//=============================================================================
// 2Dオブジェクトの位置情報の設定
//=============================================================================
void CObject2D::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
	//頂点バッファの更新
	SetVtxPos();
}

//=============================================================================
// 2Dオブジェクトの位置情報の取得
//=============================================================================
D3DXVECTOR3 CObject2D::GetPos(void) { return m_pos; }

//=============================================================================
// 2Dオブジェクトの移動量の設定
//=============================================================================
void CObject2D::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// 2Dオブジェクトの移動量の取得
//=============================================================================
D3DXVECTOR3 CObject2D::GetMove(void) { return m_move; }

//=============================================================================
// 2Dオブジェクトのサイズの設定
//=============================================================================
void CObject2D::SetSize(D3DXVECTOR3 size) {
	m_fWidth = size.x;
	m_fHeight = size.y;
	//頂点バッファの更新
	SetVtxPos();
}

//=============================================================================
// 2Dオブジェクトのサイズの取得
//=============================================================================
D3DXVECTOR3 CObject2D::GetSize(void) { return D3DXVECTOR3(m_fWidth, m_fHeight, 0.0f); }

//=============================================================================
// 2Dオブジェクトの幅の割合の設定
//=============================================================================
void CObject2D::SetRatioWidth(float fRatio) {
	m_fRatioWidth = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 2Dオブジェクトの幅の割合の設定
//=============================================================================
void CObject2D::SetRatioHeight(float fRatio) {
	m_fRatioHeight = fRatio;

	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f - (m_fHeight * m_fRatioHeight), 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f, m_pos.y + m_fHeight / 2.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x - m_fWidth / 2.0f + (m_fWidth * m_fRatioWidth), m_pos.y + m_fHeight / 2.0f, 0.0f);

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// 角度の設定
//=============================================================================
void CObject2D::SetAngle(float fAngle) {
	m_fAngle = fAngle;
	//頂点バッファの更新
	SetVtxPos();
}

//=============================================================================
// 角度の取得
//=============================================================================
float CObject2D::GetAngle(void) { return m_fAngle; }

//=============================================================================
// 各頂点の位置を更新
//=============================================================================
void CObject2D::SetVtxPos(void) {
	if (m_pVtxBuff != nullptr) {
		float fRadius = sqrtf(powf(m_fWidth / 2.0f, 2.0f) + powf(m_fHeight / 2.0f, 2.0f));	//ポリゴンの対角線の半分
		float fAngleDelta = atan2f(m_fWidth, m_fHeight);	//基準の角度からの差分（ラジアン）

		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(m_pos.x + sinf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI + fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI + fAngleDelta) * fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(m_pos.x + sinf((m_fAngle + 1.0f) * D3DX_PI - fAngleDelta) * fRadius, m_pos.y + cosf(m_fAngle * D3DX_PI - fAngleDelta) * fRadius, 0.0f);
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// カラーの設定
//=============================================================================
void CObject2D::SetColor(D3DXCOLOR col) {
	m_col = col;

	//頂点バッファの更新
	if (m_pVtxBuff != nullptr) {
		VERTEX_2D *pVtx;
		//頂点バッファのロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		// 頂点情報を設定
		pVtx[0].col = m_col;
		pVtx[1].col = m_col;
		pVtx[2].col = m_col;
		pVtx[3].col = m_col;

		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
// カラーの取得
//=============================================================================
D3DXCOLOR CObject2D::GetColor(void) { return m_col; }

//=============================================================================
// テクスチャ座標の設定
//=============================================================================
void CObject2D::SetTexPos(float startU, float startV, float endU, float endV) {
	VERTEX_2D *pVtx;
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
// 数字に対応したテクスチャ座標の設定
//=============================================================================
void CObject2D::SetTexNumber(int nNumber) {
	VERTEX_2D *pVtx;
	//頂点バッファのロック
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// 頂点情報を設定
	pVtx[0].tex = D3DXVECTOR2(nNumber / 10.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2((nNumber + 1) / 10.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(nNumber / 10.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2((nNumber + 1) / 10.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}