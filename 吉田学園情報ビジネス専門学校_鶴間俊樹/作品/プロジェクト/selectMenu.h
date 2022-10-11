//=============================================================================
//
// 選択メニュー処理 [selectMenu.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SELECT_MENU_H_
#define _SELECT_MENU_H_

#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// 選択メニュークラス
//*****************************************************************************
class CSelectMenu : public CObject
{
public:
	enum class SELECT_TYPE {
		VERTICAL = 0,	//上下
		HORIZONTAL,		//左右
		ENUM_MAX
	};

	CSelectMenu();		//デフォルトコンストラクタ
	CSelectMenu(int nNumSelect, bool bUseBG);		//オーバーロードされたコンストラクタ
	virtual ~CSelectMenu();		//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void) {}	//描画処理

	virtual void BeginChangeSelect(void) = 0;	//選択の変更が開始されたときの処理
	virtual void EndChangeSelect(void) = 0;		//選択の変更が終了したときの処理
	void SetMenuBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);	//メニューの背景の設定

	void SetSelectType(SELECT_TYPE type) { m_typeSelect = type; }	//入力の種類を設定
	int GetNumSelect(void) { return m_nNumSelect; }	//選択肢の数を取得
	int GetIdxCurSelect(void) { return m_nIdxCurSelect; }	//現在の選択の番号を取得
	void SetLockChangeSelect(bool bLock) { m_bLockChangeSelect = bLock; }	//選択の変更の制限状態の設定
	bool GetLockChangeSelect(void) { return m_bLockChangeSelect; }			//選択の変更の制限状態の取得

private:
	void ChangeSelect(void);	//選択の変更

	const int m_nNumSelect;		//選択肢の数
	SELECT_TYPE m_typeSelect;	//入力の種類
	int m_nIdxCurSelect;		//現在の選択の番号
	bool m_bFirstFrame;			//生成して最初の１フレームかどうか
	bool m_bLockChangeSelect;	//選択の変更の制限状態
	CObject2D* m_pMenuBG;		//メニューの背景へのポインタ
};

#endif // !_SELECT_MENU_H_