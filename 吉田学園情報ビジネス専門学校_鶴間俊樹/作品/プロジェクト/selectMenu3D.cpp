//=============================================================================
//
// 3D選択メニュー処理 [selectMenu3D.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "selectMenu3D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SPEED_ROT_MODEL_DEFAULT (0.07f)	//モデル移動時の回転速度のデフォルト

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSelectMenu3D::CSelectMenu3D() : m_posCenter(D3DXVECTOR3()), m_fRadiusModel(0.0f)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CSelectMenu3D::CSelectMenu3D(int nNumSelect, D3DXVECTOR3 posCenter, float fRadiusModel, CModel::MODELTYPE typeModel, float fDistCamera, float fHeightCamera, bool bUseBG)
	: CSelectMenu(nNumSelect,bUseBG), m_posCenter(posCenter), m_fRadiusModel(fRadiusModel)
{
	m_bMoveModel = false;
	m_fRotModelY = 0.0f;
	m_typeModel = typeModel;
	m_fDistCamera = fDistCamera;
	m_fRotCameraY = 0.0f;
	m_fHeightCamera = fHeightCamera;
	//左右の入力にする
	SetSelectType(SELECT_TYPE::HORIZONTAL);

	CreateModelUI();
}

//=============================================================================
// デストラクタ
//=============================================================================
CSelectMenu3D::~CSelectMenu3D()
{

}

//=============================================================================
// 3D選択メニューの生成処理
//=============================================================================
CSelectMenu3D* CSelectMenu3D::Create(int nNumSelect, D3DXVECTOR3 posCenter, float fRadius, CModel::MODELTYPE typeModel, float fDistCamera, float fHeightCamera, bool bUseBG) {
	CSelectMenu3D* pSelectMenu3D;
	pSelectMenu3D = new CSelectMenu3D(nNumSelect, posCenter, fRadius, typeModel, fDistCamera, fHeightCamera, bUseBG);
	if (pSelectMenu3D == nullptr) return nullptr;

	pSelectMenu3D->Init();

	return pSelectMenu3D;
}

//=============================================================================
// 3D選択メニューの初期化処理
//=============================================================================
HRESULT CSelectMenu3D::Init(void) {
	m_bMoveModel = false;
	m_fRotModelY = 0.0f;
	m_fSpeedRotModel = SPEED_ROT_MODEL_DEFAULT;
	m_fRotCameraY = 0.0f;

	CSelectMenu::Init();

	return S_OK;
}

//=============================================================================
// 3D選択メニューの終了処理
//=============================================================================
void CSelectMenu3D::Uninit(void) {
	int nNumSelect = GetNumSelect();

	//UIモデルの破棄
	if (m_ppObjModelUIArray != nullptr) {
		for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
		{
			if (m_ppObjModelUIArray[nCntModel] == nullptr) continue;
			m_ppObjModelUIArray[nCntModel]->Uninit();
			m_ppObjModelUIArray[nCntModel] = nullptr;
		}
		m_ppObjModelUIArray = nullptr;
	}

	CSelectMenu::Uninit();
}

//=============================================================================
// 3D選択メニューの更新処理
//=============================================================================
void CSelectMenu3D::Update(void) {
	//モデルの位置を移動中
	if (m_bMoveModel) MoveModel();

	CSelectMenu::Update();
}

//=============================================================================
// UIモデルの生成
//=============================================================================
void CSelectMenu3D::CreateModelUI(void) {
	//すでに生成済みの場合終了
	if (m_ppObjModelUIArray != nullptr) return;

	//選択肢の数の取得
	int nNumSelect = GetNumSelect();
	m_ppObjModelUIArray = new CObjectModelUI*[nNumSelect];

	//生成できなかった場合終了
	if (m_ppObjModelUIArray == nullptr) return;

	//UIモデルを映すカメラを設定
	D3DXMATRIX mtxView;	//ビューマトリックス
	D3DXVECTOR3 posR, posV;	//視点と注視点

	posR = m_posCenter;
	posV = D3DXVECTOR3(m_posCenter.x, m_posCenter.y + m_fHeightCamera, m_posCenter.z - m_fDistCamera);
	//ビューマトリックスの作成
	D3DXMatrixLookAtLH(&mtxView,
		&posV,
		&posR,
		&D3DXVECTOR3(0.0f, 1.0f, 0.0f));

	for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
	{
		D3DXVECTOR3 posModel;	//UIモデルを配置する位置
		posModel = m_posCenter +
			D3DXVECTOR3(sinf(D3DX_PI * 2.0f * (1.0f - (float)nCntModel / nNumSelect) + D3DX_PI) * m_fRadiusModel, //1.0- で左回りの配置	+PIで手前側に１番目のオブジェクトが来るように
				0.0f,
				cosf(D3DX_PI * 2.0f * (1.0f - (float)nCntModel / nNumSelect) + D3DX_PI) * m_fRadiusModel);
		//UIモデルの生成
		m_ppObjModelUIArray[nCntModel] = CObjectModelUI::Create(m_typeModel, posModel, D3DXVECTOR3(0.0f, 0.0f, 0.0f), false);

		if (m_ppObjModelUIArray[nCntModel] == nullptr) continue;
		//UIモデルのビューマトリックスと視点を設定
		m_ppObjModelUIArray[nCntModel]->SetViewMatrix(mtxView);
		m_ppObjModelUIArray[nCntModel]->SetViewPos(posV);
	}
}

//=============================================================================
// モデルの移動
//=============================================================================
void CSelectMenu3D::MoveModel(void) {
	//モデルのダブルポインタがnullの場合
	if (m_ppObjModelUIArray == nullptr) {
		m_bMoveModel = false;
		//選択の変更の制限を解除する
		SetLockChangeSelect(false);
	}

	int nNumSelect = GetNumSelect();	//選択肢の数を取得
	bool bEndMove = false;	//移動が完了したかどうか
	float fRotModelDest = D3DX_PI * 2 * ((float)GetIdxCurSelect() / nNumSelect);	//目標の角度

	//モデルの配置位置を決める角度を変更
	if (fRotModelDest - m_fRotModelY > 0) {	
		//目標の角度との差分がπ以上だった場合
		if (fRotModelDest - m_fRotModelY > D3DX_PI) {
			m_fRotModelY -= m_fSpeedRotModel;
			//0を下回った場合2piを足す
			if (m_fRotModelY < 0) {
				m_fRotModelY += D3DX_PI * 2;
				//2piを足してから目標角度以下になった場合
				if (m_fRotModelY <= fRotModelDest) {
					//移動完了
					m_fRotModelY = fRotModelDest;
					bEndMove = true;
				}
			}
		}
		else {
			m_fRotModelY += m_fSpeedRotModel;
			//目標角度以上になった場合
			if (m_fRotModelY >= fRotModelDest) {
				//移動完了
				m_fRotModelY = fRotModelDest;
				bEndMove = true;
			}
		}
	}
	else {
		//目標の角度との差分が-π以上だった場合
		if (fRotModelDest - m_fRotModelY < -D3DX_PI) {
			m_fRotModelY += m_fSpeedRotModel;
			//πを上回った場合2piを引く
			if (m_fRotModelY > D3DX_PI) {
				m_fRotModelY -= D3DX_PI * 2;
				//2πを引いてから目標角度以上になった場合
				if (m_fRotModelY >= fRotModelDest) {
					//移動完了
					m_fRotModelY = fRotModelDest;
					bEndMove = true;
				}
			}
		}
		else {
			m_fRotModelY -= m_fSpeedRotModel;
			//目標角度以下になった場合
			if (m_fRotModelY <= fRotModelDest) {
				//移動完了
				m_fRotModelY = fRotModelDest;
				bEndMove = true;
			}
		}
	}

	//UIモデルの位置を変更
	for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
	{
		D3DXVECTOR3 posModel;	//UIモデルを配置する位置
		posModel = m_posCenter +
			D3DXVECTOR3(sinf(D3DX_PI * 2.0f * (1.0f - (float)nCntModel / nNumSelect) + D3DX_PI + m_fRotModelY) * m_fRadiusModel, //1.0- で左回りの配置	+PIで手前側に１番目のオブジェクトが来るように
				0.0f,
				cosf(D3DX_PI * 2.0f * (1.0f - (float)nCntModel / nNumSelect) + D3DX_PI + m_fRotModelY) * m_fRadiusModel);
		//UIモデルの位置を設定
		if (m_ppObjModelUIArray[nCntModel] != nullptr) m_ppObjModelUIArray[nCntModel]->SetPos(posModel);
	}

	//選択の遷移が完了時
	if (bEndMove) {
		m_bMoveModel = false;
		//選択の変更の制限を解除する
		SetLockChangeSelect(false);
		//終了時の処理
		EndChangeSelect();
	}
}

//=============================================================================
// 選択の変更が開始されたときの処理
//=============================================================================
void CSelectMenu3D::BeginChangeSelect(void) {
	m_bMoveModel = true;
	//選択の変更を制限する
	SetLockChangeSelect(true);
}

//=============================================================================
// UIモデルオブジェクトのポインタを取得
//=============================================================================
CObjectModelUI* CSelectMenu3D::GetModelUI(int nIdx) {
	if (m_ppObjModelUIArray == nullptr || nIdx < 0 || nIdx >= GetNumSelect()) return nullptr;	//配列から取得できない場合null
	return m_ppObjModelUIArray[nIdx];
}