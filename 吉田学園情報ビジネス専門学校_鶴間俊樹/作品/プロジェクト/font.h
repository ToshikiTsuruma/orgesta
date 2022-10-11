//=============================================================================
//
// フォント処理 [font.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _FONT_H_
#define _FONT_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_FONT_TEXT (256)	//フォントの文字の表示できる最大数

//*****************************************************************************
// フォントクラス
//*****************************************************************************
class CFont : public CObject
{
public:
	CFont();		//デフォルトコンストラクタ
	~CFont();	//デストラクタ
	static CFont* Create(const char* pStrText, int nFlagAnchor, float fSide, float fTop, int nFontWidth);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetText(const char* pText);	//文字列の設定
	void SetRect(RECT rect);	//rectの設定
	RECT GetRect(void);			//rectの取得
	void SetColor(D3DXCOLOR col) { m_col = col; }	//フォントの色の設定
	void SetFlagAnchor(int nFlag) { m_nFlagAnchor = nFlag; }	//揃える位置のフラグの番号の設定

private:
	LPD3DXFONT m_pFont;		//フォントへのポインタ
	char m_sText[MAX_FONT_TEXT];	//描画する文字列
	RECT m_rect;			//描画範囲
	int m_nFontWidth;		//フォントの文字の幅
	D3DXCOLOR m_col;		//フォントの色
	int m_nFlagAnchor;		//揃える位置のフラグの番号

};

#endif // !_FONT_H_