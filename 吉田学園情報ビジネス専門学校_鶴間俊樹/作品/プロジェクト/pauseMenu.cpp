//=============================================================================
//
// ポーズメニュー処理 [pauseMenu.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "pauseMenu.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "sound.h"
#include "gameScene.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define NUM_SELECT (2)

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CPauseMenu::CPauseMenu() : CSelectMenu2D(NUM_SELECT, true)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CPauseMenu::~CPauseMenu()
{

}

//=============================================================================
// 林檎選択メニューの生成処理
//=============================================================================
CPauseMenu* CPauseMenu::Create(void) {
	CPauseMenu* pPauseMenu;
	pPauseMenu = new CPauseMenu();
	if (pPauseMenu == nullptr) return nullptr;

	//ポーズの設定
	pPauseMenu->SetPauseLevel(AddUpdatePauseLevel());	//ポーズのレベルを上げて返り値でポーズ更新可能レベルを取得し、オブジェクトのポーズレベルとして設定

	pPauseMenu->Init();

	return pPauseMenu;
}

//=============================================================================
// 林檎選択メニューの初期化処理
//=============================================================================
HRESULT CPauseMenu::Init(void) {
	//背景の設定
	SetMenuBG(CTexture::TEXTURE_TYPE::PAUSE_BG, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT);
	//縦選択
	SetSelectType(CSelectMenu::SELECT_TYPE::VERTICAL);
	//選択肢UIの詳細設定
	SetSelectUI(0, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 400.0f - 60.0f, 0.0f), 280.0f, 60.0f, CTexture::TEXTURE_TYPE::TEXT_CONTINUE);
	SetSelectUI(1, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 400.0f + 60.0f, 0.0f), 280.0f, 60.0f, CTexture::TEXTURE_TYPE::TEXT_TITLE);
	//選択肢アイコンの生成
	CreateSelectIcon(D3DXVECTOR3(-180.0f, 0.0f, 0.0f), 50.0f, 50.0f, CTexture::TEXTURE_TYPE::SELECT_ICON);
	SetIconPosOffset(1, D3DXVECTOR3(-120.0f, 0.0f, 0.0f));

	CSelectMenu2D::Init();

	return S_OK;
}

//=============================================================================
// 林檎選択メニューの終了処理
//=============================================================================
void CPauseMenu::Uninit(void) {
	CSelectMenu2D::Uninit();

	//ポーズのレベルを下げる
	//SubUpdatePauseLevel();	//ポーズメニューの選択によって分岐させるようにするためコメントアウト
}

//=============================================================================
// 林檎選択メニューの更新処理
//=============================================================================
void CPauseMenu::Update(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	//現在の入力デバイスの取得
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;
	//フェードの取得
	CFade* pFade = pManager->GetFade();		//フェードへのポインタ
	if (pFade == nullptr) return;
	//ゲームシーンの取得
	CGameScene* pGameScene = pManager->GetGameScene();		//フェードへのポインタ
	if (pGameScene == nullptr) return;
	//サウンドの取得
	CSound* pSound = pManager->GetSound();	//サウンドへのポインタ
	if (pSound == nullptr) return;

	if (pInput != nullptr) {
		//選択を入力
		if (pInput->GetTrigger(CInput::CODE::SELECT) && !GetLockChangeSelect()) {
			switch (GetIdxCurSelect())
			{
				//ゲーム再開
			case 0:
				//ゲームシーンのポーズメニューを破棄する
				pGameScene->DeletePauseMenu();
				//ポーズのレベルを下げる
				SubUpdatePauseLevel();
				//サウンドを再生
				pSound->PlaySound(CSound::SOUND_LABEL::TITLE_CLOSE);
				return;
				break;

				//タイトルに戻る
			case 1:
				//ゲームシーンのポーズメニューを破棄する
				pGameScene->DeletePauseMenu();
				//タイトルへシーン遷移
				pFade->SetFade(CScene::SCENE_TYPE::TITLE, 0.02f, 60);
				//サウンドを再生
				pSound->PlaySound(CSound::SOUND_LABEL::TITLE_START);
				break;
			}
		}
	}

	CSelectMenu2D::Update();
}