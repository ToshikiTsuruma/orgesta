//=============================================================================
//
// ゲームカメラ処理 [gameCamera.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "gameCamera.h"
#include "manager.h"
#include "renderer.h"
#include "gameScene.h"
#include "appleTree.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_DRAW_DISTANCE (5000.0f)		//描画可能な最大の距離
#define DEFAULT_CAMERA_DISTANCE (1500.0f)	//視点と注視点の距離

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CGameCamera::CGameCamera() : CCamera(MAX_DRAW_DISTANCE)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CGameCamera::~CGameCamera()
{

}

//=============================================================================
// ゲームカメラの生成処理
//=============================================================================
CGameCamera* CGameCamera::Create(void) {
	CGameCamera* pGameCamera;
	pGameCamera = new CGameCamera();
	if (pGameCamera == nullptr) return nullptr;

	pGameCamera->Init();

	return pGameCamera;
}

//=============================================================================
// ゲームカメラの初期化処理
//=============================================================================
HRESULT CGameCamera::Init(void) {
	SetPos(D3DXVECTOR3(0.0f, APPLETREE_POS_Y + 250.0f, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * -0.15f, D3DX_PI * -0.0f, 0.0f));
	//距離の設定
	SetDistance(DEFAULT_CAMERA_DISTANCE);

	CCamera::Init();

	return S_OK;
}

//=============================================================================
// ゲームカメラの終了処理
//=============================================================================
void CGameCamera::Uninit(void) {
	CCamera::Uninit();
}

//=============================================================================
// ゲームカメラの更新処理
//=============================================================================
void CGameCamera::Update(void) {
	CCamera::Update();

	//------------------------------------
	//プレイヤーとの位置の間を取得
	//------------------------------------
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//ゲームシーンの取得
	CGameScene* pGameScene = pManager->GetGameScene();
	//ゲームの木とプレイヤーの取得
	CObject* pAppleTree = nullptr;
	CObject* pPlayer = nullptr;
	if (pGameScene != nullptr) {
		pAppleTree = pGameScene->GetAppleTree();
		pPlayer = pGameScene->GetPlayer();
	}

	D3DXVECTOR3 posTree = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pAppleTree != nullptr) posTree = pAppleTree->GetPos();

	D3DXVECTOR3 posPlayer = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	if (pPlayer != nullptr) posPlayer = pPlayer->GetPos();

	D3DXVECTOR3 posCamera = posTree + (posPlayer - posTree) / 2.0f;
	posCamera.y = APPLETREE_POS_Y + 250.0f;	//Yは固定
	//位置の設定
	SetPos(posCamera);

	//------------------------------------
	//視点と注視点の設定
	//------------------------------------
	D3DXVECTOR3 rot = GetRot();
	float fDist = GetDistance();

	SetPosR(posCamera);
	SetPosV(posCamera + D3DXVECTOR3(sinf(rot.y) * fDist * cosf(rot.x + D3DX_PI),
		sinf(rot.x + D3DX_PI) * fDist,
		cosf(rot.y) * fDist * cosf(rot.x + D3DX_PI)));
}