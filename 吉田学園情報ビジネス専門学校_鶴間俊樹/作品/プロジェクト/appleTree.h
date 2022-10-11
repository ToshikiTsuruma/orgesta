//=============================================================================
//
// 林檎の木の処理 [appleTree.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _APPLE_TREE_H_
#define _APPLE_TREE_H_

#include "main.h"
#include "objectModel.h"
#include "glowApple.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define APPLETREE_POS_Y (430.0f)
#define MAX_LIFE_APPLETREE (2000)		//体力の最大値
#define MAX_NUM_CREATE_APPLE (6)	//生成可能な林檎の最大数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CAppleMenu;
class CGauge2D;
class CObject2D;

//*****************************************************************************
// 林檎の木クラス
//*****************************************************************************
class CAppleTree : public CObjectModel
{
public:
	CAppleTree();		//デフォルトコンストラクタ
	virtual ~CAppleTree();		//デストラクタ
	static CAppleTree* Create(D3DXVECTOR3 pos);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void Heal(int nHeal);		//体力を回復する
	void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead);		//ダメージ
	void Dead(void);		//死亡時処理

	static CGlowApple* CreateApple(CGlowApple::APPLE_TYPE typeApple, D3DXVECTOR3 posCreate, CAppleTree* pParentTree);	//林檎の生成
	void YieldApple(CGlowApple::APPLE_TYPE typeApple);	//林檎が実をつける
	static D3DXVECTOR3 GetOffsetPosApple(int nIdxApple);	//林檎生成位置の取得
	void SetMaxLife(int nMaxLife);	//体力の最大値の設定
	void AddMaxLife(int nAddLife);	//体力の最大値の増加
	void AddGrow(int nAddValue);	//成長度を増やす
	CGlowApple::APPLE_TYPE GetCreateAppleType(int nIdx);	//リンゴの種類の取得
	int GetNumApple(CGlowApple::APPLE_TYPE type);	//リンゴの数を取得

	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//当たり判定の情報の取得

private:
	void GrowUp(void);	//成長

	int m_nNumApple;	//生成した林檎の数
	CGlowApple* m_apCreateApple[MAX_NUM_CREATE_APPLE];	//生成したリンゴのポインタ
	CGlowApple::APPLE_TYPE m_aTypeCreateApple[MAX_NUM_CREATE_APPLE];	//生成したリンゴの種類

	//体力
	int m_nMaxLife;			//体力の最大値
	int m_nLife;			//体力
	CGauge2D* m_pGaugeLife;	//体力ゲージ
	CObject2D* m_pIconHP;	//HPのアイコン
	//成長
	int m_nGrowValue;		//成長度
	int m_nGrowValueMax;	//成長度の最大（成長の必要量）
	CGauge2D* m_pGaugeGrow;	//成長ゲージ

	bool m_bDead;			//死亡
	int m_nCntDead;			//死亡後のカウント

	CAppleMenu* m_pMenuApple;	//林檎のメニュー

};

#endif // !_APPLE_TREE_H_