//=============================================================================
//
// 2D選択メニュー処理 [selectMenu2D.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "selectMenu2D.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CSelectMenu2D::CSelectMenu2D() : CSelectMenu(0, false)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CSelectMenu2D::CSelectMenu2D(int nNumSelect, bool bUseBG) : CSelectMenu(nNumSelect , bUseBG)
{
	CreateSelectUI();
	m_pPosSelectIconOffsetArray = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CSelectMenu2D::~CSelectMenu2D()
{
	int nNumSelect = GetNumSelect();

	//UIモデルの破棄
	if (m_ppSelectUIArray != nullptr) {
		for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
		{
			m_ppSelectUIArray[nCntModel]->Uninit();
			m_ppSelectUIArray[nCntModel] = nullptr;
		}
	}
}

//=============================================================================
// 2D選択メニューの生成処理
//=============================================================================
CSelectMenu2D* CSelectMenu2D::Create(int nNumSelect, bool bUseBG) {
	CSelectMenu2D* pSelectMenu2D;
	pSelectMenu2D = new CSelectMenu2D(nNumSelect, bUseBG);
	if (pSelectMenu2D == nullptr) return nullptr;

	pSelectMenu2D->Init();

	return pSelectMenu2D;
}

//=============================================================================
// 2D選択メニューの初期化処理
//=============================================================================
HRESULT CSelectMenu2D::Init(void) {
	CSelectMenu::Init();
	return S_OK;
}

//=============================================================================
// 2D選択メニューの終了処理
//=============================================================================
void CSelectMenu2D::Uninit(void) {
	int nNumSelect = GetNumSelect();

	//選択肢UIの破棄
	if (m_ppSelectUIArray != nullptr) {
		for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
		{
			if (m_ppSelectUIArray[nCntModel] == nullptr) continue;
			m_ppSelectUIArray[nCntModel]->Uninit();
			m_ppSelectUIArray[nCntModel] = nullptr;
		}
		m_ppSelectUIArray = nullptr;
	}

	//選択中のアイコンの破棄
	if (m_pSelectIcon != nullptr) {
		m_pSelectIcon->Uninit();
		m_pSelectIcon = nullptr;
	}

	//選択のオフセットの破棄
	if (m_pPosSelectIconOffsetArray != nullptr) {
		delete[] m_pPosSelectIconOffsetArray;
		m_pPosSelectIconOffsetArray = nullptr;
	}

	CSelectMenu::Uninit();
}

//=============================================================================
// 2D選択メニューの更新処理
//=============================================================================
void CSelectMenu2D::Update(void) {


	CSelectMenu::Update();
}

//=============================================================================
// 選択肢UIの生成
//=============================================================================
void CSelectMenu2D::CreateSelectUI(void) {
	//すでに生成済みの場合終了
	if (m_ppSelectUIArray != nullptr) return;

	//選択肢の数の取得
	int nNumSelect = GetNumSelect();
	m_ppSelectUIArray = new CObject2D*[nNumSelect];

	//生成できなかった場合終了
	if (m_ppSelectUIArray == nullptr) return;

	for (int nCntModel = 0; nCntModel < nNumSelect; nCntModel++)
	{
		//選択肢UIの生成
		m_ppSelectUIArray[nCntModel] = CObject2D::Create();	//後に外部で設定するので値は適当
	}
}

//=============================================================================
// 選択の変更が開始されたときの処理
//=============================================================================
void CSelectMenu2D::BeginChangeSelect(void) {
	if (m_ppSelectUIArray == nullptr) return;

	//選択中のアイコンがある場合
	if (m_pSelectIcon != nullptr && m_pPosSelectIconOffsetArray != nullptr) {
		//現在の選択肢UIの位置を取得
		D3DXVECTOR3 posSelectUI;
		int nIdxSelect = GetIdxCurSelect();
		if (m_ppSelectUIArray[nIdxSelect] != nullptr) posSelectUI = m_ppSelectUIArray[nIdxSelect]->GetPos();

		//選択中アイコンを選択中の選択肢UIの隣に配置する
		m_pSelectIcon->SetPos(posSelectUI + m_pPosSelectIconOffsetArray[nIdxSelect]);
	}
	//選択中のアイコンがない場合
	else {
		//すべての選択肢を白に戻す
		for (int nCnt = 0; nCnt < GetNumSelect(); nCnt++)
		{
			if (m_ppSelectUIArray[nCnt] != nullptr) m_ppSelectUIArray[nCnt]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		//選択肢UIの色の変更
		int nIdxSelect = GetIdxCurSelect();
		if (m_ppSelectUIArray[nIdxSelect] != nullptr) m_ppSelectUIArray[nIdxSelect]->SetColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

//=============================================================================
// 選択肢UIの設定
//=============================================================================
void CSelectMenu2D::SetSelectUI(int nIdx, D3DXVECTOR3 pos, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex) {
	if (m_ppSelectUIArray == nullptr || nIdx < 0 || nIdx >= GetNumSelect()) return;	//配列から取得できない場合終了
	//設定するUIのオブジェクトがnullの場合終了
	if (m_ppSelectUIArray[nIdx] == nullptr) return;

	m_ppSelectUIArray[nIdx]->SetPos(pos);
	m_ppSelectUIArray[nIdx]->SetSize(D3DXVECTOR3(fWidth, fHeight, 0.0f));
	m_ppSelectUIArray[nIdx]->SetTexType(typeTex);
}

//=============================================================================
// 選択中アイコンの生成
//=============================================================================
void CSelectMenu2D::CreateSelectIcon(D3DXVECTOR3 posOffset, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex) {
	//すでに生成済みの場合は終了
	if (m_pSelectIcon != nullptr) return;

	//選択のオフセットが生成済みの場合破棄
	if (m_pPosSelectIconOffsetArray != nullptr) {
		delete[] m_pPosSelectIconOffsetArray;
		m_pPosSelectIconOffsetArray = nullptr;
	}

	const int nNumSelect = GetNumSelect();	//選択肢の数

	//選択中アイコンの生成
	m_pSelectIcon = CObject2D::Create(D3DXVECTOR3(), typeTex, fWidth, fHeight);

	//選択のオフセットの配列を生成
	m_pPosSelectIconOffsetArray = new D3DXVECTOR3[nNumSelect];

	//すべての選択のオフセットの設定
	if (m_pPosSelectIconOffsetArray != nullptr) {
		for (int nCnt = 0; nCnt < nNumSelect; nCnt++)
		{
			m_pPosSelectIconOffsetArray[nCnt] = posOffset;
		}
	}
	
	//選択中アイコンの位置の更新
	UpdateSelectIconPos();
}

//=============================================================================
// 選択アイコンの位置のオフセットを設定
//=============================================================================
void CSelectMenu2D::SetIconPosOffset(int nIdx, D3DXVECTOR3 posOffset) {
	const int nNumSelect = GetNumSelect();	//選択肢の数

	//nullの場合は終了
	if (m_pPosSelectIconOffsetArray == nullptr || nIdx < 0 || nIdx >= nNumSelect) return;

	//オフセットの設定
	m_pPosSelectIconOffsetArray[nIdx] = posOffset;

	//選択中アイコンの位置の更新
	UpdateSelectIconPos();
}

//=============================================================================
// 選択アイコンの位置を更新
//=============================================================================
void CSelectMenu2D::UpdateSelectIconPos(void) {
	//選択中アイコンの位置の設定
	if (m_ppSelectUIArray != nullptr && m_pSelectIcon != nullptr && m_pPosSelectIconOffsetArray != nullptr) {
		//現在の選択肢UIの位置を取得
		D3DXVECTOR3 posSelectUI;
		int nIdxSelect = GetIdxCurSelect();
		if (m_ppSelectUIArray[nIdxSelect] != nullptr) posSelectUI = m_ppSelectUIArray[nIdxSelect]->GetPos();

		//選択中アイコンを選択中の選択肢UIの隣に配置する
		m_pSelectIcon->SetPos(posSelectUI + m_pPosSelectIconOffsetArray[nIdxSelect]);
	}
}