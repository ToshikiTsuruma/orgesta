//=============================================================================
//
// UIモデルオブジェクト処理 [objectModelUI.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "objectModelUI.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "camera.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObjectModelUI::CObjectModelUI()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CObjectModelUI::CObjectModelUI(CModel::MODELTYPE typeModel, bool bOutLine) : CObjectModel(typeModel, bOutLine)
{
	//描画順の設定
	SetDrawPriority(DRAW_PRIORITY::UI);

	D3DXMatrixIdentity(&m_mtxView);
	m_rotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectModelUI::~CObjectModelUI()
{

}

//=============================================================================
// UIモデルオブジェクトの生成処理
//=============================================================================
CObjectModelUI* CObjectModelUI::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) {
	CObjectModelUI* pObjectModelUI;
	pObjectModelUI = new CObjectModelUI(type, bOutLine);
	if (pObjectModelUI == nullptr) return nullptr;

	pObjectModelUI->SetPos(pos);
	pObjectModelUI->SetRot(rot);
	pObjectModelUI->Init();

	return pObjectModelUI;
}

//=============================================================================
// UIモデルオブジェクトの初期化処理
//=============================================================================
HRESULT CObjectModelUI::Init(void) {
	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// UIモデルオブジェクトの終了処理
//=============================================================================
void CObjectModelUI::Uninit(void) {
	CObjectModel::Uninit();
}

//=============================================================================
// UIモデルオブジェクトの更新処理
//=============================================================================
void CObjectModelUI::Update(void) {
	//角度を取得
	D3DXVECTOR3 rot = GetRot();
	//回転させる
	rot += m_rotSpeed;

	//π超過時の調節
	if (rot.x > D3DX_PI) {
		rot.x -= D3DX_PI * 2;
	}
	if (rot.x < 0.0f) {
		rot.x += D3DX_PI * 2;
	}
	if (rot.y > D3DX_PI) {
		rot.y -= D3DX_PI * 2;
	}
	if (rot.y < 0.0f) {
		rot.y += D3DX_PI * 2;
	}
	if (rot.z > D3DX_PI) {
		rot.z -= D3DX_PI * 2;
	}
	if (rot.z < 0.0f) {
		rot.z += D3DX_PI * 2;
	}

	//角度を設定
	SetRot(rot);

	CObjectModel::Update();
}

//=============================================================================
// UIモデルオブジェクトの描画処理
//=============================================================================
void CObjectModelUI::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;
	//カメラの取得
	CCamera* pCamera = pManager->GetCamera();
	if (pCamera == nullptr) return;

	//Zテクスチャに書き込まない
	if (pRenderer->GetDrawZTex()) return;

	D3DXMATRIX mtxViewDef;	//通常のビューマトリックス
	pDevice->GetTransform(D3DTS_VIEW, &mtxViewDef);	//通常のビューマトリックスを取得
	D3DXVECTOR4 posVDef = pCamera->GetPosV();	//通常のカメラ視点を取得

	//フォグ無効
	pRenderer->SetEffectFogEnable(false);
	//ビューマトリックスの変更
	pRenderer->SetEffectMatrixView(m_mtxView);
	//視点を変更
	pRenderer->SetEffectPosView((D3DXVECTOR4)m_posV);

	//描画
	CObjectModel::Draw();

	//フォグ有効
	pRenderer->SetEffectFogEnable(true);
	//ビューマトリックスを戻す
	pRenderer->SetEffectMatrixView(mtxViewDef);
	//視点の設定を戻す
	pRenderer->SetEffectPosView(posVDef);
}