//=============================================================================
//
// タイトルシーン処理 [titleScene.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "titleScene.h"
#include "manager.h"
#include "renderer.h"
#include "titleCamera.h"
#include "sound.h"
#include "input.h"
#include "fade.h"
#include "object2D.h"
#include "selectMenu2D.h"
#include "objectModel.h"
#include "glowApple.h"

#include "meshwall.h"
#include "player.h"
#include "appleTree.h"
//=============================================================================
// マクロ定義
//=============================================================================
#define FOG_COLOR (D3DXCOLOR(0.9f, 0.5f, 0.0f, 1.0f))	//フォグの色
#define TEXT_FILE_NAME_APPLETYPE "data/TEXT/save_appletype.txt"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitleScene::CTitleScene()
{
	m_pSelectMenuTitle = nullptr;

	m_pTutorial = nullptr;
	m_pCreadit = nullptr;
	m_bBeginFade = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CTitleScene::~CTitleScene()
{

}

//=============================================================================
// タイトルシーンの初期化処理
//=============================================================================
void CTitleScene::Init(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//カメラの設定
	if (pManager != nullptr) pManager->SetCamera(CTitleCamera::Create());

	//------------------------------
	//ライトの初期設定
	//------------------------------
	D3DXMATRIX mtxLightProj;   // ライトの射影変換
	//ライトのプロジェクションマトリックスを生成
	D3DXMatrixPerspectiveFovLH(&mtxLightProj, D3DXToRadian(45.0f), 1.0f, 200.0f, 1800.0f);

	D3DXMATRIX mtxLightView;   // ライトビュー変換
	D3DXVECTOR3 posLight = D3DXVECTOR3(0.0f, 1200.0f, -1000.0f);	//ライトの位置
	D3DXVECTOR3 vecLight;	//ライトのベクトル
	//正規化する
	D3DXVec3Normalize(&vecLight, &posLight);
	//逆向きにする
	vecLight *= -1;
	//地上の位置に合わせる
	posLight.y += APPLETREE_POS_Y;

	//ライトのビューマトリックスを生成
	D3DXMatrixLookAtLH(&mtxLightView, &posLight, &D3DXVECTOR3(posLight + vecLight), &D3DXVECTOR3(0, 1, 0));
	//シェーダのライトを設定
	if (pRenderer != nullptr) {
		pRenderer->SetEffectLightMatrixView(mtxLightView);
		pRenderer->SetEffectLightVector(D3DXVECTOR4(vecLight, 1.0f));
		pRenderer->SetEffectLightMatrixProj(mtxLightProj);
	}

	//------------------------------
	//フォグの初期設定
	//------------------------------
	if (pRenderer != nullptr) {
		pRenderer->SetEffectFogEnable(true);
		pRenderer->SetEffectFogColor(FOG_COLOR);
		pRenderer->SetEffectFogRange(200.0f, 4000.0f);
		//バックバッファをフォグの色に合わせる
		pRenderer->SetBackBuffColor(FOG_COLOR);
	}

	//------------------------------
	//モーション情報のロード
	//------------------------------
	CPlayer::LoadMotionInfo();

	//------------------------------
	//3Dモデルの生成
	//------------------------------
	//地形
	CObjectModel::Create(CModel::MODELTYPE::TERRAIN, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);	
	//プレイヤー
	CPlayer::Create(D3DXVECTOR3(0.0f, APPLETREE_POS_Y, -350.0f));	
	//リンゴの木
	D3DXVECTOR3 posTree = D3DXVECTOR3(0.0f, APPLETREE_POS_Y, 0.0f);
	CObjectModel* pTree = CObjectModel::Create(CModel::MODELTYPE::OBJ_APPLE_TREE, posTree, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);
	if (pTree != nullptr) pTree->SetModelGlowColor(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));

	//リンゴの種類の取得
	int aTypeApple[MAX_NUM_CREATE_APPLE];
	memset(&aTypeApple, -1, sizeof(aTypeApple));

	FILE* pFile = fopen(TEXT_FILE_NAME_APPLETYPE, "r");
	if (pFile != nullptr) {
		//リンゴの種類を取得
		for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++) {
			fscanf(pFile, "%d", &aTypeApple[nCnt]);
		}
		//ファイルを閉じる
		fclose(pFile);
	}

	//リンゴの生成
	for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++)
	{
		if (aTypeApple[nCnt] == -1) continue;
		D3DXVECTOR3 posApple = posTree + CAppleTree::GetOffsetPosApple(nCnt);
		//リンゴの生成
		CAppleTree::CreateApple((CGlowApple::APPLE_TYPE)aTypeApple[nCnt], posApple, nullptr);
	}

	//------------------------------
	//UIの生成
	//------------------------------
	CObject2D::Create(D3DXVECTOR3(120.0f, 40.0f, 0.0f), CTexture::TEXTURE_TYPE::QUIT_GAME, 200.0f, 40.0f);
	CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f - 100.0f, 0.0f), CTexture::TEXTURE_TYPE::TEXT_TITLENAME, 800.0f, 200.0f);

	//タイトルの選択メニューの生成
	m_pSelectMenuTitle = CSelectMenu2D::Create(3, false);

	if (m_pSelectMenuTitle != nullptr) {
		//縦選択
		m_pSelectMenuTitle->SetSelectType(CSelectMenu::SELECT_TYPE::VERTICAL);
		//選択肢UIの詳細設定
		m_pSelectMenuTitle->SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH - 200.0f, 500.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_GAMESTART);
		m_pSelectMenuTitle->SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH - 160.0f, 580.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_TUTORIAL);
		m_pSelectMenuTitle->SetSelectUI(2, D3DXVECTOR3(SCREEN_WIDTH - 140.0f, 660.0f, 0.0f), 350.0f, 70.0f, CTexture::TEXTURE_TYPE::TEXT_CREDIT);
		//選択肢アイコンの生成
		m_pSelectMenuTitle->CreateSelectIcon(D3DXVECTOR3(-200.0f, 0.0f, 0.0f), 40.0f, 40.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
		m_pSelectMenuTitle->SetIconPosOffset(1, D3DXVECTOR3(-160.0f, 0.0f, 0.0f));
		m_pSelectMenuTitle->SetIconPosOffset(2, D3DXVECTOR3(-140.0f, 0.0f, 0.0f));
	}

	//BGMの再生
	if (pSound != nullptr) {
		pSound->PlaySound(CSound::SOUND_LABEL::BGM_TITLE);
		pSound->SetBGM(CSound::SOUND_LABEL::BGM_TITLE);
	}

	//オブジェクトのポーズが無いように設定
	CObject::SetUpdatePauseLevel(0);


#ifdef _DEBUG
	//Zバッファテクスチャの表示
	CObject2D* pZBuff = CObject2D::Create(D3DXVECTOR3(70.0f, 120.0f, 0.0f), CTexture::TEXTURE_TYPE::NONE, 100.0f, 100.0f);
	if (pZBuff != nullptr) {
		pZBuff->SetDrawPriority(CObject::DRAW_PRIORITY::FRONT);
		pZBuff->SetUseZBuffTexture(true);
	}
#endif

}

//=============================================================================
// タイトルシーンの終了処理
//=============================================================================
void CTitleScene::Uninit(void) {
	//モーション情報のアンロード
	CPlayer::UnloadMotionInfo();

	//シーンの終了処理
	CScene::Uninit();

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//音の停止
	if (pSound != nullptr) {
		pSound->StopSound();
	}
}

//=============================================================================
// タイトルシーンの更新処理
//=============================================================================
void CTitleScene::Update(void) {
	//シーン遷移開始後は更新なし
	if (m_bBeginFade) return;

	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ
	CFade* pFade = nullptr;		//フェードへのポインタ
	CSound* pSound = nullptr;	//サウンドへのポインタ

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//フェードの取得
		pFade = pManager->GetFade();
		//サウンドの取得
		pSound = pManager->GetSound();
	}

	if (pInput == nullptr || pFade == nullptr) return;


	//決定キーを押したとき
	if (pInput->GetTrigger(CInput::CODE::SELECT) && m_pSelectMenuTitle != nullptr) {
		//フェード中だった場合
		if (pFade->GetFade()) {
			//フェードをスキップ
			pFade->SkipFade();
		}
		//チュートリアル画面が表示されているとき
		else if (m_pTutorial != nullptr) {
			//チュートリアル画面を閉じる
			m_pTutorial->Uninit();
			m_pTutorial = nullptr;
			//選択のロックを解除
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//決定音の再生
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//クレジット画面が表示されているとき
		else if (m_pCreadit != nullptr) {
			//クレジット画面を閉じる
			m_pCreadit->Uninit();
			m_pCreadit = nullptr;
			//選択のロックを解除
			m_pSelectMenuTitle->SetLockChangeSelect(false);
			//決定音の再生
			if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
		}
		//他の画面が表示されていないとき
		else {
			//選択メニューの現在の選択肢を取得
			int nIdxSelect = m_pSelectMenuTitle->GetIdxCurSelect();
			switch (nIdxSelect)
			{
				//ゲームスタート
			case 0:
				//シーン遷移開始
				m_bBeginFade = true;
				if (pFade != nullptr) pFade->SetFade(CScene::SCENE_TYPE::GAME, 0.02f, 60);
				//決定音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
				//メニューの選択をロック
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//チュートリアル画面
			case 1:
				//チュートリアル画面の生成
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::TUTORIAL, SCREEN_WIDTH, SCREEN_HEIGHT);
				//決定音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//メニューの選択をロック
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;

				//クレジット画面
			case 2:
				//クレジット画面の生成
				m_pTutorial = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), CTexture::TEXTURE_TYPE::CREDIT, SCREEN_WIDTH, SCREEN_HEIGHT);
				//決定音の再生
				if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::TITLE_OPEN);
				//メニューの選択をロック
				m_pSelectMenuTitle->SetLockChangeSelect(true);
				break;
			}
		}
	}
}