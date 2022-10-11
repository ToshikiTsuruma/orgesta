//=============================================================================
//
// 人形敵処理 [enemy_human.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _ENEMY_HUMAN_H_
#define _ENEMY_HUMAN_H_

#include "enemy.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************

//*****************************************************************************
// 人形敵クラス
//*****************************************************************************
class CEnemyHuman : public CEnemy
{
public:
	//モーションの種類
	enum class MOTION_TYPE {
		NEUTRAL = 0,	//ニュートラルモーション
		WALK,	//歩き
		DASH,	//走り
		ATTACK,	//攻撃
		DEAD,	//死亡
		ENUM_MAX
	};

	CEnemyHuman();	//デフォルトコンストラクタ
	~CEnemyHuman();	//デストラクタ

	static CEnemyHuman* Create(D3DXVECTOR3 pos);	//生成処理
	static void LoadMotionInfo(void);	//モーション情報をロード
	static void UnloadMotionInfo(void);	//モーション情報をアンロード

	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void);		//描画処理
	void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol);	//当たり判定の情報の取得

	MOTION_CATEGORY GetMotionCategory(void);	//モーションの区分の取得
	void SetMoveMotion(void);	//移動時のモーションを設定
	void SetDeadMotion(void);	//死亡時のモーションを設定
	void AttackStart(void);		//攻撃開始時
	void AttackStop(void);		//攻撃停止
	void MotionEnd(void);	//モーション終了時
	void MotionAct(void);	//モーション中の行動

private:
	static int m_nNumParts;	//パーツ数
	static PARTS_INFO* m_pPartsInfoArray;	//パーツ情報の配列のポインタ
	static MOTION_INFO m_aMotionInfo[(int)MOTION_TYPE::ENUM_MAX];	//モーション情報
};

#endif // !_ENEMY_HUMAN_H_