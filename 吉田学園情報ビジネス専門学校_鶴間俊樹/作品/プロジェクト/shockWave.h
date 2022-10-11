//=============================================================================
//
// 衝撃波処理 [shockWave.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _SHOCKWAVE_H_
#define _SHOCKWAVE_H_

#include "main.h"
#include "meshCylinder.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjectList;

//*****************************************************************************
// 衝撃波クラス
//*****************************************************************************
class CShockwave : public CMeshcylinder
{
public:
	CShockwave();	//デフォルトコンストラクタ
	CShockwave(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fHeight);	//オーバーロードされたコンストラクタ
	virtual ~CShockwave();	//デストラクタ
	static CShockwave* Create(D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理

private:
	void Rotate(void);	//回転する
	void AttackCollision(int nObjtype);	//攻撃の当たり判定

	int m_nLife;	//消滅するまでのカウンター
	float m_fAddRadius;		//追加する半径
	float m_fAddHeight;		//追加する高さ
	float m_fRotateSpeed;	//回転速度
	CObjectList* m_pListAttacked;	//攻撃済みのオブジェクトのリスト
};

#endif // !_SHOCKWAVE_H_