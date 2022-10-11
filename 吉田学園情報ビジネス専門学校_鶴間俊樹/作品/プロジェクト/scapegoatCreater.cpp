//=============================================================================
//
// 生贄生成処理 [scapegoatCreater.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "scapegoatCreater.h"
#include "manager.h"
#include "input.h"
#include "gameScene.h"
#include "player.h"
#include "seedling.h"
#include "score.h"
#include "billboard.h"
#include "object2D.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_CREATE_SCAPEGOAT_DISTANCE (200.0f);	//生贄を生成可能な最大の距離
#define DEFAULT_NUM_KILL_ENEMY (50)	//最初から持ってる敵の数

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CScapegoatCreater::CScapegoatCreater()
{
	m_nNumKillEnemy = 0;

	m_pNumKillEnemyUI = CScore::Create(4, CTexture::TEXTURE_TYPE::NUMBER_002, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 20.0f);
	m_pNumUseEnemyUI = nullptr;
	m_pInteractUI = nullptr;
	m_pNixSignUI = nullptr;
	m_pTextEnergy = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH - 130.0f, SCREEN_HEIGHT - 30.0f + 10.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_ENERGY, 80.0f, 20.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CScapegoatCreater::~CScapegoatCreater()
{

}

//=============================================================================
// 生贄生成クラスの生成処理
//=============================================================================
CScapegoatCreater* CScapegoatCreater::Create(void) {
	CScapegoatCreater* pScapegoatCreater;
	pScapegoatCreater = new CScapegoatCreater();
	if (pScapegoatCreater == nullptr) return nullptr;

	pScapegoatCreater->Init();

	return pScapegoatCreater;
}

//=============================================================================
// 生贄生成クラスの初期化処理
//=============================================================================
HRESULT CScapegoatCreater::Init(void) {
	m_nNumKillEnemy = DEFAULT_NUM_KILL_ENEMY;
	if (m_pNumKillEnemyUI != nullptr) m_pNumKillEnemyUI->SetScore(m_nNumKillEnemy);

	return S_OK;
}

//=============================================================================
// 生贄生成クラスの終了処理
//=============================================================================
void CScapegoatCreater::Uninit(void) {
	//敵を倒した数のUIの破棄
	if (m_pNumKillEnemyUI != nullptr) {
		m_pNumKillEnemyUI->Uninit();
		m_pNumKillEnemyUI = nullptr;
	}
	//敵を使用する数のUIの破棄
	if (m_pNumUseEnemyUI != nullptr) {
		m_pNumUseEnemyUI->Uninit();
		m_pNumUseEnemyUI = nullptr;
	}
	//インタラクト表示のビルボードの破棄
	if (m_pInteractUI != nullptr) {
		m_pInteractUI->Uninit();
		m_pInteractUI = nullptr;
	}
	//禁止マークのビルボードの破棄
	if (m_pNixSignUI != nullptr) {
		m_pNixSignUI->Uninit();
		m_pNixSignUI = nullptr;
	}
	//エネルギーテキストの破棄
	if (m_pTextEnergy != nullptr) {
		m_pTextEnergy->Uninit();
		m_pTextEnergy = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 生贄生成クラスの更新処理
//=============================================================================
void CScapegoatCreater::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	CInput* pInput = nullptr;
	CGameScene* pGame = nullptr;
	CPlayer* pPlayer = nullptr;

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//ゲームシーンの取得
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//プレイヤーの取得
	if (pGame != nullptr) pPlayer = pGame->GetPlayer();

	D3DXVECTOR3 posPlayer;	//プレイヤーの位置
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();
	float fDistSeedling;	//プレイヤーと苗木の距離

	D3DXVECTOR3 posSeedling;	//苗木の位置
	//一番近い苗木を取得
	CSeedling* pNearSeedling = dynamic_cast<CSeedling*>(GetNearObject(posPlayer, OBJTYPE_SEEDLING, 0, &posSeedling, &fDistSeedling, nullptr));
	if (pNearSeedling == nullptr) return;

	int nNumNeedEnemy;	//苗木から生贄を生み出す時に必要な敵の量
	if (pNearSeedling != nullptr) {
		nNumNeedEnemy = pNearSeedling->GetNumNeedEnemy();
	}

	bool bDistCreate = fDistSeedling < MAX_CREATE_SCAPEGOAT_DISTANCE;	//生成可能な距離かどうか
	bool bCanCreate = m_nNumKillEnemy >= nNumNeedEnemy;	//生成可能な量の敵を所持しているかどうか

	bool bEnableXInput = false;
	//現在の入力デバイスの種類を取得
	if (pManager != nullptr) {
		bEnableXInput = pManager->GetEnableXInput();
	}

	//アイコンテクスチャの設定
	CTexture::TEXTURE_TYPE typeTex = CTexture::TEXTURE_TYPE::ICON_INTERACT_KEYBOARD;	//キーボードのインタラクトキーのテクスチャ
	if (bEnableXInput) {
		typeTex = CTexture::TEXTURE_TYPE::ICON_INTERACT_GAMEPAD;	//ゲームパッドのインタラクトボタンのテクスチャ
	}

	//苗木の上の位置に素材の必要量をビルボードで表示
	if (bDistCreate) {
		//敵を使用する数のUIの生成
		if (m_pNumUseEnemyUI == nullptr) {
			m_pNumUseEnemyUI = CScore::Create(0, CTexture::TEXTURE_TYPE::NUMBER_006, D3DXVECTOR3(SCREEN_WIDTH - 20.0f, SCREEN_HEIGHT - 55.0f, 0.0f), 20.0f);
		}
		if (m_pNumUseEnemyUI != nullptr) {
			m_pNumUseEnemyUI->SetScore(nNumNeedEnemy);
		}

		//インタラクト表示ビルボードの生成
		if (m_pInteractUI == nullptr) {
			m_pInteractUI = CBillboard::Create(posSeedling + D3DXVECTOR3(0.0f, 150.0f, 0.0f), typeTex, 40.0f, 40.0f);
		}
		//ビルボードの更新
		if (m_pInteractUI != nullptr) {
			//テクスチャの更新
			m_pInteractUI->SetTexType(typeTex);
		}
		//生成禁止マークの生成
		if (!bCanCreate && m_pNixSignUI == nullptr) {
			m_pNixSignUI = CBillboard::Create(posSeedling + D3DXVECTOR3(0.0f, 150.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_NIX_SIGN, 70.0f, 70.0f);
			//ビルボードの初期設定
			if (m_pNixSignUI != nullptr) {
				//m_pNixSignUI->SetOffsetPos(D3DXVECTOR3(0.0f, 0.0f, -1.0f));	//少し手前	描画順的に要らなかった
			}
		}
		//禁止マークのビルボードの破棄
		if (bCanCreate && m_pNixSignUI != nullptr) {
			m_pNixSignUI->Uninit();
			m_pNixSignUI = nullptr;
		}
	}
	else {
		//敵を使用する数のUIの破棄
		if (m_pNumUseEnemyUI != nullptr) {
			m_pNumUseEnemyUI->Uninit();
			m_pNumUseEnemyUI = nullptr;
		}
		//インタラクト表示ビルボードの破棄
		if (m_pInteractUI != nullptr) {
			m_pInteractUI->Uninit();
			m_pInteractUI = nullptr;
		}
		//禁止マークのビルボードの破棄
		if (m_pNixSignUI != nullptr) {
			m_pNixSignUI->Uninit();
			m_pNixSignUI = nullptr;
		}
	}

	//入力処理
	if (pInput != nullptr) {
		//インタラクトボタン押下時
		if (pInput->GetTrigger(CInput::CODE::INTERACT) && bDistCreate && bCanCreate && pNearSeedling != nullptr) {
			//残りの素材量の更新
			m_nNumKillEnemy -= nNumNeedEnemy;
			if (m_pNumKillEnemyUI != nullptr) m_pNumKillEnemyUI->SetScore(m_nNumKillEnemy);
			//インタラクト表示ビルボードの破棄	生成時に位置を設定しているため、生成時に近くに苗木があった場合位置が更新されないため
			if (m_pInteractUI != nullptr) {
				m_pInteractUI->Uninit();
				m_pInteractUI = nullptr;
			}
			//生贄の生成
			pNearSeedling->CreateScapegoat();
			pNearSeedling = nullptr;	//生贄生成時に破棄されるため
		}
	}
}

//=============================================================================
// 倒した敵の数を増やす
//=============================================================================
void CScapegoatCreater::AddNumKillEnemy(int nNum) {
	m_nNumKillEnemy += nNum;
	if (m_pNumKillEnemyUI != nullptr) m_pNumKillEnemyUI->SetScore(m_nNumKillEnemy);
}