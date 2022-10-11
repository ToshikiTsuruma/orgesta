//=============================================================================
//
// ゲームシーン処理 [gameScene.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include "scene.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CStage;
class CAppleTree;
class CTimer;
class CPauseMenu;
class CSelectMenu2D;
class CEnemySpawner;
class CScore;
class CObject2D;
class CGameScoreManager;
class CScapegoatCreater;

//*****************************************************************************
// ゲームシーンクラス
//*****************************************************************************
class CGameScene : public CScene
{
public:
	CGameScene();	//デフォルトコンストラクタ
	virtual ~CGameScene();	//デストラクタ
	void Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void DeletePauseMenu(void);	//ポーズメニューの破棄
	void GameClear(void);	//ゲームクリア
	void GameOver(void);	//ゲームオーバー

	bool GetGameClear(void) { return m_bGameClear; }	//ゲームクリア状態のフラグ
	bool GetGameOver(void) { return m_bGameOver; }		//ゲームオーバー状態のフラグ

	CAppleTree* GetAppleTree(void) { return m_pAppleTree; }	//林檎の木の取得
	CTimer* GetTimer(void) { return m_pTimer; }	//タイマーの取得
	CEnemySpawner* GetEnemySpawner(void) { return m_pEnemySpawner; }	//敵のスポナーを取得
	void AddGameScore(int nScore);	//ゲームのスコアを加算
	void AddNumKillEnemy(int nNum);	//ゲーム中に敵を倒した数の加算

private:
	void UpdateGame(void);			//ゲーム中の更新
	void UpdateGameClear(void);		//ゲームクリア時の更新
	void UpdateGameOver(void);		//ゲームオーバー時の更新
	void CreateMenuEndGame(void);	//ゲーム終了時の選択メニューの生成
	void SaveAppleType(void);		//ゲーム終了時のリンゴの種類の保存

	bool m_bGameClear;	//ゲームオーバー
	bool m_bGameOver;	//ゲームオーバー

	CStage* m_pStage;				//ステージへのポインタ
	CAppleTree* m_pAppleTree;		//林檎の木のポインタ
	CTimer* m_pTimer;				//ゲームのタイマー
	CObject2D* m_pTimerFrame;		//タイマーの枠
	CScore* m_pScore;				//スコアへのポインタ
	CObject2D* m_pScoreFrame;		//スコアの枠
	CGameScoreManager* m_pGameScoreManager;	//ゲームのスコアのマネージャー
	int m_nGameScore;				//ゲーム終了時のスコア
	CPauseMenu* m_pMenuPause;		//ポーズメニュー
	CSelectMenu2D* m_pMenuGameEnd;	//ゲーム終了時の選択メニュー
	CEnemySpawner* m_pEnemySpawner;	//敵のスポナー
	CScapegoatCreater* m_pScapegoatCreater;	//生贄生成クラス

	int m_nCntGameClear;	//ゲームクリア後のカウント
};

#endif // !_GAME_SCENE_H_