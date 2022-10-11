//=============================================================================
//
// 生贄生成処理 [scapegoatCreater.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SCAPEGOAT_CREATER_H_
#define _SCAPEGOAT_CREATER_H_

#include "main.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CScore;
class CBillboard;
class CObject2D;

//*****************************************************************************
// 生贄生成クラス
//*****************************************************************************
class CScapegoatCreater : public CObject
{
public:
	CScapegoatCreater();	//デフォルトコンストラクタ
	~CScapegoatCreater();	//デストラクタ

	static CScapegoatCreater* Create(void);	//生成処理
	HRESULT Init(void);		//初期化処理
	void Uninit(void);		//終了処理
	void Update(void);		//更新処理
	void Draw(void) {}		//描画処理

	void AddNumKillEnemy(int nNum);	//倒した敵の数を増やす

private:
	int m_nNumKillEnemy;	//倒した敵の数
	CScore* m_pNumKillEnemyUI;	//倒した敵の数のUI
	CScore* m_pNumUseEnemyUI;	//使用する敵の数のUI
	CBillboard* m_pInteractUI;	//生贄生成時のインタラクトボタンのビルボード
	CBillboard* m_pNixSignUI;	//禁止マークのビルボード
	CObject2D* m_pTextEnergy;	//エネルギーのテキスト

};

#endif // !_SCAPEGOAT_CREATER_H_