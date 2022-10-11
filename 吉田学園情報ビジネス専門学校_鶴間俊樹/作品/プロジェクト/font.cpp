//=============================================================================
//
// フォント処理 [font.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "font.h"
#include "renderer.h"
#include "manager.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CFont::CFont()
{
	SetDrawPriority(DRAW_PRIORITY::UI);

	wsprintf(m_sText, "text");
	m_rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_nFlagAnchor = DT_LEFT;	//デフォルトは左揃え
}

//=============================================================================
// デストラクタ
//=============================================================================
CFont::~CFont()
{

}

//=============================================================================
// フォントの生成処理
//=============================================================================
CFont* CFont::Create(const char* pStrText, int nFlagAnchor, float fSide, float fTop, int nFontWidth) {
	CFont* pFont;
	pFont = new CFont;
	if (pFont == nullptr) return nullptr;

	wsprintf(pFont->m_sText, pStrText);
	pFont->m_nFlagAnchor = nFlagAnchor;
	//揃える位置によって設定する位置の種類を変える
	if (nFlagAnchor == DT_RIGHT) {
		pFont->m_rect.right = (LONG)fSide;
	}
	else {
		pFont->m_rect.left = (LONG)fSide;
	}
	pFont->m_rect.top = (LONG)fTop;
	pFont->m_nFontWidth = nFontWidth;
	pFont->Init();

	return pFont;
}

//=============================================================================
// フォントの初期化処理
//=============================================================================
HRESULT CFont::Init(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//デバイスへのポインタ
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//レンダラーの取得
	CRenderer* pRenderer = nullptr;
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//デバイスの取得
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//デバイスがnullの場合終了
	if (pDevice == nullptr) return S_OK;

	if (m_pFont == nullptr) {
		//フォントの生成
		D3DXCreateFont(pDevice, m_nFontWidth, 0, FW_REGULAR, 0, FALSE, SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "ＭＳ 明朝", &m_pFont);
	}

	return S_OK;
}

//=============================================================================
// フォントの終了処理
//=============================================================================
void CFont::Uninit(void) {
	//頂点バッファの破棄
	if (m_pFont != nullptr) {
		m_pFont->Release();
		m_pFont = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// フォントの更新処理
//=============================================================================
void CFont::Update(void) {

}

//=============================================================================
// フォントの描画処理
//=============================================================================
void CFont::Draw(void) {
	// テキスト描画
	m_pFont->DrawText(nullptr, m_sText, -1, &m_rect, m_nFlagAnchor, m_col);
}

//=============================================================================
// フォントの文字列の設定
//=============================================================================
void CFont::SetText(const char* pText) {
	if (pText == nullptr) return;

	if (strlen(pText) < MAX_FONT_TEXT) {
		strcpy(m_sText, pText);
	}
}

//=============================================================================
// rectの設定
//=============================================================================
void CFont::SetRect(RECT rect) {
	m_rect = rect;
}

//=============================================================================
// rectの取得
//=============================================================================
RECT CFont::GetRect(void) {
	return m_rect;
}