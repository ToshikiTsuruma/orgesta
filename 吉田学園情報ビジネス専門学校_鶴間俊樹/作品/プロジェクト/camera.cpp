//=============================================================================
//
// カメラ処理 [camera.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "camera.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"

#include "scene.h"
#include "gameScene.h"
#include "player.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define CAMERA_ROTATE_SPEED_YAW (0.008f)	//カメラの回転速度(Y軸)
#define CAMERA_ROTATE_SPEED_PITCH (0.006f)	//カメラの回転速度(X軸)
#define MAX_ROTATE_PITCH (-0.05f)	//カメラのX軸回転の最大
#define MIN_ROTATE_PITCH (-0.18f)	//カメラのX軸回転の最低

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CCamera::CCamera() : m_fMaxDrawDist(1000.0f)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDist = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));

	m_bLockControll = false;
}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CCamera::CCamera(float fMaxDrawDist) : m_fMaxDrawDist(fMaxDrawDist)
{
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fDist = 0.0f;
	m_vecU = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxProjection);
	D3DXMatrixIdentity(&m_mtxView);
	ZeroMemory(&m_viewport, sizeof(D3DVIEWPORT9));

	m_bLockControll = false;
}

//=============================================================================
// デストラクタ
//=============================================================================
CCamera::~CCamera()
{

}

//=============================================================================
// カメラの初期化処理
//=============================================================================
HRESULT CCamera::Init(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = nullptr;
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();

	//ビューポートの設定
	m_viewport.X = (DWORD)0;
	m_viewport.Y = (DWORD)0;
	m_viewport.Width = (DWORD)SCREEN_WIDTH;
	m_viewport.Height = (DWORD)SCREEN_HEIGHT;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	//ビューポートの設定
	if (pDevice != nullptr) pDevice->SetViewport(&m_viewport);

	//プロジェクションマトリックスを作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),						//画角 360度
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	//画面比率
		10.0f,										//手前の位置
		m_fMaxDrawDist);							//奥の位置

	//プロジェクションマトリックスの設定
	pRenderer->SetEffectMatrixProj(m_mtxProjection);

	//上ベクトルの設定
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//ビューマトリックスの設定
	SetCamera();

	return S_OK;
}

//=============================================================================
// カメラの終了処理
//=============================================================================
void CCamera::Uninit(void) {

}

//=============================================================================
// カメラの更新処理
//=============================================================================
void CCamera::Update(void) {
	//移動処理
	//Move();

	//回転処理
	if(!m_bLockControll) Rotate();
}

//=============================================================================
// カメラのセット
//=============================================================================
void CCamera::SetCamera(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	//------------------------------------
	//ビューマトリックスの作成
	//------------------------------------
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_posV,
		&m_posR,
		&m_vecU);
	//ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);	//ビルボードのために残す
	//シェーダにビューマトリックスを設定
	pRenderer->SetEffectMatrixView(m_mtxView);
}

//=============================================================================
// 移動
//=============================================================================
void CCamera::Move(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//nullの場合終了
	if (pInput == nullptr) return;

	//--------------------
	//入力処理
	//--------------------
	float fMoveSpeed = 5.0f;
	if (pInput->GetPress(CInput::CODE::DASH)) {
		fMoveSpeed *= 3;
	}

	if (pInput->GetPress(CInput::CODE::MOVE_UP)) {
		//前移動
		m_pos.z += fMoveSpeed * cosf(m_rot.y);
		m_pos.x += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_DOWN)) {
		//後ろ移動
		m_pos.z -= fMoveSpeed * cosf(m_rot.y);
		m_pos.x -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_LEFT)) {
		//左移動
		m_pos.x -= fMoveSpeed * cosf(m_rot.y);
		m_pos.z += fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RIGHT)) {
		//右移動
		m_pos.x += fMoveSpeed * cosf(m_rot.y);
		m_pos.z -= fMoveSpeed * sinf(m_rot.y);
	}
	if (pInput->GetPress(CInput::CODE::MOVE_RISE)) {
		//上昇
		m_pos.y += fMoveSpeed;
	}
	if (pInput->GetPress(CInput::CODE::MOVE_FALL)) {
		//下降
		m_pos.y -= fMoveSpeed;
	}
}

//=============================================================================
// 回転
//=============================================================================
void CCamera::Rotate(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	CInput* pInput = nullptr;	//入力デバイスへのポインタ

	//現在の入力デバイスの取得
	if (pManager != nullptr) pInput = pManager->GetInputCur();
	//nullの場合終了
	if (pInput == nullptr) return;

	//--------------------
	//入力処理
	//--------------------
	if (pInput->GetPress(CInput::CODE::ROTATE_UP)) {
		//視点を下げる
		m_rot.x += CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x > MAX_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MAX_ROTATE_PITCH * D3DX_PI;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_DOWN)) {
		//視点を上げる
		m_rot.x -= CAMERA_ROTATE_SPEED_PITCH * D3DX_PI;
		if (m_rot.x < MIN_ROTATE_PITCH * D3DX_PI) {
			m_rot.x = MIN_ROTATE_PITCH * D3DX_PI;
		}
	}

	if (pInput->GetPress(CInput::CODE::ROTATE_LEFT)) {
		//視点を左回転
		m_rot.y -= CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y < -D3DX_PI) {
			m_rot.y = m_rot.y + D3DX_PI * 2;
		}
	}
	if (pInput->GetPress(CInput::CODE::ROTATE_RIGHT)) {
		//視点を右回転
		m_rot.y += CAMERA_ROTATE_SPEED_YAW * D3DX_PI;
		if (m_rot.y > D3DX_PI) {
			m_rot.y = m_rot.y - D3DX_PI * 2;
		}
	}

}