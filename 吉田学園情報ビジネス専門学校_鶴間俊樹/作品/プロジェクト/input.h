//=============================================================================
//
// 入力処理 [input.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#include "main.h"

//*****************************************************************************
// インプットクラス
//*****************************************************************************
class CInput
{
public:
	enum class CODE {
		//システム
		SELECT = 0,
		BACK,
		PAUSE,
		//移動
		MOVE_UP,	//上
		MOVE_DOWN,	//下
		MOVE_LEFT,	//左
		MOVE_RIGHT,	//右
		MOVE_RISE,	//上昇
		MOVE_FALL,	//下降
		//回転
		ROTATE_UP,		//上
		ROTATE_DOWN,	//下
		ROTATE_LEFT,	//左
		ROTATE_RIGHT,	//右
		//選択
		SELECT_UP,
		SELECT_DOWN,
		SELECT_LFET,
		SELECT_RIGHT,
		//アクション
		ATTACK_1,
		ATTACK_2,
		DASH,
		INTERACT,

#ifdef _DEBUG
		//デバッグ用
		DEBUG_0,
		DEBUG_1,
		DEBUG_2,
		DEBUG_3,
#endif

		CODE_MAX
	};

	CInput();			//デフォルトコンストラクタ
	virtual ~CInput();	//デストラクタ
	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);	//初期化処理
	virtual void Uninit(void);		//終了処理
	virtual void Update(void);		//更新処理
	virtual bool GetPress(CODE code);	//入力判定（プレス）
	virtual bool GetTrigger(CODE code);	//入力判定（トリガー）

protected:
	LPDIRECTINPUTDEVICE8 m_pDevice;
	static LPDIRECTINPUT8 m_pInput;
};

//*****************************************************************************
// キーボードクラス
//*****************************************************************************
class CInputKeyboard : public CInput
{
public:
	CInputKeyboard();
	~CInputKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetPress(int nKey);
	bool GetTrigger(int nKey);
	bool GetPress(CODE code);	//入力判定（プレス）
	bool GetTrigger(CODE code);	//入力判定（トリガー）

private:
	BYTE m_aKeyState[256];
	BYTE m_aKeyStateTrigger[256];
};

//*****************************************************************************
// ゲームパッドクラス(Xinput)
//*****************************************************************************
class CInputGamepadX : public CInput
{
public:
	//トリガーの左右
	enum class TRIGGER_TYPE {
		RIGHT = 0,
		LEFT,
		ENUM_MAX
	};

	//スティックの上下左右
	enum class STICK_TYPE {
		MOVE_UP = 0,
		MOVE_DOWN,
		MOVE_LEFT,
		MOVE_RIGHT,
		ENUM_MAX
	};

	CInputGamepadX();	//デフォルトコンストラクタ
	~CInputGamepadX();	//デストラクタ
	void Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	bool GetConnectGamepad(void);	//ゲームパッド接続の判定
	bool GetButtonPress(int nButton);		//ボタンのプレス判定
	bool GetButtonTrigger(int nButton);		//ボタンのトリガー判定
	bool GetTriggerPress(TRIGGER_TYPE type);		//トリガーのプレス判定
	bool GetTriggerTrigger(TRIGGER_TYPE type);	//トリガーのトリガー判定
	bool GetLeftStick(STICK_TYPE type);			//左スティックの判定
	bool GetRightStick(STICK_TYPE type);			//右スティックの判定
	bool GetPress(CODE code);	//入力判定（プレス）
	bool GetTrigger(CODE code);	//入力判定（トリガー）
	void SetVibration(int nLeftValue, int nRightValue, int nCntVibration);	//バイブレーションの設定

private:
	XINPUT_STATE m_state;		//コントローラーの状態
	XINPUT_STATE m_stateLast;	//コントローラーのひとつ前の状態（トリガー判定用）
	XINPUT_VIBRATION m_vibration;	//バイブレーション
	bool m_bConnect;		//接続確認
	int m_nCntVibration;	//バイブレーション用のカウンター
	bool m_bVibration;		//バイブレーションが実行中かどうか
};

#endif // !_INPUT_H_