//=============================================================================
//
// 3Dゲージ処理 [gauge3D.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAUGE_3D_H_
#define _GAUGE_3D_H_

#include "gauge.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 3Dゲージクラス
//*****************************************************************************
class CGauge3D : public CGauge
{
public:

	CGauge3D();		//デフォルトコンストラクタ
	CGauge3D(int nMaxValue, bool bVertical);	//オーバーロードされたコンストラクタ
	virtual ~CGauge3D();		//デストラクタ
	static CGauge3D* Create(int nMaxValue, bool bVertical, int nValue, int nDrawLifeMax, bool bExtend);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);			//ゲージを生成
	virtual void CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);		//ゲージの背景を生成
	virtual void CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);	//ゲージの枠を生成

private:

};

#endif // !_GAUGE_3D_H_