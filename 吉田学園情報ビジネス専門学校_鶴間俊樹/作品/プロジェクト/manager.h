//=============================================================================
//
// マネージャー処理 [manager.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define	NUM_LIGHT (3)

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CRenderer;
class CInput;
class CInputKeyboard;
class CInputGamepadX;
class CCamera;
class CSound;
class CFade;
class CScene;
class CGameScene;

//*****************************************************************************
// マネージャークラス
//*****************************************************************************
class CManager
{
public:
	static void Create(void);		//マネージャーの生成
	static void Destroy(void);		//マネージャーの破棄
	static CManager* GetManager(void);	//マネージャーの取得

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

	void ChangeScene(int nTypeScene);	//シーンの変更

	CRenderer* GetRenderer(void) { return m_pRenderer; }	//レンダラーの取得
	CInput* GetInputCur(void) { return m_pInputCur; }		//現在の入力デバイスの取得
	bool GetEnableXInput(void) { return m_bEnableXInput; }	//現在の入力デバイスがXinputかどうかの取得
	void SetCamera(CCamera* pCamera);	//カメラの設定
	CCamera* GetCamera(void) { return m_pCamera; }				//カメラの取得
	CSound* GetSound(void) { return m_pSound; }	//サウンドの取得
	CFade* GetFade(void) { return m_pFade; }	//フェードの取得
	CScene* GetScene(void) { return m_pScene; }	//シーンの取得
	CGameScene* GetGameScene(void);	//ゲームシーンの取得

private:
	CManager();		//デフォルトコンストラクタ
	~CManager();	//デストラクタ
	static CManager* m_pManager;	//マネージャーのインスタンス

	CRenderer* m_pRenderer;				//レンダラーへのポインタ
	CInput* m_pInputCur;				//現在の入力デバイスへのポインタ
	bool m_bEnableXInput;				//Xinput有効
	CInputKeyboard* m_pInputKeyboard;	//キーボードへのポインタ
	CInputGamepadX* m_pInputGamepadX;	//ゲームパッドへのポインタ(Xinput)
	CCamera* m_pCamera;		//カメラへのポインタ
	CSound* m_pSound;		//サウンドへのポインタ
	CFade* m_pFade;			//フェードのポインタ
	CScene* m_pScene;		//シーンへのポインタ
};

#endif // !_MANAGER_H_
