//=============================================================================
//
// エフェクト処理 [effect.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "effect.h"
#include "manager.h"
#include "renderer.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEffect::CEffect() : m_bLoop(false)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CEffect::CEffect(EFFECT_TYPE type, bool bLoop) : m_bLoop(bLoop)
{
	m_type = type;
	m_nCntAnim = 0;
	m_nSpanAnim = 0;
	m_nPatternAnim = 0;
	m_nPatternU = 0;
	m_nPatternV = 0;
	m_bDestBlendOne = false;

	//深度関係なく描画できるようにする
	SetZtestAlways(true);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEffect::~CEffect()
{

}

//=============================================================================
// 生成処理
//=============================================================================
CEffect* CEffect::Create(D3DXVECTOR3 pos, EFFECT_TYPE type, float fWidth, float fHeight, bool bLoop) {
	CEffect* pEffect;
	pEffect = new CEffect(type, bLoop);
	if (pEffect == nullptr) return nullptr;

	pEffect->SetPos(pos);
	pEffect->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));

	pEffect->Init();

	return pEffect;
}

//=============================================================================
// エフェクトの初期化処理
//=============================================================================
HRESULT CEffect::Init(void) {
	switch (m_type)
	{
	case EFFECT_TYPE::EXPLOSION:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_EXPLOSION);
		m_nSpanAnim = 2;
		m_nPatternU = 7;
		m_nPatternV = 1;
		break;
	case EFFECT_TYPE::DAMAGE_PLAYER:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_DAMAGE_PLAYER);
		m_nSpanAnim = 1;
		m_nPatternU = 1;
		m_nPatternV = 15;
		break;
	case EFFECT_TYPE::DAMAGE_ENEMY:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_DAMAGE_ENEMY);
		m_nSpanAnim = 1;
		m_nPatternU = 13;
		m_nPatternV = 1;
		break;
	case EFFECT_TYPE::DAMAGE_BULLET:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_DAMAGE_BULLET);
		m_nSpanAnim = 1;
		m_nPatternU = 12;
		m_nPatternV = 1;
		break;
	case EFFECT_TYPE::DEATH:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_DEATH);
		m_nSpanAnim = 2;
		m_nPatternU = 8;
		m_nPatternV = 1;
		break;
	case EFFECT_TYPE::HEAL_APPLE:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_HEAL_APPLE);
		m_nSpanAnim = 1;
		m_nPatternU = 9;
		m_nPatternV = 1;
		m_bDestBlendOne = true;	//加算合成
		break;
	case EFFECT_TYPE::DRAIN:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_DRAIN_APPLE);
		m_nSpanAnim = 1;
		m_nPatternU = 10;
		m_nPatternV = 1;
		m_bDestBlendOne = true;	//加算合成
		break;
	case EFFECT_TYPE::REPULSIVEWAVE:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_REPULSIVEWAVE_APPLE);
		m_nSpanAnim = 1;
		m_nPatternU = 10;
		m_nPatternV = 1;
		m_bDestBlendOne = true;	//加算合成
		break;
	case EFFECT_TYPE::FIRE:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_FIRE);
		m_nSpanAnim = 2;
		m_nPatternU = 8;
		m_nPatternV = 1;
		break;
	case EFFECT_TYPE::FIRE_BOLL:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_FIRE_BOLL);
		m_nSpanAnim = 2;
		m_nPatternU = 5;
		m_nPatternV = 2;
		SetZtestAlways(false);	//他のオブジェクトに遮られる
		break;
	case EFFECT_TYPE::THUNDERBOLT:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_THUNDERBOLT);
		m_nSpanAnim = 1;
		m_nPatternU = 8;
		m_nPatternV = 1;
		SetZtestAlways(false);	//他のオブジェクトに遮られる
		SetRotateYOnly(true);	//Y回転のみ
		break;
	case EFFECT_TYPE::THUNDER:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_THUNDER);
		m_nSpanAnim = 3;
		m_nPatternU = 8;
		m_nPatternV = 1;
		break;
	case EFFECT_TYPE::WIND:
		SetTexType(CTexture::TEXTURE_TYPE::EFFECT_WIND);
		m_nSpanAnim = 3;
		m_nPatternU = 12;
		m_nPatternV = 1;
		break;
	}

	//メンバ変数の初期化
	m_nCntAnim = 0;
	m_nPatternAnim = 0;
	//ビルボードの初期化
	CBillboard::Init();
	//テクスチャ座標の設定
	CBillboard::SetTexPos(0.0f, 0.0f, 1.0f / (float)m_nPatternU, 1.0f / (float)m_nPatternV);

	return S_OK;
}

//=============================================================================
// エフェクトの終了処理
//=============================================================================
void CEffect::Uninit(void) {
	CBillboard::Uninit();
}

//=============================================================================
// エフェクトの更新処理
//=============================================================================
void CEffect::Update(void) {
	CBillboard::Update();

	//アニメーション切り替え
	if (m_nCntAnim >= m_nSpanAnim) {
		m_nCntAnim = 0;
		m_nPatternAnim++;

		//アニメーションを終了した場合
		if (m_nPatternAnim >= m_nPatternU * m_nPatternV) {
			if (m_bLoop) {
				//ループさせる
				m_nPatternAnim = 0;
			}
			else
			{
				//破棄して終了
				Uninit();
				return;
			}
		}

		//テクスチャ座標の設定
		CBillboard::SetTexPos(
			(1.0f / (float)m_nPatternU) * (m_nPatternAnim % m_nPatternU),
			(1.0f / (float)m_nPatternV) * (m_nPatternAnim / m_nPatternU),
			(1.0f / (float)m_nPatternU) * ((m_nPatternAnim % m_nPatternU) + 1),
			(1.0f / (float)m_nPatternV) * ((m_nPatternAnim / m_nPatternU) + 1)
		);
	}
	else {
		m_nCntAnim++;	//カウンターの加算
	}
}

//=============================================================================
// エフェクトの描画処理
//=============================================================================
void CEffect::Draw(void) {
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

	//フォグを無効
	pRenderer->SetEffectFogEnable(false);
	//アルファテスト　有効／無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	//アルファ値の参照値
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	if (m_bDestBlendOne) {
		//αブレンディングの設定
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	}

	CBillboard::Draw();

	//フォグを有効
	pRenderer->SetEffectFogEnable(true);
	//アルファテスト　有効／無効
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//アルファテスト
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	//アルファ値の参照値
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

	if (m_bDestBlendOne) {
		//αブレンディングを通常に戻す
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}
}