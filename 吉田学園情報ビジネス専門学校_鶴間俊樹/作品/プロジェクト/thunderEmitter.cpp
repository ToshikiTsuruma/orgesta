//=============================================================================
//
// 落雷放出処理 [thunderEmitter.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "thunderEmitter.h"
#include "thunder.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CThunderEmitter::CThunderEmitter() : m_nNumEmitDest(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CThunderEmitter::CThunderEmitter(int nNumEmitDest) : m_nNumEmitDest(nNumEmitDest)
{
	//落雷の生成位置を動的確保
	m_pPosEmitArray = new D3DXVECTOR3[nNumEmitDest];
	m_nNumEmit = 0;
	m_nCntEmit = 0;
	m_nSpanEmit = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CThunderEmitter::~CThunderEmitter()
{
	//落雷の生成位置の破棄
	if (m_pPosEmitArray != nullptr) {
		delete[] m_pPosEmitArray;
		m_pPosEmitArray = nullptr;
	}
}

//=============================================================================
// 直進の落雷生成処理
//=============================================================================
CThunderEmitter* CThunderEmitter::CreateStraight(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, D3DXVECTOR3 vecStraight, float fDist) {
	//生成数０の場合終了
	if (nNumEmitDest <= 0) return nullptr;

	CThunderEmitter* pThunderEmitter;
	pThunderEmitter = new CThunderEmitter(nNumEmitDest);
	if (pThunderEmitter == nullptr) return nullptr;

	pThunderEmitter->m_nSpanEmit = nSpanEmit;
	pThunderEmitter->Init();

	//落雷の生成位置の設定
	if (pThunderEmitter->m_pPosEmitArray != nullptr) {
		D3DXVECTOR3 vecNor;	//生成方向の法線ベクトル
		D3DXVec3Normalize(&vecNor, &vecStraight);	//正規化

		for (int nCnt = 0; nCnt < nNumEmitDest; nCnt++)
		{
			(pThunderEmitter->m_pPosEmitArray)[nCnt] = pos + vecNor * fDist * (float)nCnt;
		}
	}

	return pThunderEmitter;
}

//=============================================================================
// 円形の落雷生成処理
//=============================================================================
CThunderEmitter* CThunderEmitter::CreateRound(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, float rotY, float fRadius) {
	//生成数０の場合終了
	if (nNumEmitDest <= 0) return nullptr;

	CThunderEmitter* pThunderEmitter;
	pThunderEmitter = new CThunderEmitter(nNumEmitDest);
	if (pThunderEmitter == nullptr) return nullptr;

	pThunderEmitter->m_nSpanEmit = nSpanEmit;
	pThunderEmitter->Init();

	//落雷の生成位置の設定
	if (pThunderEmitter->m_pPosEmitArray != nullptr) {
		for (int nCnt = 0; nCnt < nNumEmitDest; nCnt++)
		{
			(pThunderEmitter->m_pPosEmitArray)[nCnt] = pos + D3DXVECTOR3(sinf(rotY + D3DX_PI * 2 * (nCnt / (float)nNumEmitDest)) * fRadius, 0.0f, cosf(rotY + D3DX_PI * 2 * (nCnt / (float)nNumEmitDest)) * fRadius);
		}
	}

	return pThunderEmitter;
}

//=============================================================================
// 落雷放出の初期化処理
//=============================================================================
HRESULT CThunderEmitter::Init(void) {
	m_nNumEmit = 0;
	m_nCntEmit = 0;

	return S_OK;
}

//=============================================================================
// 落雷放出の終了処理
//=============================================================================
void CThunderEmitter::Uninit(void) {
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 落雷放出の更新処理
//=============================================================================
void CThunderEmitter::Update(void) {
	//生成タイミング
	if (m_nCntEmit > m_nSpanEmit) {
		m_nCntEmit = 0;

		//落雷の生成
		if(m_pPosEmitArray != nullptr) CThunder::Create(m_pPosEmitArray[m_nNumEmit]);

		//生成カウントの加算
		m_nNumEmit++;

		//生成終了
		if (m_nNumEmit >= m_nNumEmitDest) {
			Uninit();
			return;
		}
	}
	else {
		m_nCntEmit++;
	}
}