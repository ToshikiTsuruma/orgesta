//=============================================================================
//
// UIモデルオブジェクト処理 [objectModelUI.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT_MODEL_UI_H_
#define _OBJECT_MODEL_UI_H_

#include "main.h"
#include "objectModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// UIモデルオブジェクトクラス
//*****************************************************************************
class CObjectModelUI : public CObjectModel
{
public:

	CObjectModelUI();		//デフォルトコンストラクタ
	CObjectModelUI(CModel::MODELTYPE typeModel, bool bOutLine);		//オーバーロードされたコンストラクタ
	virtual ~CObjectModelUI();		//デストラクタ
	static CObjectModelUI* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理

	void SetViewMatrix(D3DXMATRIX mtxView) { m_mtxView = mtxView; }	//ビューマトリックスの設定
	void SetViewPos(D3DXVECTOR3 posV) { m_posV = posV; }		//カメラの視点設定
	void SetRotSpeed(D3DXVECTOR3 rotSpeed) { m_rotSpeed = rotSpeed; }	//回転速度の設定
private:
	D3DXMATRIX m_mtxView;	//ビューマトリックス
	D3DXVECTOR3 m_posV;		//カメラの視点
	D3DXVECTOR3 m_rotSpeed;	//回転速度

};

#endif // !_OBJECT_MODEL_UI_H_