//=============================================================================
//
// タイマー処理 [timer.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _TIMER_H_
#define _TIMER_H_

#include "score.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// タイマークラス
//*****************************************************************************
class CTimer : public CScore
{
public:
	CTimer();	//デフォルトコンストラクタ
	CTimer(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//オーバーロードされたコンストラクタ
	~CTimer();	//デストラクタ
	static CTimer* Create(int nStartTime, int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理
	int GetCountTimer(void) { return m_nCntTime; }	//タイマーのカウントを取得
	void SetStop(bool bStop) { m_bStop = bStop; }	//タイマーの停止状態を設定

private:
	bool m_bStop;		//タイマーが停止中かどうか
	int m_nCntTime;		//タイマーの残り時間が減るまでのカウント
};

#endif // !_TIMER_H_