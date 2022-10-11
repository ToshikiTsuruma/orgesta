//=============================================================================
//
// モデルオブジェクト処理 [objectModel.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "objectModel.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CObjectModel::CObjectModel()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CObjectModel::CObjectModel(CModel::MODELTYPE typeModel, bool bOutLine)
{
	m_pModel = CModel::Create(typeModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nullptr, bOutLine);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//=============================================================================
// デストラクタ
//=============================================================================
CObjectModel::~CObjectModel()
{

}

//=============================================================================
// モデルオブジェクトの生成処理
//=============================================================================
CObjectModel* CObjectModel::Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine) {
	CObjectModel* pObjectModel;
	pObjectModel = new CObjectModel(type, bOutLine);
	if (pObjectModel == nullptr) return nullptr;

	pObjectModel->Init();
	pObjectModel->SetPos(pos);
	pObjectModel->SetRot(rot);

	return pObjectModel;
}

//=============================================================================
// モデルオブジェクトの初期化処理
//=============================================================================
HRESULT CObjectModel::Init(void) {

	return S_OK;
}

//=============================================================================
// モデルオブジェクトの終了処理
//=============================================================================
void CObjectModel::Uninit(void) {
	if (m_pModel != nullptr) {
		m_pModel->Uninit();
		delete m_pModel;
		m_pModel = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// モデルオブジェクトの更新処理
//=============================================================================
void CObjectModel::Update(void) {
	//モデルの更新
	if (m_pModel != nullptr) {
		m_pModel->Update();
	}

	//移動処理
	Move();
	//回転処理
	Rotate();
}

//=============================================================================
// モデルオブジェクトの描画処理
//=============================================================================
void CObjectModel::Draw(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();	
	if (pManager == nullptr) return;
	//レンダラーの取得
	CRenderer* pRenderer = pManager->GetRenderer();
	if (pRenderer == nullptr) return;
	//デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();
	if (pDevice == nullptr) return;

	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-----------------------------------
	//モデルの描画
	//-----------------------------------
	if (m_pModel != nullptr) m_pModel->Draw();
}

//=============================================================================
// 移動
//=============================================================================
void CObjectModel::Move(void) {
	//位置の移動
	m_pos += m_move;
}

//=============================================================================
// 回転
//=============================================================================
void CObjectModel::Rotate(void) {
	m_rot += m_rotSpeed;

	if (m_rot.x > D3DX_PI) {
		m_rot.x += -D3DX_PI * 2;
	}
	if (m_rot.x < -D3DX_PI) {
		m_rot.x += D3DX_PI * 2;
	}
	if (m_rot.y < -D3DX_PI) {
		m_rot.y += D3DX_PI * 2;
	}
	if (m_rot.y > D3DX_PI) {
		m_rot.y += -D3DX_PI * 2;
	}
	if (m_rot.z < -D3DX_PI) {
		m_rot.z += D3DX_PI * 2;
	}
	if (m_rot.z > D3DX_PI) {
		m_rot.z += -D3DX_PI * 2;
	}
}

//=============================================================================
// モデルの種類の取得
//=============================================================================
CModel::MODELTYPE CObjectModel::GetModelType(void) {
	if (m_pModel != nullptr) return m_pModel->GetModelType();
	return (CModel::MODELTYPE) NULL;
}

//=============================================================================
// モデルの色の設定
//=============================================================================
void CObjectModel::SetModelColor(D3DXCOLOR col, int nIdx) {
	if (m_pModel == nullptr) return;
	m_pModel->SetMaterialDiffuse(col, nIdx);
}

//=============================================================================
// モデルの発光色の設定
//=============================================================================
void CObjectModel::SetModelGlowColor(D3DXCOLOR col) {
	if (m_pModel == nullptr) return;
	m_pModel->SetColorGlow(col);
}