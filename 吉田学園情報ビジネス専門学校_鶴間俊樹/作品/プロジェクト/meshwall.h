//=============================================================================
//
// メッシュウォール処理 [meshwall.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MESHWALL_H_
#define _MESHWALL_H_

#include "main.h"
#include "object3D.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// メッシュウォールクラス
//*****************************************************************************
class CMeshwall : public CObject3D
{
public:
	CMeshwall();	//デフォルトコンストラクタ
	virtual ~CMeshwall();	//デストラクタ
	static CMeshwall* Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nNumX, int nNumY, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	int m_nNumX;	//X方向のポリゴンの数
	int m_nNumY;	//Y方向のポリゴンの数
	float m_fWidth;	//ポリゴンの幅
	float m_fHeight;//ポリゴンの高さ
};

#endif // !_MESHWALL_H_