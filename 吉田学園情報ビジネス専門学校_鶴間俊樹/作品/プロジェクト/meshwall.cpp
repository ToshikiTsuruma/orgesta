//=============================================================================
//
// メッシュウォール処理 [meshwall.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "meshwall.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CMeshwall::CMeshwall()
{
	m_nNumX = 0;
	m_nNumY = 0;
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CMeshwall::~CMeshwall()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CMeshwall* CMeshwall::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumY, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex) {
	CMeshwall* pMeshwall;
	pMeshwall = new CMeshwall;
	if (pMeshwall == nullptr) return nullptr;

	pMeshwall->SetPos(pos);
	pMeshwall->SetRot(rot);
	pMeshwall->m_nNumX = nNumX;
	pMeshwall->m_nNumY = nNumY;
	pMeshwall->m_fWidth = fWidth;
	pMeshwall->m_fHeight = fHeight;
	pMeshwall->SetNumVtx((nNumX + 1) * (nNumY + 1));	//(X方向のポリゴン数 + 1) * (Y方向のポリゴン数 + 1)
	pMeshwall->SetNumIdx((2 * (nNumX + 1)) * nNumY + (nNumY - 1) * 2);	//X方向で使う頂点数 * Y方向のポリゴン数 + 縮退ポリゴンの数
	pMeshwall->SetTexType(typeTex);

	pMeshwall->Init();

	return pMeshwall;
}

//=============================================================================
// メッシュウォールの初期化処理
//=============================================================================
HRESULT CMeshwall::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	//頂点バッファの取得
	LPDIRECT3DVERTEXBUFFER9* ppVtxBuff = GetPtrVtxBuff();

	int nNumVtx = GetNumVtx();	//頂点数の取得
	int nNumIdx = GetNumIdx();	//インデックス数の取得

	//頂点バッファの生成
	if (*ppVtxBuff == nullptr) {
		pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * nNumVtx,
			D3DUSAGE_WRITEONLY,
			0,
			D3DPOOL_MANAGED,
			ppVtxBuff,
			nullptr);
	}

	//頂点バッファの設定
	if (*ppVtxBuff != nullptr) {
		VERTEX_3D *pVtx;	//頂点バッファのポインタ
		//頂点バッファをロック
		(*ppVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);
		//頂点の設定
		for (int nCntVtx = 0; nCntVtx < (m_nNumX + 1) * (m_nNumY + 1); nCntVtx++) {

			//頂点座標の設定
			pVtx[nCntVtx].pos = D3DXVECTOR3((nCntVtx % (m_nNumX + 1)) * m_fWidth - (m_fWidth / 2.0f) * m_nNumX,
				(nCntVtx / (m_nNumX + 1)) * -m_fHeight + m_fHeight * m_nNumY,
				0.0f);

			//法線ベクトルの設定
			pVtx[nCntVtx].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			pVtx[nCntVtx].tex = D3DXVECTOR2((float)(nCntVtx % (m_nNumX + 1)), (float)(nCntVtx / (m_nNumX + 1)));
		}
		//頂点バッファをアンロックする
		(*ppVtxBuff)->Unlock();
	}

	//インデックスバッファのの取得
	LPDIRECT3DINDEXBUFFER9* ppIdxBuff = GetPtrIdxBuff();

	//インデックスバッファの生成
	if (*ppIdxBuff == nullptr) {
		pDevice->CreateIndexBuffer(sizeof(WORD) * nNumIdx,
			D3DUSAGE_WRITEONLY,
			D3DFMT_INDEX16,
			D3DPOOL_MANAGED,
			ppIdxBuff,
			nullptr);
	}
	//インデックスバッファの設定
	if (*ppIdxBuff != nullptr) {
		WORD *pIdx;	//インデックス情報へのポインタ
		//インデックスバッファをロックし、番号データへのポインタを取得
		(*ppIdxBuff)->Lock(0, 0, (void**)&pIdx, 0);
		//番号データ設定
		for (int nCntIdx = 0; nCntIdx < (m_nNumX + 1) * 2 * m_nNumY + (m_nNumY - 1) * 2; nCntIdx++) {
			if (nCntIdx == (m_nNumX + 2) * 2 * (1 + nCntIdx / ((m_nNumX + 2) * 2)) - 2) {
				pIdx[nCntIdx] = pIdx[nCntIdx - 1];
			}
			else if (nCntIdx == (m_nNumX + 2) * 2 * (1 + nCntIdx / ((m_nNumX + 2) * 2)) - 1) {
				pIdx[nCntIdx] = (m_nNumX + 1) * (2 + nCntIdx / ((m_nNumX + 1) * 2 + 2));
			}
			else {
				pIdx[nCntIdx] = (m_nNumX + 1 + nCntIdx / 2) - (nCntIdx / ((m_nNumX + 2) * 2)) - ((m_nNumX + 1)*(nCntIdx % 2));//下　縮退　上 縮退に修正
			}
		}
		//インデックスバッファをアンロックする
		(*ppIdxBuff)->Unlock();
	}

	CObject3D::Init();

	return S_OK;
}

//=============================================================================
// メッシュウォールの終了処理
//=============================================================================
void CMeshwall::Uninit(void) {
	CObject3D::Uninit();
}

//=============================================================================
// メッシュウォールの更新処理
//=============================================================================
void CMeshwall::Update(void) {
	CObject3D::Update();
}

//=============================================================================
// メッシュウォールの描画処理
//=============================================================================
void CMeshwall::Draw(void) {
	CObject3D::Draw();
}