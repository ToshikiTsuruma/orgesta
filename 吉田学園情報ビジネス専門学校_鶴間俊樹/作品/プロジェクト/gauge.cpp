//=============================================================================
//
// ゲージ処理 [gauge.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "gauge.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGauge::CGauge() : m_bVertical(false), m_nMaxValueDefault(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CGauge::CGauge(int nMaxValue, bool bVertical) : m_bVertical(bVertical), m_nMaxValueDefault(nMaxValue)
{
	m_nMaxValue = nMaxValue;
	m_nValue = 0;
	m_nDrawLifeMax = 0;
	m_nCntDrawLife = 0;
	m_bExtend = false;

	m_colGaugeDefault = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_colGaugeDanger = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nDangerValue = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGauge::~CGauge()
{

}

//=============================================================================
// ゲージの初期化処理
//=============================================================================
HRESULT CGauge::Init(void) {
	SetGaugeValue(0);
	m_nCntDrawLife = 0;

	return S_OK;
}

//=============================================================================
// ゲージの終了処理
//=============================================================================
void CGauge::Uninit(void) {
	//ゲージの破棄
	if (m_pGauge != nullptr) {
		m_pGauge->Uninit();
		m_pGauge = nullptr;
	}
	//ゲージの背景の破棄
	if (m_pGaugeBG != nullptr) {
		m_pGaugeBG->Uninit();
		m_pGaugeBG = nullptr;
	}
	//ゲージの枠の破棄
	if (m_pGaugeFrame != nullptr) {
		m_pGaugeFrame->Uninit();
		m_pGaugeFrame = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ゲージの更新処理
//=============================================================================
void CGauge::Update(void) {
	if (m_nCntDrawLife > 0) {
		//描画する時間を減らす
		m_nCntDrawLife--;
		//描画時間が終了した場合
		if (m_nCntDrawLife == 0) {
			//ゲージを非表示
			SetGaugeDraw(false);
		}
	}
}

//=============================================================================
// ゲージの値を追加
//=============================================================================
void CGauge::AddGaugeValue(int nAddValue) {
	SetGaugeValue(m_nValue + nAddValue);
}

//=============================================================================
// ゲージの値を設定
//=============================================================================
void CGauge::SetGaugeValue(int nValue) {
	m_nValue = nValue;

	//上限・下限超過時の調整
	if (m_nValue < 0) m_nValue = 0;
	if (m_nValue > m_nMaxValue) m_nValue = m_nMaxValue;

	m_nCntDrawLife = m_nDrawLifeMax;	//描画時間を最大値に設定

	//ゲージを表示
	SetGaugeDraw(true);

	if (m_pGauge == nullptr) return;

	if (m_nValue <= m_nDangerValue) {
		//危険時の色に変更
		m_pGauge->SetColor(m_colGaugeDanger);
	}
	else {
		//通常色へ変更
		m_pGauge->SetColor(m_colGaugeDefault);
	}

	//ゲージを最大値に合わせて拡張する場合
	if (m_bExtend) {
		//ゲージの割合を設定
		if (m_bVertical) {
			m_pGauge->SetRatioHeight((float)m_nValue / (float)m_nMaxValueDefault);
		}
		else {
			m_pGauge->SetRatioWidth((float)m_nValue / (float)m_nMaxValueDefault);
		}
	}
	else {
		//ゲージの割合を設定
		if (m_bVertical) {
			m_pGauge->SetRatioHeight((float)m_nValue / (float)m_nMaxValue);
		}
		else
		{
			m_pGauge->SetRatioWidth((float)m_nValue / (float)m_nMaxValue);
		}
	}
}

//=============================================================================
// ゲージの最大値を設定
//=============================================================================
void CGauge::SetMaxValue(int nValue) {
	//最大値を設定
	m_nMaxValue = nValue;

	if (m_pGauge == nullptr) return;

	//ゲージを最大値に合わせて拡張する場合
	if (m_bExtend) {
		if (m_bVertical) {
			//ゲージの割合を設定
			m_pGauge->SetRatioHeight((float)m_nValue / (float)m_nMaxValueDefault);
			//ゲージの背景の割合を設定
			m_pGaugeBG->SetRatioHeight((float)m_nMaxValue / (float)m_nMaxValueDefault);
			//ゲージの枠の割合を設定
			m_pGaugeFrame->SetRatioHeight((float)m_nMaxValue / (float)m_nMaxValueDefault);
		}
		else {
			//ゲージの割合を設定
			m_pGauge->SetRatioWidth((float)m_nValue / (float)m_nMaxValueDefault);
			//ゲージの背景の割合を設定
			m_pGaugeBG->SetRatioWidth((float)m_nMaxValue / (float)m_nMaxValueDefault);
			//ゲージの枠の割合を設定
			m_pGaugeFrame->SetRatioWidth((float)m_nMaxValue / (float)m_nMaxValueDefault);
		}
	}
}

//=============================================================================
// ゲージの描画状態の設定
//=============================================================================
void CGauge::SetGaugeDraw(bool bDraw) {
	//ゲージの表示切り替え
	if (m_pGauge != nullptr) m_pGauge->SetDraw(bDraw);
	//ゲージの背景の表示切り替え
	if (m_pGaugeBG != nullptr) m_pGaugeBG->SetDraw(bDraw);
	//ゲージの枠の表示切り替え
	if (m_pGaugeFrame != nullptr) m_pGaugeFrame->SetDraw(bDraw);
}

//=============================================================================
// ゲージのすべての位置の設定
//=============================================================================
void CGauge::SetAllGaugePos(D3DXVECTOR3 pos) {
	if (m_pGauge != nullptr) m_pGauge->SetPos(pos);
	if (m_pGaugeBG != nullptr) m_pGaugeBG->SetPos(pos);
	if (m_pGaugeFrame != nullptr) m_pGaugeFrame->SetPos(pos);
}

//=============================================================================
// ゲージの位置の設定
//=============================================================================
void CGauge::SetGaugePos(D3DXVECTOR3 pos) {
	if (m_pGauge == nullptr) return;
	m_pGauge->SetPos(pos);
}

//=============================================================================
// ゲージの背景の位置の設定
//=============================================================================
void CGauge::SetGaugeBGPos(D3DXVECTOR3 pos) {
	if (m_pGaugeBG == nullptr) return;
	m_pGaugeBG->SetPos(pos);
}

//=============================================================================
// ゲージの枠の位置の設定
//=============================================================================
void CGauge::SetGaugeFramePos(D3DXVECTOR3 pos) {
	if (m_pGaugeFrame == nullptr) return;
	m_pGaugeFrame->SetPos(pos);
}

//=============================================================================
// ゲージの色の設定
//=============================================================================
void CGauge::SetGaugeColor(D3DXCOLOR col) {
	if (m_pGauge == nullptr) return;
	m_pGauge->SetColor(col);
	m_colGaugeDefault = col;
}

//=============================================================================
// ゲージの背景の色の設定
//=============================================================================
void CGauge::SetGaugeBGColor(D3DXCOLOR col) {
	if (m_pGaugeBG == nullptr) return;
	m_pGaugeBG->SetColor(col);
}

//=============================================================================
// ゲージの枠の色の設定
//=============================================================================
void CGauge::SetGaugeFrameColor(D3DXCOLOR col) {
	if (m_pGaugeFrame == nullptr) return;
	m_pGaugeFrame->SetColor(col);
}

//=============================================================================
//ゲージのポインタを設定
//=============================================================================
void CGauge::SetGaugePtr(CObject* pGauge) {
	//すでに設定されている場合破棄
	if (m_pGauge != nullptr) {
		m_pGauge->Uninit();
	}

	m_pGauge = pGauge;
}

//=============================================================================
//ゲージの背景のポインタを設定
//=============================================================================
void CGauge::SetGaugeBGPtr(CObject* pGaugeBG) {
	//すでに設定されている場合破棄
	if (m_pGaugeBG != nullptr) {
		m_pGaugeBG->Uninit();
	}

	m_pGaugeBG = pGaugeBG;
}

//=============================================================================
//ゲージの枠のポインタを設定
//=============================================================================
void CGauge::SetGaugeFramePtr(CObject* pGaugeFrame) {
	//すでに設定されている場合破棄
	if (m_pGaugeFrame != nullptr) {
		m_pGaugeFrame->Uninit();
	}

	m_pGaugeFrame = pGaugeFrame;
}