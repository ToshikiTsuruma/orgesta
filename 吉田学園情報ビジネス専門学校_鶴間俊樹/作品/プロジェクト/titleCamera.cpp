//=============================================================================
//
// タイトルカメラ処理 [titleCamera.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "titleCamera.h"
#include "manager.h"
#include "renderer.h"
#include "appleTree.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_DRAW_DISTANCE (4000.0f)		//描画可能な最大の距離
#define DEFAULT_CAMERA_DISTANCE (1000.0f)	//視点と注視点の距離

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CTitleCamera::CTitleCamera() : CCamera(MAX_DRAW_DISTANCE)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CTitleCamera::~CTitleCamera()
{

}

//=============================================================================
// タイトルカメラの生成処理
//=============================================================================
CTitleCamera* CTitleCamera::Create(void) {
	CTitleCamera* pTitleCamera;
	pTitleCamera = new CTitleCamera();
	if (pTitleCamera == nullptr) return nullptr;

	pTitleCamera->Init();

	return pTitleCamera;
}

//=============================================================================
// タイトルカメラの初期化処理
//=============================================================================
HRESULT CTitleCamera::Init(void) {
	SetPos(D3DXVECTOR3(0.0f, 200.0f + APPLETREE_POS_Y, 0.0f));
	SetRot(D3DXVECTOR3(D3DX_PI * -0.1f, D3DX_PI * 0.0f, 0.0f));
	//距離の設定
	SetDistance(DEFAULT_CAMERA_DISTANCE);

	CCamera::Init();

	//カメラのコントロール不可
	SetLockControll(true);

	return S_OK;
}

//=============================================================================
// タイトルカメラの終了処理
//=============================================================================
void CTitleCamera::Uninit(void) {
	CCamera::Uninit();
}

//=============================================================================
// タイトルカメラの更新処理
//=============================================================================
void CTitleCamera::Update(void) {
	CCamera::Update();

	D3DXVECTOR3 rot = GetRot();
	//回転させる
	rot.y += 0.005f;
	if (rot.y > D3DX_PI) {
		rot.y -= D3DX_PI * 2;
	}
	SetRot(rot);

	//------------------------------------
	//視点と注視点の設定
	//------------------------------------
	D3DXVECTOR3 pos = GetPos();
	float fDist = GetDistance();

	SetPosR(pos);
	SetPosV(D3DXVECTOR3(pos.x + sinf(rot.y) * fDist * cosf(rot.x + D3DX_PI),
		pos.y + sinf(rot.x + D3DX_PI) * fDist,
		pos.z + cosf(rot.y) * fDist * cosf(rot.x + D3DX_PI)));	//視点が中心の場合から変更したときに移動がないようにX回転にPIを足している
}