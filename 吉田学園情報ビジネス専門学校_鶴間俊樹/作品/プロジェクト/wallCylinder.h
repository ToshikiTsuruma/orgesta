//=============================================================================
//
// 円柱の壁の処理 [wallCylinder.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _WALL_CYLINDER_H_
#define _WALL_CYLINDER_H_

#include "main.h"
#include "meshcylinder.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 円柱の壁クラス
//*****************************************************************************
class CWallCylinder : public CMeshcylinder
{
public:
	CWallCylinder();	//デフォルトコンストラクタ
	CWallCylinder(D3DXVECTOR3 pos, float fRadius, float fHeight, CTexture::TEXTURE_TYPE typeTex, bool bClear);	//オーバーロードされたコンストラクタ
	~CWallCylinder();	//デストラクタ
	static CWallCylinder* Create(D3DXVECTOR3 pos, float fRadius, float fHeight, CTexture::TEXTURE_TYPE typeTex, bool bClear);	//生成処理
	static bool Collision(D3DXVECTOR3* pPos, D3DXVECTOR3 lastPos, float fRadius);	//当たり判定
	static void CrossPoint(D3DXVECTOR2* pPosCross, const D3DXVECTOR3* pPosStart, const D3DXVECTOR3* pPosEnd, float fDelta);	//壁と線分の交点
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理

private:
	void ChangeColorAlpha(void);	//透明度の変更

	bool m_bClear;	//透明になるかどうか
};

#endif // !_WALL_CYLINDER_H_