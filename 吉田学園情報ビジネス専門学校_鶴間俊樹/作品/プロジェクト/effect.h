//=============================================================================
//
// エフェクト処理 [effect.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "main.h"
#include "billboard.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// エフェクトクラス
//*****************************************************************************
class CEffect : public CBillboard
{
public:

	//エフェクトの種類
	enum class EFFECT_TYPE{
		EXPLOSION = 0,	//爆発
		DAMAGE_PLAYER,	//プレイヤー攻撃のダメージ
		DAMAGE_ENEMY,	//敵の攻撃のダメージ
		DAMAGE_BULLET,	//弾のダメージ
		DEATH,			//死亡
		HEAL_APPLE,		//林檎による回復
		DRAIN,			//林檎によるHP吸収
		REPULSIVEWAVE,	//林檎による斥力波生成
		FIRE_BOLL,		//火の玉
		FIRE,			//火
		THUNDERBOLT,	//落雷
		THUNDER,		//雷
		WIND,			//風の渦
		ENUM_MAX
	};	

	CEffect();	//デフォルトコンストラクタ
	CEffect(EFFECT_TYPE type, bool bLoop);	//オーバーロードされたコンストラクタ
	virtual ~CEffect();	//デストラクタ
	static CEffect* Create(D3DXVECTOR3 pos, EFFECT_TYPE type, float fWidth, float fHeight, bool bLoop);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	EFFECT_TYPE m_type;		//エフェクトのタイプ
	int m_nCntAnim;			//アニメーションカウンター
	int m_nSpanAnim;		//アニメーション切り替えの間隔
	int m_nPatternAnim;		//パターン番号
	int m_nPatternU, m_nPatternV;	//UV各方向へのパターンの数
	const bool m_bLoop;	//ループするかどうか

	bool m_bDestBlendOne;	//ブレンド係数を１にする？
};

#endif // !_EFFECT_H_