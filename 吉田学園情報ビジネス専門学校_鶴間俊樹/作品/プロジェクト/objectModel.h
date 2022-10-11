//=============================================================================
//
// モデルオブジェクト処理 [objectModel.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT_MODEL_H_
#define _OBJECT_MODEL_H_

#include "main.h"
#include "object.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// モデルオブジェクトクラス
//*****************************************************************************
class CObjectModel : public CObject
{
public:

	CObjectModel();		//デフォルトコンストラクタ
	CObjectModel(CModel::MODELTYPE typeModel, bool bOutLine);		//オーバーロードされたコンストラクタ
	virtual ~CObjectModel();		//デストラクタ
	static CObjectModel* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//位置座標の設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//位置座標の取得
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	//移動速度の設定
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		//角度の設定
	D3DXVECTOR3 GetRot(void) { return m_rot; }			//角度の取得
	void SetRotSpeed(D3DXVECTOR3 rotSpeed) { m_rotSpeed = rotSpeed; }	//移動速度の設定

	CModel::MODELTYPE GetModelType(void);	//モデルの種類の取得
	CModel* GetPtrModel(void) { return m_pModel; }		//モデルのポインタの取得
	void SetModelColor(D3DXCOLOR col, int nIdx);		//モデルの色の設定
	void SetModelGlowColor(D3DXCOLOR col);	//モデルの発光色の設定

private:
	void Move(void);	//移動
	void Rotate(void);	//回転

	CModel* m_pModel;	//モデルへのポインタ
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動速度
	D3DXVECTOR3 m_rot;	//角度
	D3DXVECTOR3 m_rotSpeed;	//回転速度
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
};

#endif // !_OBJECT_MODEL_H_