//=============================================================================
//
// スコア処理 [score.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// スコアクラス
//*****************************************************************************
class CScore : public CObject
{
public:
	CScore();	//デフォルトコンストラクタ
	CScore(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//オーバーロードされたコンストラクタ
	~CScore();	//デストラクタ
	static CScore* Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void) {}	//描画処理
	void SetScore(int nScore);	//スコアの設定
	void AddScore(int nScore);	//スコアの追加
	int GetScore(void);	//スコアを取得
	void SetDigitMin(int nDigit) { m_nDigitMin = nDigit; }	//表示される桁数の最小数を設定

private:
	std::vector<CObject2D*> m_vNumberPtr;	//数字のオブジェクトのポインタ配列
	D3DXVECTOR3 m_pos;	//スコアの右上の位置
	float m_fSize;		//スコアのナンバーのサイズ
	int m_nScore;		//スコアの値
	int m_nDigitMin;	//表示される桁数の最小数
};

#endif // !_SCORE_H_