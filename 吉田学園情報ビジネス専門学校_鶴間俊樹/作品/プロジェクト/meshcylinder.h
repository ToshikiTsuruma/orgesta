//=============================================================================
//
// メッシュシリンダー処理 [meshcylinder.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MESHCYLINDER_H_
#define _MESHCYLINDER_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュシリンダークラス
//*****************************************************************************
class CMeshcylinder : public CObject3D
{
public:
	CMeshcylinder();	//デフォルトコンストラクタ
	CMeshcylinder(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadiusTop, float fRadiusBottom, int nNumY, float fHeight, bool bOut);	//オーバーロードされたコンストラクタ
	virtual ~CMeshcylinder();	//デストラクタ
	static CMeshcylinder* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumXZ, float fRadiusTop, float fRadiusBottom, int nNumY, float fHeight, bool bOut, CTexture::TEXTURE_TYPE typeTex);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	float GetRadius(void);		//半径の取得（下）
	float GetRadiusTop(void);	//上半径の取得
	void AddRadius(float fRadius);	//半径を追加
	void AddRadiusTop(float fRadius);		//上半径を追加
	void AddRadiusBottom(float fRadius);	//下半径を追加
	void AddHeight(float fHeight);	//高さを追加
	float GetHeight(void) { return m_fHeight; }	//高さを取得

private:
	void SetVtxBuff(void);	//頂点バッファの情報を更新
	void SetVtxBuffPos(void);	//頂点バッファの位置情報を更新

	const int m_nNumXZ;	//XZ方向のポリゴンの数
	const int m_nNumY;	//Y方向のポリゴンの数
	float m_fHeight;	//ポリゴンの高さ
	float m_fRadiusTop;		//円柱の半径の上
	float m_fRadiusBottom;	//円柱の半径の下
	bool m_bOut;		//ポリゴンが外向きかどうか
};

#endif // !_MESHCYLINDER_H_