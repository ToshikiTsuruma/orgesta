//=============================================================================
//
// パーティクルエフェクト処理 [particleEffect.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _PARTICLE_EFFECT_H_
#define _PARTICLE_EFFECT_H_

#include "main.h"
#include "object.h"
#include "particle.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// パーティクルエフェクトクラス
//*****************************************************************************
class CParticleEffect : public CObject
{
public:
	//生成するパーティクルの情報
	typedef struct {
		int nLife;			//ライフ
		float fSizeStart;	//開始時のサイズ
		float fAddSize;		//サイズの追加量
		float fSpeedMove;	//移動速度
		D3DXVECTOR3 addMove;//加速量
		D3DXCOLOR colStart;	//生成時の色
		D3DXCOLOR colEnd;	//消滅時の色
	} PARTICLE_INFO;

	CParticleEffect();	//デフォルトコンストラクタ
	CParticleEffect(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit, bool bRandomAngle);	//オーバーロードされたコンストラクタ
	virtual ~CParticleEffect();	//デストラクタ
	static CParticleEffect* Create(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit, bool bRandomAngle);	//生成処理
	static void EmitCircle(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreate, float fAngleEmit);	//円状にパーティクルを放射する
	static void EmitSphere(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreateXZ, int nNumCreateY, float fAddSpeed);	//球状にパーティクルを放射する
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理

private:
	PARTICLE_INFO m_particleInfo;	//生成するパーティクルの情報
	D3DXVECTOR3 m_pos;	//エフェクトの位置
	bool m_bLoop;		//ループするかどうか
	int m_nLife;		//エフェクトが存在する時間
	int m_nCntCreate;	//パーティクル生成のカウント
	int m_nSpanCreate;	//パーティクル生成の間隔
	float m_fAngleEmit;	//放射する角度
	bool m_bRandomAngle;//放射する角度がランダム
};

#endif // !_PARTICLE_EFFECT_H_