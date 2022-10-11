//=============================================================================
//
// ビルボード処理 [billboard.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// ビルボードクラス
//*****************************************************************************
class CBillboard : public CObject
{
public:
	CBillboard();	//デフォルトコンストラクタ
	virtual ~CBillboard();	//デストラクタ
	static CBillboard* Create(D3DXVECTOR3 pos, CTexture::TEXTURE_TYPE type, float fWidth, float fHeight);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	D3DXVECTOR3 GetPos(void);		//位置座標の取得
	void SetMove(D3DXVECTOR3 move);	//移動量の設定
	D3DXVECTOR3 GetMove(void);		//移動量の取得
	void AddSize(D3DXVECTOR3 size) { SetSize(GetSize() + size); }	//サイズの設定
	void SetSize(D3DXVECTOR3 size);	//サイズの設定
	D3DXVECTOR3 GetSize(void);		//サイズの取得
	void SetRatioWidth(float fRatio);	//幅の割合の設定
	void SetRatioHeight(float fRatio);	//高さの割合の設定
	void SetOffsetPos(D3DXVECTOR3 pos);	//位置のオフセットの設定
	void SetTexPos(float startU, float startV, float endU, float endV);	//テクスチャ座標の設定
	void SetColor(D3DXCOLOR col);	//色の設定
	void SetRotateYOnly(bool bFlag) { m_bRotateYOnly = bFlag; }	//カメラに対してY回転のみにするかどうかの設定

	void SetEnableFog(bool bEnable);	//フォグを有効にするかどうかの設定
	void SetZtestAlways(bool bAlways);	//深度関係なく描画できるかどうかの設定
	void SetZwriteEnable(bool bEnable);	//深度が高いポリゴンを遮るかどうかの設定
	void SetDrawAllDistance(bool bDraw);	//全ての距離で描画可能かどうかの設定
	void SetDistanceDrawMax(float fDist);	//描画可能な距離の最大値の設定

private:
	D3DXVECTOR3	m_pos;	//ポリゴンの中心座標
	D3DXVECTOR3 m_move;	//移動量
	D3DXMATRIX m_mtxWorld;//ワールドマトリックス
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	//頂点バッファのポインタ
	float m_fWidth;			//ポリゴンの幅
	float m_fRatioWidth;	//ポリゴンの幅の割合
	float m_fHeight;		//ポリゴンの高さ
	float m_fRatioHeight;	//ポリゴンの高さの割合
	D3DXVECTOR3 m_offsetPos;//位置のオフセット
	bool m_bRotateYOnly;	//Y回転のみ
	D3DXCOLOR m_col;	//頂点色

	bool m_bEnableFog;		//フォグを有効にするかどうか
	bool m_bZtestAlways;	//深度関係なく描画できるかどうか
	bool m_bZwriteEnable;	//深度が高いポリゴンを遮るかどうか
	bool m_bDrawAllDist;	//全ての距離で描画可能かどうか
	float m_fDistDrawMax;	//描画可能な距離の最大値
};

#endif // !_BILLBOARD_H_