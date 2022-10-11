//=============================================================================
//
// 2D選択メニュー処理 [selectMenu2D.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SELECT_MENU_2D_H_
#define _SELECT_MENU_2D_H_

#include "selectMenu.h"
#include "object2D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************

//*****************************************************************************
// 2D選択メニュークラス
//*****************************************************************************
class CSelectMenu2D : public CSelectMenu
{
public:

	CSelectMenu2D();		//デフォルトコンストラクタ
	CSelectMenu2D(int nNumSelect, bool bUseBG);	//オーバーロードされたコンストラクタ
	virtual ~CSelectMenu2D();		//デストラクタ
	static CSelectMenu2D* Create(int nNumSelect, bool bUseBG);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void) {}	//描画処理

	virtual void BeginChangeSelect(void);	//選択の変更が開始されたときの処理
	virtual void EndChangeSelect(void) {}	//選択の変更が終了したときの処理

	void SetSelectUI(int nIdx, D3DXVECTOR3 pos, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex);	//選択肢UIの設定
	void CreateSelectIcon(D3DXVECTOR3 posOffset, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex);	//選択中アイコンの生成
	void SetIconPosOffset(int nIdx, D3DXVECTOR3 posOffset);	//選択アイコンの位置のオフセットを設定

private:
	void CreateSelectUI(void);	//UIを生成
	void UpdateSelectIconPos(void);	//選択アイコンの位置を更新

	CObject2D** m_ppSelectUIArray;	//選択肢のUIの動的配列

	CObject2D* m_pSelectIcon;	//選択中の選択肢の隣に生成されるアイコン
	D3DXVECTOR3* m_pPosSelectIconOffsetArray;	//選択中アイコンの選択肢からのオフセット位置の動的配列のポインタ
};

#endif // !_SELECT_MENU_3D_H_