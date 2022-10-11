//=============================================================================
//
// パーティクル処理 [particle.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// パーティクルクラス
//*****************************************************************************
class CParticle : public CBillboard
{
public:
	CParticle();	//デフォルトコンストラクタ
	CParticle(int nLife, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd);	//オーバーロードされたコンストラクタ
	virtual ~CParticle();	//デストラクタ
	static CParticle* Create(D3DXVECTOR3 pos, int nLife, float fSizeStart, float fAddSize, D3DXVECTOR3 move, D3DXVECTOR3 addMove, D3DXCOLOR colStart, D3DXCOLOR colEnd);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	const int m_nLifeDef;	//ライフの初期値
	int m_nLife;			//ライフ
	float m_fAddSize;		//サイズの追加量
	D3DXVECTOR3 m_move;		//移動量
	D3DXVECTOR3 m_addMove;	//加速量
	D3DXCOLOR m_colStart;	//生成時の色
	D3DXCOLOR m_colEnd;		//消滅時の色
};

#endif // !_PARTICLE_H_