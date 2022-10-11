//=============================================================================
//
// 斥力波処理 [repulsiveWave.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _REPULSIVEWAVE_H_
#define _REPULSIVEWAVE_H_

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
// 斥力波クラス
//*****************************************************************************
class CRepulsivewave : public CMeshcylinder
{
public:
	CRepulsivewave();	//デフォルトコンストラクタ
	CRepulsivewave(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside);	//オーバーロードされたコンストラクタ
	virtual ~CRepulsivewave();	//デストラクタ
	static CRepulsivewave* Create(D3DXVECTOR3 pos, float fRadiusInside, float fRadiusOutside, float fAddRadius, int nLife, float fRotSpeed);	//生成処理
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理

private:
	void Rotate(void);	//回転する
	void AttackCollision(int nObjtype);	//攻撃の当たり判定

	int m_nLife;	//消滅するまでのカウンター
	float m_fAddRadius;		//追加する半径
	float m_fRotateSpeed;	//回転速度
	CObjectList* m_pListAttacked;	//攻撃済みのオブジェクトのリスト
};

#endif // !_REPULSIVEWAVE_H_