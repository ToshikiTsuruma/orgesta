//=============================================================================
//
// 選択メニュー処理 [selectMenu.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "selectMenu.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "input.h"
#include "object2D.h"

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSelectMenu::CSelectMenu() : m_nNumSelect(0)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CSelectMenu::CSelectMenu(int nNumSelect, bool bUseBG) : m_nNumSelect(nNumSelect)
{
	m_typeSelect = SELECT_TYPE::VERTICAL;
	m_nIdxCurSelect = 0;

	if (bUseBG) {
		//背景の生成
		m_pMenuBG = CObject2D::Create(D3DXVECTOR3(), CTexture::TEXTURE_TYPE::NONE, 0.0f, 0.0f);	//後で値を設定する
	}
}

//=============================================================================
// デストラクタ
//=============================================================================
CSelectMenu::~CSelectMenu()
{

}

//=============================================================================
// 選択メニューの初期化処理
//=============================================================================
HRESULT CSelectMenu::Init(void) {
	m_nIdxCurSelect = 0;
	m_bLockChangeSelect = true;
	m_bFirstFrame = true;

	return S_OK;
}

//=============================================================================
// 選択メニューの終了処理
//=============================================================================
void CSelectMenu::Uninit(void) {
	//メニューの背景の破棄
	if (m_pMenuBG != nullptr) {
		m_pMenuBG->Uninit();
		m_pMenuBG = nullptr;
	}
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 選択メニューの更新処理
//=============================================================================
void CSelectMenu::Update(void) {
	//選択の変更
	if(!m_bLockChangeSelect) ChangeSelect();

	//最初の１フレームだった場合
	if (m_bFirstFrame) {
		m_bFirstFrame = false;
		//入力のロックを解除
		m_bLockChangeSelect = false;
	}
}

//=============================================================================
// 選択の変更
//=============================================================================
void CSelectMenu::ChangeSelect(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	CInput* pInput = pManager->GetInputCur();	//現在の入力デバイスへのポインタ
	if (pInput == nullptr) return;

	bool bChangeSelect = false;	//変更があったかどうか

	switch (m_typeSelect)
	{
		//上下
	case CSelectMenu::SELECT_TYPE::VERTICAL:
		//選択の変更の入力
		if (pInput->GetTrigger(CInput::CODE::SELECT_UP)) {
			m_nIdxCurSelect--;	//選択を１つ前に
			if (m_nIdxCurSelect < 0) m_nIdxCurSelect = m_nNumSelect - 1;	//０を下回った場合最大の番号
			bChangeSelect = true;
		}
		if (pInput->GetTrigger(CInput::CODE::SELECT_DOWN)) {
			m_nIdxCurSelect++;	//選択を１つ後に
			if (m_nIdxCurSelect >= m_nNumSelect) m_nIdxCurSelect = 0;	//最大を上回った場合０
			bChangeSelect = true;
		}
		break;
		//左右
	case CSelectMenu::SELECT_TYPE::HORIZONTAL:
		//選択の変更の入力
		if (pInput->GetTrigger(CInput::CODE::SELECT_LFET)) {
			m_nIdxCurSelect--;	//選択を１つ前に
			if (m_nIdxCurSelect < 0) m_nIdxCurSelect = m_nNumSelect - 1;	//０を下回った場合最大の番号
			bChangeSelect = true;
		}
		if (pInput->GetTrigger(CInput::CODE::SELECT_RIGHT)) {
			m_nIdxCurSelect++;	//選択を１つ後に
			if (m_nIdxCurSelect >= m_nNumSelect) m_nIdxCurSelect = 0;	//最大を上回った場合０
			bChangeSelect = true;
		}
		break;
	}

	//変更があったときの処理
	if (bChangeSelect) {
		BeginChangeSelect();	//変更開始時の処理を派生クラスで行う

		//マネージャーの取得
		CManager* pManager = CManager::GetManager();
		CSound* pSound = nullptr;	//サウンドへのポインタ
		//サウンドの取得
		if (pManager != nullptr) pSound = pManager->GetSound();
		//選択変更音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::SELECT_SWITCH);
	}
}

//=============================================================================
// メニューの背景の設定
//=============================================================================
void CSelectMenu::SetMenuBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight) {
	if (m_pMenuBG == nullptr) return;

	//背景の設定
	m_pMenuBG->SetTexType(typeTex);
	m_pMenuBG->SetPos(pos);
	m_pMenuBG->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));
}