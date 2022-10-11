//=============================================================================
//
// カメラ処理 [camera.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// カメラクラス
//*****************************************************************************
class CCamera
{
public:
	CCamera();		//デフォルトコンストラクタ
	CCamera(float fMaxDrawDist);		//オーバーロードされたコンストラクタ
	virtual ~CCamera();	//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	void SetCamera(void);	//カメラのセット
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//位置座標の設定
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//位置座標の取得
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }	//視点の位置座標の設定
	D3DXVECTOR3 GetPosV(void) { return m_posV; }		//視点の位置座標の取得
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }	//注視点の位置座標の設定
	D3DXVECTOR3 GetPosR(void) { return m_posR; }		//注視点の位置座標の取得
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		//角度の設定
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }		//角度の追加
	D3DXVECTOR3 GetRot(void) { return m_rot; }			//角度の取得
	void SetDistance(float fDistance) { m_fDist = fDistance; }	//距離の設定
	float GetDistance(void) { return m_fDist; }			//距離の取得
	void SetLockControll(bool bLock) { m_bLockControll = bLock; }	//操作の制限を設定

private:
	void Move(void);	//移動
	void Rotate(void);	//回転

	const float m_fMaxDrawDist;	//描画可能な最大の距離
	D3DXVECTOR3 m_pos;			//カメラの位置
	D3DXVECTOR3 m_rot;			//カメラの向き
	D3DXVECTOR3 m_posV;			//視点
	D3DXVECTOR3 m_posR;			//注視点
	float m_fDist;				//視点と注視点の距離
	D3DXVECTOR3 m_vecU;			//上方向ベクトル
	D3DXMATRIX m_mtxProjection;	//プロジェクションマトリックス
	D3DXMATRIX m_mtxView;		//ビューマトリックス
	D3DVIEWPORT9 m_viewport;	//ビューポート

	bool m_bLockControll;	//操作の制限
};

#endif // !_CAMERA_H_