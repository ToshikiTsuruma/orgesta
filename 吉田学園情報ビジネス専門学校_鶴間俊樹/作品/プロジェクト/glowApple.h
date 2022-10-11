//=============================================================================
//
// グローアップル処理 [glowApple.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _GLOW_APPLE_H_
#define _GLOW_APPLE_H_

#include "main.h"
#include "objectModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define GLOW_APPLE_EMISSIVE (D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f))

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CAppleTree;
class CParticle;

//*****************************************************************************
// グローアップルクラス
//*****************************************************************************
class CGlowApple : public CObjectModel
{
public:
	enum class APPLE_TYPE {
		RED = 0,
		GREEN,
		WHITE,
		BLACK,
		SILVER,
		GOLD,
		ENUM_MAX
	};

	CGlowApple();		//デフォルトコンストラクタ
	CGlowApple(CAppleTree* pTree);		//オーバーロードされたコンストラクタ
	virtual ~CGlowApple();		//デストラクタ
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	void Dead(void);		//死亡時処理
	CAppleTree* GetAppleTree(void) { return m_pAppleTree; }	//この林檎がなる林檎の木の取得
	void CreateGlowParticle(APPLE_TYPE type);	//発光表現用パーティクルの生成

	static D3DXCOLOR GetAppleColor(APPLE_TYPE type);	//種類ごとの林檎の色を取得
	static void GetAppleText(char* pStr, APPLE_TYPE type);		//種類ごとの林檎の説明テキスト

private:
	CAppleTree* m_pAppleTree;	//この林檎がなる林檎の木
	CParticle* m_pGlowParticle;	//発光表現のためのパーティクル
};

#endif // !_GLOW_APPLE_H_