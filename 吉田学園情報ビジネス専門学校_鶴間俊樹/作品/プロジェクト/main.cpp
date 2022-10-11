//=============================================================================
//
// メイン処理 [main.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "main.h"
#include "manager.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define CLASS_NAME		"AppClass"			// ウインドウのクラス名
#define WINDOW_NAME		"GlowApple"	// ウインドウのキャプション名

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
#ifdef _DEBUG
int g_nCountFPS;	// FPSカウンタ
#endif

//=============================================================================
// メイン関数
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),
		CS_CLASSDC,
		WndProc,
		0,
		0,
		hInstance,
		NULL,
		LoadCursor(NULL, IDC_ARROW),
		(HBRUSH)(COLOR_WINDOW + 1),
		NULL,
		CLASS_NAME,
		NULL
	};
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	HWND hWnd;
	MSG msg;
	DWORD dwCurrentTime;
	DWORD dwFrameCount;
	DWORD dwExecLastTime;
	DWORD dwFPSLastTime;

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// 指定したクライアント領域を確保するために必要なウィンドウ座標を計算
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

	RECT rectDesk;        // デスクトップ領域
	//デスクトップのサイズを取得
	SystemParametersInfo(SPI_GETWORKAREA, 0, &rectDesk, 0);
	int nWndRectLeft = (rectDesk.right - SCREEN_WIDTH) / 2;		//ウィンドウの左端の位置
	int nWndRectTop = (rectDesk.bottom - SCREEN_HEIGHT) / 2;	//ウィンドウの上端の位置

	// ウィンドウの作成
	hWnd = CreateWindow(CLASS_NAME,
		WINDOW_NAME,
		WS_OVERLAPPEDWINDOW,

		//画面中央にウィンドウを生成
		nWndRectLeft,	//元はCW_USEDEFAULT
		nWndRectTop,	//元はCW_USEDEFAULT

		(rect.right - rect.left),
		(rect.bottom - rect.top),
		NULL,
		NULL,
		hInstance,
		NULL);

	CManager::Create();	//マネージャーのインスタンスの生成
	CManager* const pManager = CManager::GetManager();	//マネージャーのインスタンスの取得
	//初期化処理（ウィドウを生成してから行う）（DirectX本体の生成を行っている）
	if (pManager != nullptr) {
		bool bWindow = false;
#if _DEBUG
		bWindow = true;
#endif

		if (FAILED(pManager->Init(hInstance, hWnd, bWindow))) {
			return -1;
		}
	}

	// 分解能を設定
	timeBeginPeriod(1);

	// フレームカウント初期化
	dwCurrentTime =
		dwFrameCount = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// ウインドウの表示
	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

#ifdef NDEBUG
	// マウス移動範囲の設定
	//RECT rectMouse = { nWndRectLeft + 10, nWndRectTop + 10, nWndRectLeft + SCREEN_WIDTH, nWndRectTop + SCREEN_HEIGHT };	//ちょっとズレがあったので調整
	//ClipCursor(&rectMouse);
	//マウス非表示
	ShowCursor(FALSE);
#endif // NDEBUG

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれたらループ終了
				break;
			}
			else
			{
				// メッセージの翻訳とディスパッチ
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();	// 現在の時間を取得
			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒ごとに実行
#ifdef _DEBUG
			 // FPSを算出
				g_nCountFPS = dwFrameCount * 1000 / (dwCurrentTime - dwFPSLastTime);
#endif
				dwFPSLastTime = dwCurrentTime;	// 現在の時間を保存
				dwFrameCount = 0;
			}

			if ((dwCurrentTime - dwExecLastTime) >= (1000 / FPS))
			{// 1/60秒経過
				dwExecLastTime = dwCurrentTime;	// 現在の時間を保存

				// 更新処理
				if (pManager != nullptr) {
					pManager->Update();
				}

				// 描画処理
				if (pManager != nullptr) {
					pManager->Draw();
				}

				dwFrameCount++;
			}
		}
	}

	// 終了処理
	CManager::Destroy();	//マネージャーのインスタンスの破棄

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	// 分解能を戻す
	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
// ウインドウプロシージャ
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nID;

	switch (uMsg)
	{
	case WM_CREATE:
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]キーが押された
			nID = MessageBox(hWnd, TEXT("本当に終了しますか？"), TEXT("終了確認"), MB_YESNO | MB_ICONQUESTION);
			if (nID == IDYES) {
				//ウィンドウを破棄する（WM_DESTROYメッセージを送る）
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_CLOSE:	//閉じるボタン押下のメッセージ
		nID = MessageBox(hWnd, TEXT("本当に終了しますか？"), TEXT("終了確認"), MB_YESNO | MB_ICONQUESTION);
		if (nID == IDYES) {
			//ウィンドウを破棄する（WM_DESTROYメッセージを送る）
			DestroyWindow(hWnd);
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef _DEBUG
int GetFPS(void) {
	return g_nCountFPS;
}
#endif