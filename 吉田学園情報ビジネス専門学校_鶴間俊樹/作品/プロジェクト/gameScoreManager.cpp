//=============================================================================
//
//ゲームスコアマネージャー処理 [gameScoreManager.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "gameScoreManager.h"
#include "gauge2D.h"
#include "object2D.h"
#include "score.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_TIME_COMBO_DEFAULT (10 * FPS)	//初期のコンボの最大時間

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGameScoreManager::CGameScoreManager()
{
	m_nKillCombo = 0;
	m_nTimeCombo = 0;
	m_nTimeComboMax = 0;
	m_fScoreRate = 1.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CGameScoreManager::~CGameScoreManager()
{

}

//=============================================================================
// ゲームスコアマネージャーの生成処理
//=============================================================================
CGameScoreManager* CGameScoreManager::Create(D3DXVECTOR3 pos, float fSize) {
	CGameScoreManager* pGameScoreManager;
	pGameScoreManager = new CGameScoreManager();
	if (pGameScoreManager == nullptr) return nullptr;

	pGameScoreManager->m_nKillCombo = 0;
	pGameScoreManager->m_nTimeCombo = 0;
	pGameScoreManager->m_fScoreRate = 1.0f;
	pGameScoreManager->m_pScoreRate = CObject2D::Create(pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.1f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_X1, fSize * 0.8f, fSize * 0.8f);
	pGameScoreManager->m_pTextCombo = CObject2D::Create(pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.8f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_COMBO, fSize, fSize * 0.3f);
	pGameScoreManager->m_pComboTimeGauge = CGauge2D::Create(MAX_TIME_COMBO_DEFAULT, false, 0, 0, false);
	//時間ゲージの初期設定
	if (pGameScoreManager->m_pComboTimeGauge != nullptr) {
		//背景設定
		pGameScoreManager->m_pComboTimeGauge->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.6f, 0.0f), fSize, fSize * 0.1f);
		pGameScoreManager->m_pComboTimeGauge->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		//ゲージ設定
		pGameScoreManager->m_pComboTimeGauge->CreateGauge(CTexture::TEXTURE_TYPE::GAUGE_HORIZONTAL, pos + D3DXVECTOR3(fSize * 0.5f, fSize * 0.6f, 0.0f), fSize - fSize * 0.05f, fSize * 0.1f - fSize * 0.05f);
		pGameScoreManager->m_pComboTimeGauge->SetGaugeColor(D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f));
		pGameScoreManager->m_pComboTimeGauge->SetGaugeColorDanger(D3DXCOLOR(1.0f, 0.8f, 0.0f, 1.0f));
		pGameScoreManager->m_pComboTimeGauge->SetDangerValue(FPS * 2);
		pGameScoreManager->m_pComboTimeGauge->SetGaugeValue(0);

	}
	//コンボ数
	pGameScoreManager->m_pComboScore = CScore::Create(1, CTexture::TEXTURE_TYPE::NUMBER_002, pos + D3DXVECTOR3(0.0f, fSize * 0.5f, 0.0f), fSize * 0.5f);

	pGameScoreManager->Init();

	return pGameScoreManager;
}

//=============================================================================
// ゲームスコアマネージャーの初期化処理
//=============================================================================
HRESULT CGameScoreManager::Init(void) {
	m_nKillCombo = 0;
	m_nTimeCombo = 0;
	m_nTimeComboMax = MAX_TIME_COMBO_DEFAULT;
	m_fScoreRate = 1.0f;

	return S_OK;
}

//=============================================================================
// ゲームスコアマネージャーの終了処理
//=============================================================================
void CGameScoreManager::Uninit(void) {
	//スコアの倍率表示の破棄
	if (m_pScoreRate != nullptr) {
		m_pScoreRate->Uninit();
		m_pScoreRate = nullptr;
	}
	//コンボの残り時間ゲージの破棄
	if (m_pComboTimeGauge != nullptr) {
		m_pComboTimeGauge->Uninit();
		m_pComboTimeGauge = nullptr;
	}
	//コンボ数の破棄
	if (m_pComboScore != nullptr) {
		m_pComboScore->Uninit();
		m_pComboScore = nullptr;
	}
	//コンボのテキストの破棄
	if (m_pTextCombo != nullptr) {
		m_pTextCombo->Uninit();
		m_pTextCombo = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// ゲームスコアマネージャーの更新処理
//=============================================================================
void CGameScoreManager::Update(void) {
	if (m_nTimeCombo > 0) {
		//コンボ有効時間を減らす
		m_nTimeCombo--;
		//ゲージの更新
		if (m_pComboTimeGauge != nullptr) {
			//コンボゲージの値の設定
			m_pComboTimeGauge->SetGaugeValue(m_nTimeCombo);
		}

		//コンボが途切れた場合
		if (m_nTimeCombo <= 0) {
			m_nTimeCombo = 0;
			//初期状態に戻す
			m_nTimeComboMax = MAX_TIME_COMBO_DEFAULT;
			m_nKillCombo = 0;
			m_fScoreRate = 1.0f;
			//倍率表示を１倍にする
			if (m_pScoreRate != nullptr) {
				m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X1);
			}
			//コンボ数の設定
			if (m_pComboScore != nullptr) {
				m_pComboScore->SetScore(0);
			}
		}
	}
}

//=============================================================================
// 倒したコンボの加算
//=============================================================================
void CGameScoreManager::AddKillCombo(void) {
	//コンボ追加
	m_nKillCombo++;
	//コンボ数の設定
	if (m_pComboScore != nullptr) {
		m_pComboScore->SetScore(m_nKillCombo);
	}

	//特定のコンボ数に達した時の設定
	switch (m_nKillCombo)
	{
	case 10:
		//コンボの最大有効時間を減少
		m_nTimeComboMax = 6 * FPS;
		//スコアの倍率の増加
		m_fScoreRate = 2.0f;
		//倍率表示を変更
		if (m_pScoreRate != nullptr) {
			m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X2);
		}
		break;
	case 30:
		//コンボの最大有効時間を減少
		m_nTimeComboMax = 4 * FPS;
		//スコアの倍率の増加
		m_fScoreRate = 3.0f;
		//倍率表示を変更
		if (m_pScoreRate != nullptr) {
			m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X3);
		}
		break;
	case 50:
		//コンボの最大有効時間を減少
		m_nTimeComboMax = (float)(2.5f * FPS);
		//スコアの倍率の増加
		m_fScoreRate = 5.0f;
		//倍率表示を変更
		if (m_pScoreRate != nullptr) {
			m_pScoreRate->SetTexType(CTexture::TEXTURE_TYPE::TEXT_X5);
		}
		break;
	}

	//コンボの時間を設定
	m_nTimeCombo = m_nTimeComboMax;
	//コンボゲージの設定
	if (m_pComboTimeGauge != nullptr) {
		//コンボゲージの最大値の設定
		m_pComboTimeGauge->SetMaxValue(m_nTimeComboMax);
		//コンボゲージの値の設定
		m_pComboTimeGauge->SetGaugeValue(m_nTimeCombo);
	}
}