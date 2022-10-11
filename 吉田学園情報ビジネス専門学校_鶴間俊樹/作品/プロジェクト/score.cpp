//=============================================================================
//
// スコア処理 [score.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "score.h"
#include "object2D.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CScore::CScore()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CScore::CScore(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize)
{
	m_pos = pos;
	m_fSize = fSize;
	m_nScore = 0;

	SetTexType(type);	//保存用

	//桁数分の2Dオブジェクトを生成
	for (int nCnt = 0; nCnt < nNumDigit; nCnt++)
	{
		m_vNumberPtr.push_back(CObject2D::Create());
		if (m_vNumberPtr[nCnt] == nullptr) continue;

		//ナンバーの初期化
		m_vNumberPtr[nCnt]->SetTexType(type);
		m_vNumberPtr[nCnt]->SetPos(m_pos + D3DXVECTOR3(-m_fSize / 2.0f, m_fSize / 2.0f, 0.0f) + D3DXVECTOR3(-m_fSize, 0.0f, 0.0f) * (float)nCnt);	//スコア全体の右上がposとなるように
		m_vNumberPtr[nCnt]->SetSize(D3DXVECTOR3(m_fSize, m_fSize, 0.0f));
	}

	m_nDigitMin = 1;
	//m_nDigitMin = nNumDigit;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// スコアの生成処理
//=============================================================================
CScore* CScore::Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize) {
	CScore* pScore;
	pScore = new CScore(nNumDigit, type, pos, fSize);
	if (pScore == nullptr) return nullptr;

	pScore->Init();

	return pScore;
}

//=============================================================================
// スコアの初期化処理
//=============================================================================
HRESULT CScore::Init(void) {
	//数字の設定
	SetScore(m_nScore);

	return S_OK;
}

//=============================================================================
// スコアの終了処理
//=============================================================================
void CScore::Uninit(void) {
	for (unsigned int nCnt = 0; nCnt < m_vNumberPtr.size(); nCnt++) {
		if (m_vNumberPtr[nCnt] == nullptr) continue;
		//数字の破棄
		m_vNumberPtr[nCnt]->Uninit();
		m_vNumberPtr[nCnt] = nullptr;
	}

	//オブジェクトの破棄
	Release();
}

//=============================================================================
// スコアの更新処理
//=============================================================================
void CScore::Update(void) {
	//ナンバーの更新処理
	for (unsigned int nCnt = 0; nCnt < m_vNumberPtr.size(); nCnt++) {
		if (m_vNumberPtr[nCnt] == nullptr) continue;
		//更新処理
		(m_vNumberPtr[nCnt])->Update();
	}
}

//=============================================================================
// スコアの設定
//=============================================================================
void CScore::SetScore(int nScore) {
	m_nScore = nScore;

	//桁数の調整
	int nScoreDigit = std::to_string(m_nScore).length();		//スコアの桁数
	if (nScoreDigit < m_nDigitMin) nScoreDigit = m_nDigitMin;	//スコアの桁数が最小桁より少なければ置き換える
	int nSizeNumberVec = m_vNumberPtr.size();				//数字のベクターのサイズ
	int nDeltaDigit = nScoreDigit - nSizeNumberVec;			//スコアの桁数と現在の桁数の差分

	//桁数追加
	for (int nCnt = nSizeNumberVec; nCnt < nDeltaDigit + nSizeNumberVec; nCnt++)
	{
		//新しくナンバーを追加
		m_vNumberPtr.push_back(CObject2D::Create());

		//ナンバーの初期設定
		m_vNumberPtr[nCnt]->SetTexType(GetTexType());
		m_vNumberPtr[nCnt]->SetPos(m_pos + D3DXVECTOR3(-m_fSize / 2.0f, m_fSize / 2.0f, 0.0f) + D3DXVECTOR3(-m_fSize, 0.0f, 0.0f) * (float)nCnt);	//スコア全体の右上がposとなるように
		m_vNumberPtr[nCnt]->SetSize(D3DXVECTOR3(m_fSize, m_fSize, 0.0f));
	}

	//桁数削除
	if (nDeltaDigit < 0) {
		int nDeleteDigit = m_vNumberPtr.size() - m_nDigitMin;	//削除可能な桁数
		if (nDeleteDigit > abs(nDeltaDigit)) nDeleteDigit = abs(nDeltaDigit);	//削除する桁数が削除可能な最大数より小さい場合
		//大きい方の桁数から削除
		for (int nCnt = 0; nCnt < nDeleteDigit; nCnt++)
		{		
			//オブジェクトの終了処理
			if (m_vNumberPtr.back() != nullptr) m_vNumberPtr.back()->Uninit();
			//リストから削除
			m_vNumberPtr.pop_back();
		}
	}

	//UV座標の更新
	for (unsigned int nCnt = 0; nCnt < m_vNumberPtr.size(); nCnt++) {
		(m_vNumberPtr[nCnt])->SetTexNumber((m_nScore % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
	}
}

//=============================================================================
// スコアの追加
//=============================================================================
void CScore::AddScore(int nScore) {
	//スコアの設定
	SetScore(m_nScore + nScore);
}

//=============================================================================
// スコアの取得
//=============================================================================
int CScore::GetScore(void) {
	return m_nScore;
}