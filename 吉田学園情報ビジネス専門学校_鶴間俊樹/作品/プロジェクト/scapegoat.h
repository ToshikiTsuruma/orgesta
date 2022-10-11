//=============================================================================
//
// 生贄の処理 [scapegoat.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCAPEGOAT_H_
#define _SCAPEGOAT_H_

#include "objectModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGauge3D;

//*****************************************************************************
// 生贄クラス
//*****************************************************************************
class CScapegoat : public CObjectModel
{
public:
	CScapegoat();	//デフォルトコンストラクタ
	~CScapegoat();	//デストラクタ

	static CScapegoat* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	//生成処理
	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理

	void Heal(int nHeal);		//体力を回復する
	void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead);		//ダメージ
	void Dead(void);		//死亡時処理
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//当たり判定の情報の取得

private:
	int m_nLife;	//体力
	CGauge3D* m_pGaugeLife;	//体力ゲージ
	int m_nCntAttack;	//	攻撃発生カウンタ
};

#endif // !_SCAPEGOAT_H_