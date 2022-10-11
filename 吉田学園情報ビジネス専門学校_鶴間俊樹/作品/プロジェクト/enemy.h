//=============================================================================
//
// エネミー処理 [enemy.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _ENEMY_H_
#define _ENEMY_H_

#include "objectMotion.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_DIGIT_ADD_TIME_ENEMY (2)	//敵を倒したときに追加される時間の最大桁数
#define ENEMY_ATTACK_TARGET (OBJTYPE_APPLETREE | OBJTYPE_SCAPEGOAT)

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CGauge3D;

//*****************************************************************************
// エネミークラス
//*****************************************************************************
class CEnemy : public CObjectMotion
{
public:
	enum class MOTION_CATEGORY {
		NEUTRAL = 0,	//ニュートラル
		MOVE,			//移動
		ATTACK,			//攻撃
		DEAD,			//死亡
		ENUM_MAX
	};	//モーションの区分

	CEnemy();	//デフォルトコンストラクタ
	CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, float fRotSpeed, int nLife, float fDistAttack, float fHeightLifeGauge);	//オーバーロードされたコンストラクタ
	~CEnemy();	//デストラクタ

	virtual HRESULT Init(void);		//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual void Draw(void);		//描画処理
	void ReleasePtr(CObject* pReleaseObj) { if(m_pAttackTarget == pReleaseObj) m_pAttackTarget = nullptr; }	//このオブジェクトが保持している、破棄される予定のオブジェクトのポインタをnullにする

	void SetMove(D3DXVECTOR3 move);	//移動量の設定
	void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead);		//ダメージ
	void Dead(void);		//死亡時処理
	void SetKillScore(int nScore) { m_nKillScore = nScore; }	//倒したスコアの設定

	static void SetGoldRush(bool bRush) { m_bGoldRush = bRush; }	//ゴールドラッシュの設定

	virtual MOTION_CATEGORY GetMotionCategory(void) = 0;	//モーションの区分の取得
	virtual void SetMoveMotion(void) = 0;	//移動時のモーションを設定
	virtual void SetDeadMotion(void) = 0;	//死亡時のモーションを設定
	virtual void AttackStart(void) = 0;		//攻撃開始時
	virtual void AttackStop(void) = 0;		//攻撃停止
	virtual void MotionEnd(void) = 0;	//モーション終了時
	virtual void MotionAct(void) = 0;	//モーション中の行動

protected:
	CObject* GetAttackTarget(void) { return m_pAttackTarget; }	//攻撃目標の取得

private:
	void SearchAttackTarget(void);	//攻撃目標を探す
	void SetRotToTarget(void);	//攻撃目標の方向への角度に設定
	void AttackDistance(void);	//攻撃可能な距離かどうかを計算
	void Attack(void);	//攻撃の処理

	const float m_fMoveSpeed;	//移動速度
	const float m_fRotateSpeed;	//回転速度
	const float m_fDistAttack;	//攻撃する距離
	const int m_nMaxLife;	//最大の体力

	D3DXVECTOR3 m_move;		//移動量
	bool m_bMove;			//移動可能かどうか
	float m_rotDestY;		//目標のY軸の角度
	bool m_bLand;			//接地しているかどうか
	CObject* m_pAttackTarget;	//攻撃目標
	bool m_bAttackDist;		//攻撃可能な距離かどうか
	int m_nLife;			//体力
	bool m_bDead;			//死亡
	int m_nCntDead;			//死亡後のカウント
	int m_nKillScore;		//倒したときのスコア

	static bool m_bGoldRush;	//すべての敵が金になるかどうか
	bool m_bGoldEnemy;		//金色の敵かどうか

	CGauge3D* m_pGaugeLife;	//体力ゲージ
	const float m_fHeightLifeGauge;	//体力ゲージの高さの差分
};

#endif // !_ENEMY_H_