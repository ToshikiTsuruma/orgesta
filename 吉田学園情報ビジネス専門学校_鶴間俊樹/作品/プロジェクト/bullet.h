//=============================================================================
//
// 弾処理 [bullet.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _BULLET_H_
#define _BULLET_H_

#include "objectModel.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************

//*****************************************************************************
//前方宣言
//*****************************************************************************
class CObjectList;

//*****************************************************************************
// 弾クラス
//*****************************************************************************
class CBullet : public CObjectModel
{
public:
	CBullet();	//デフォルトコンストラクタ
	CBullet(CModel::MODELTYPE typeModel);		//オーバーロードされたコンストラクタ
	~CBullet();	//デストラクタ
	static CBullet* Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nObjtypeAttack, float fRadiusCol, int nDamage);	//生成処理
	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetParticleInfo(int nLife, float fSize, float fAddSize, D3DXCOLOR colStart, D3DXCOLOR colEnd);	//パーティクル情報を設定
	void SetNumParticle(int nNum) { m_nNumParticle = nNum; }	//補完のパーティクルの数の設定

private:
	bool AttackCollision(void);	//攻撃対象への当たり判定

	D3DXVECTOR3 m_move;	//移動量
	int m_nLife;	//ライフ
	int m_nObjtypeAttack;	//攻撃対象のオブジェクトタイプ
	float m_fRadiusCol;	//当たり判定の半径
	int m_nDamage;	//弾のダメージ
	CObjectList* m_pListAttacked;	//攻撃済みのオブジェクトのリスト

	//軌道パーティクルの情報
	bool m_bCreateParticle;	//パーティクル生成するかどうか
	int m_nLifeParticle;	//パーティクルのライフ
	float m_fSizeParticle;	//パーティクルのサイズ加算量
	float m_fAddSizeParticle;	//パーティクルのサイズ加算量
	D3DXCOLOR m_colStartParticle;	//パーティクルの開始色
	D3DXCOLOR m_colEndParticle;		//パーティクルの終了色
	//パーティクルの補完
	bool m_bCreateParticleFirst;	//パーティクルの初回の生成が終わったかどうか
	int m_nNumParticle;				//追加されるパーティクルの数
	D3DXVECTOR3 m_posLastParticle;	//前回のパーティクル生成位置
};

#endif // !_BULLET_H_