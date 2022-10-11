//=============================================================================
//
// パーティクル処理 [particle.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CParticle::CParticle() : m_nLifeDef(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CParticle::CParticle(int nLife, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd) : m_nLifeDef(nLife)
{
	//深度バッファをそのままにする
	SetZtestAlways(false);
	//描画可能な距離の最大値設定
	SetDistanceDrawMax(4000.0f);

	m_nLife = nLife;
	m_fAddSize = fAddSize;
	m_move = move;
	m_addMove = addMove;
	m_colStart = colStart;
	m_colEnd = colEnd;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CParticle* CParticle::Create(D3DXVECTOR3 pos, int nLife, float fSizeStart, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd) {
	CParticle* pParticle;
	pParticle = new CParticle(nLife, fAddSize, move, addMove, colStart, colEnd);
	if (pParticle == nullptr) return nullptr;

	pParticle->SetTexType(CTexture::TEXTURE_TYPE::EFFECT_PARTICLE);
	pParticle->SetPos(pos);
	pParticle->SetSize(D3DXVECTOR3(fSizeStart, fSizeStart, 0.0f));
	pParticle->Init();
	pParticle->SetColor(colStart);

	return pParticle;
}

//=============================================================================
// パーティクルの初期化処理
//=============================================================================
HRESULT CParticle::Init(void) {
	//ビルボードの初期化
	CBillboard::Init();

	return S_OK;
}

//=============================================================================
// パーティクルの終了処理
//=============================================================================
void CParticle::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// パーティクルの更新処理
//=============================================================================
void CParticle::Update(void) {
	//ライフが0
	if (m_nLife == 0) {
		//パーティクル破棄
		Uninit();
		return;
	}
	else {
		//ライフの減少
		m_nLife--;
	}

	//ビルボードの更新
	CBillboard::Update();

	//位置の更新
	D3DXVECTOR3 pos = GetPos();	//位置の取得
	pos += m_move;	//位置を移動
	SetPos(pos);	//位置の設定

	//移動量を追加
	m_move += m_addMove;	

	D3DXVECTOR3 size = GetSize();	//サイズの取得
	//サイズの減算
	size.x += m_fAddSize;
	size.y += m_fAddSize;
	//サイズが０未満にならないように
	if (size.x >= 0) {
		//サイズの設定
		SetSize(size);
	}

	//色の設定
	D3DXCOLOR colDelta = m_colEnd - m_colStart;	//最初と最後の色の差分
	SetColor(colDelta * ((float)(m_nLifeDef - m_nLife) / (float)m_nLifeDef) + m_colStart);
}

//=============================================================================
// パーティクルの描画処理
//=============================================================================
void CParticle::Draw(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return;

	//αブレンディングを加算合成に設定
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	//フォグを無効
	pRenderer->SetEffectFogEnable(false);	

	CBillboard::Draw();

	//フォグを有効
	pRenderer->SetEffectFogEnable(true);


	//αブレンディングを通常に戻す
	//pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}