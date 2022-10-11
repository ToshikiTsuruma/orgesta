//=============================================================================
//
// 落雷放出処理 [thunderEmitter.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _THUNDER_EMITTER_H_
#define _THUNDER_EMITTER_H_

#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// 落雷放出クラス
//*****************************************************************************
class CThunderEmitter : public CObject
{
public:
	CThunderEmitter();	//デフォルトコンストラクタ
	CThunderEmitter(int nNumEmitDest);	//オーバーロードされたコンストラクタ
	~CThunderEmitter();	//デストラクタ
	static CThunderEmitter* CreateStraight(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, D3DXVECTOR3 vecStraight, float fDist);	//直進の落雷生成処理
	static CThunderEmitter* CreateRound(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, float rotY, float fRadius);	//円形の落雷生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理

private:
	D3DXVECTOR3* m_pPosEmitArray;	//放出目標数分の放出位置の動的配列のポインタ
	const int m_nNumEmitDest;	//放出する目標数
	int m_nNumEmit;		//放出した数
	int m_nCntEmit;		//放出のカウンター
	int m_nSpanEmit;	//放出のスパン
};

#endif // !_THUNDER_EMITTER_H_