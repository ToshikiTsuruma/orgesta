//=============================================================================
//
// ゲームスコアマネージャー処理 [gameScoreManager.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAMESCORE_MANAGER_H_
#define _GAMESCORE_MANAGER_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;
class CGauge2D;
class CScore;

//*****************************************************************************
// ゲームスコアマネージャークラス
//*****************************************************************************
class CGameScoreManager : public CObject
{
public:
	CGameScoreManager();	//デフォルトコンストラクタ
	~CGameScoreManager();	//デストラクタ
	static CGameScoreManager* Create(D3DXVECTOR3 pos, float fSize);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理

	void AddKillCombo(void);		//倒したコンボの加算
	float GetScoreRate(void) { return m_fScoreRate; }	//スコアの倍率を取得

private:
	int m_nKillCombo;	//倒したコンボ数
	int m_nTimeCombo;	//コンボの残り時間
	int m_nTimeComboMax;	//コンボの残り時間の最大
	float m_fScoreRate;		//スコアの倍率

	CObject2D* m_pScoreRate;		//スコア倍率のテキスト
	CObject2D* m_pTextCombo;		//コンボのテキスト
	CGauge2D* m_pComboTimeGauge;	//コンボの残り時間のゲージ
	CScore* m_pComboScore;			//コンボ数
};

#endif // !_GAMESCORE_MANAGER_H_