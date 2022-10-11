//=============================================================================
//
// 3Dオブジェクト処理 [object3D.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 3Dオブジェクトクラス
//*****************************************************************************
class CObject3D : public CObject
{
public:
	CObject3D();	//デフォルトコンストラクタ
	virtual ~CObject3D();	//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理

	void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	D3DXVECTOR3 GetPos(void);		//位置座標の取得
	void SetRot(D3DXVECTOR3 rot);	//角度の設定
	D3DXVECTOR3 GetRot(void);		//角度の取得
	LPDIRECT3DVERTEXBUFFER9* GetPtrVtxBuff(void);	//頂点バッファのポインタのポインタを取得
	LPDIRECT3DINDEXBUFFER9* GetPtrIdxBuff(void);	//インデックスバッファのポインタのポインタを取得
	void SetNumVtx(int nNumVtx);	//頂点数の設定
	int GetNumVtx(void);			//頂点数の取得
	void SetNumIdx(int nNumVtx);	//インデックス数の設定
	int GetNumIdx(void);			//インデックス数の取得
	void SetColor(D3DXCOLOR col) { m_col = col; }	//色の設定
	void AddColor(D3DXCOLOR col) { m_col += col; }	//色の加算
	D3DXCOLOR GetColor(void) { return m_col; }	//色の取得

private:
	D3DXVECTOR3	m_pos;	//ポリゴンの中心座標
	D3DXVECTOR3 m_rot;	//ポリゴンの角度
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; //インデックスバッファへのポインタ
	int m_nNumVtx;	//頂点数
	int m_nNumIdx;	//インデックス数
	D3DXCOLOR m_col;	//マテリアルの色
};

#endif // !_OBJECT3D_H_