//=============================================================================
//
// 林檎選択メニュー処理 [appleMenu.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _APPLE_MENU_H_
#define _APPLE_MENU_H_

#include "selectMenu3D.h"
#include "glowApple.h"
#include "appleTree.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define NUM_SELECT_APPLE_MENU (3)	//選択する林檎の数

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObject2D;
class CFont;

//*****************************************************************************
// 林檎選択メニュークラス
//*****************************************************************************
class CAppleMenu : public CSelectMenu3D
{
public:

	CAppleMenu();		//デフォルトコンストラクタ
	CAppleMenu(D3DXVECTOR3 posCenter);	//オーバーロードされたコンストラクタ
	~CAppleMenu();		//デストラクタ
	static CAppleMenu* Create(D3DXVECTOR3 posCenter, CAppleTree* pAppleTree);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	virtual void Draw(void) {}	//描画処理

	CGlowApple::APPLE_TYPE GetSelectAppleType(void);	//現在選択中の林檎の種類を取得
	void EndChangeSelect(void);		//選択の変更が終了したときの処理

private:
	void SetAppleTypeRandom(void);	//林檎の種類をランダムで決定
	void SetAppleText(int nSelect);	//林檎の説明テキストを設定

	CGlowApple::APPLE_TYPE m_aTypeApple[NUM_SELECT_APPLE_MENU];	//林檎の種類の選択肢分の配列
	CAppleTree* m_pAppleTree;	//林檎を生成させる木のポインタ
	CObject2D* m_pGrowUpText;	//grow upの画像テキスト
	CObject2D* m_apArrow[2];	//矢印UI
	CFont* m_pFontAppleText;	//林檎の説明テキスト
};

#endif // !_APPLE_MENU_H_