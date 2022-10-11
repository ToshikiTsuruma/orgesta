//=============================================================================
//
// 3D選択メニュー処理 [selectMenu3D.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SELECT_MENU_3D_H_
#define _SELECT_MENU_3D_H_

#include "selectMenu.h"
#include "objectModelUI.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 3D選択メニュークラス
//*****************************************************************************
class CSelectMenu3D : public CSelectMenu
{
public:

	CSelectMenu3D();		//デフォルトコンストラクタ
	CSelectMenu3D(int nNumSelect, D3DXVECTOR3 posCenter, float fRadiusModel, CModel::MODELTYPE typeModel, float fDistCamera, float fHeightCamera, bool bUseBG);	//オーバーロードされたコンストラクタ
	virtual ~CSelectMenu3D();		//デストラクタ
	static CSelectMenu3D* Create(int nNumSelect, D3DXVECTOR3 posCenter, float fRadius, CModel::MODELTYPE typeModel, float fDistCamera, float fHeightCamera, bool bUseBG);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void) {}	//描画処理

	virtual void BeginChangeSelect(void);	//選択の変更が開始されたときの処理
	virtual void EndChangeSelect(void) {}	//選択の変更が終了したときの処理

	CObjectModelUI* GetModelUI(int nIdx);	//UIモデルオブジェクトのポインタを取得
	void SetSpeedRotModel(float fSpeed) { m_fSpeedRotModel = fSpeed; }	//モデル移動時の回転速度の設定

private:
	void CreateModelUI(void);	//モデルの生成
	void MoveModel(void);	//モデル移動

	const D3DXVECTOR3 m_posCenter;	//メニューの中央の位置
	const float m_fRadiusModel;	//オブジェクトを配置する半径

	CObjectModelUI** m_ppObjModelUIArray;	//UIモデルオブジェクトの配列のダブルポインタ
	CModel::MODELTYPE m_typeModel;	//生成するモデルの種類
	bool m_bMoveModel;		//モデル移動中かどうか
	float m_fRotModelY;		//モデルの配置位置を決める角度
	float m_fSpeedRotModel;	//モデル移動時の回転速度
	float m_fDistCamera;	//メニュー用のカメラの距離
	float m_fRotCameraY;	//メニュー用のカメラのY角度
	float m_fHeightCamera;	//メニュー用のカメラの位置の高さ
};

#endif // !_SELECT_MENU_3D_H_