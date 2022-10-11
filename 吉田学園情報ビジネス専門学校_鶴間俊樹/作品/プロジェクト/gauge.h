//=============================================================================
//
// ゲージ処理 [gauge.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAUGE_H_
#define _GAUGE_H_

#include "main.h"
#include "texture.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// ゲージクラス
//*****************************************************************************
class CGauge : public CObject
{
public:

	CGauge();		//デフォルトコンストラクタ
	CGauge(int nMaxValue, bool bVertical);		//オーバーロードされたコンストラクタ
	virtual ~CGauge();		//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void) {}	//描画処理
	virtual void CreateGauge(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;			//ゲージを生成
	virtual void CreateGaugeBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;		//ゲージの背景を生成
	virtual void CreateGaugeFrame(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) = 0;	//ゲージの枠を生成

	void AddGaugeValue(int nAddValue);	//ゲージの値を追加
	void SetGaugeValue(int nValue);	//ゲージの値を設定
	void SetMaxValue(int nValue);	//ゲージの最大値を設定
	void SetMaxDrawLife(int nLife) { m_nDrawLifeMax = nLife; }	//描画時間の最大値を設定
	void SetExtend(bool bExtend) { m_bExtend = bExtend; }	//最大値に合わせてゲージの幅を拡張するかどうかの設定

	void SetGaugeDraw(bool bDraw);	//ゲージの描画状態の設定

	void SetAllGaugePos(D3DXVECTOR3 pos);	//ゲージのすべての位置の設定
	void SetGaugePos(D3DXVECTOR3 pos);		//ゲージの位置の設定
	void SetGaugeBGPos(D3DXVECTOR3 pos);	//ゲージの背景の位置の設定
	void SetGaugeFramePos(D3DXVECTOR3 pos);	//ゲージの枠の位置の設定

	void SetGaugeColor(D3DXCOLOR col);		//ゲージの色の設定
	void SetGaugeColorDanger(D3DXCOLOR col) { m_colGaugeDanger = col; }	//ゲージのデフォルトカラーの設定
	void SetDangerValue(int nValue) { m_nDangerValue = nValue; }	//ゲージの危険時の色になる値を設定
	void SetGaugeBGColor(D3DXCOLOR col);	//ゲージの背景の色の設定
	void SetGaugeFrameColor(D3DXCOLOR col);	//ゲージの枠の色の設定

protected:
	//このオブジェクト生成以前のオブジェクトを設定するとおそらくオブジェクト全破棄時に壊れるので注意
	void SetGaugePtr(CObject* pGauge);				//ゲージのポインタを設定
	void SetGaugeBGPtr(CObject* pGaugeBG);			//ゲージの背景のポインタを設定
	void SetGaugeFramePtr(CObject* pGaugeFrame);	//ゲージの枠のポインタを設定

private:
	const bool m_bVertical;		//縦方向のゲージかどうか
	const int m_nMaxValueDefault;	//生成時の値の最大値
	int m_nDrawLifeMax;	//描画時間の最大
	int m_nCntDrawLife;	//描画時間
	int m_nMaxValue;	//ゲージの値の最大
	int m_nValue;		//ゲージの値
	bool m_bExtend;		//最大値に合わせて拡張するかどうか

	CObject* m_pGauge;		//ゲージのポインタ
	CObject* m_pGaugeBG;	//ゲージの背景のポインタ
	CObject* m_pGaugeFrame;	//ゲージの枠のポインタ

	D3DXCOLOR m_colGaugeDefault;	//ゲージのデフォルト色
	D3DXCOLOR m_colGaugeDanger;		//ゲージの危険時の色
	int m_nDangerValue;	//ゲージが危険時の色になる値
};

#endif // !_GAUGE_H_