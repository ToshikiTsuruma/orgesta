//=============================================================================
//
// 弾処理 [bullet.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "bullet.h"
#include "particle.h"
#include "manager.h"
#include "sound.h"
#include "effect.h"
#include "objectList.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_ATTACK_DISTANCE (500.0f)	//オブジェクトに攻撃可能な最大の距離

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CBullet::CBullet()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CBullet::CBullet(CModel::MODELTYPE typeModel) : CObjectModel(typeModel, false)
{
	m_bCreateParticleFirst = false;
	//オブジェクトリストの生成
	m_pListAttacked = new CObjectList;
	m_nNumParticle = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CBullet::~CBullet()
{
	//オブジェクトリストの破棄
	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// 弾の生成処理
//=============================================================================
CBullet* CBullet::Create(CModel::MODELTYPE typeModel, D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, int nObjtypeAttack, float fRadiusCol, int nDamage) {
	CBullet* pBullet;
	pBullet = new CBullet(typeModel);
	if (pBullet == nullptr) return nullptr;

	pBullet->SetPos(pos);
	pBullet->m_move = move;
	pBullet->m_nLife = nLife;
	pBullet->m_nObjtypeAttack = nObjtypeAttack;
	pBullet->m_fRadiusCol = fRadiusCol;
	pBullet->m_nDamage = nDamage;

	pBullet->Init();

	return pBullet;
}

//=============================================================================
// 弾の初期化処理
//=============================================================================
HRESULT CBullet::Init(void) {
	m_bCreateParticleFirst = false;

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// 弾の終了処理
//=============================================================================
void CBullet::Uninit(void) {

	CObjectModel::Uninit();
}

//=============================================================================
// 弾の更新処理
//=============================================================================
void CBullet::Update(void) {
	//ライフの減少
	if (m_nLife <= 0) {
		//終了処理
		Uninit();
		return;
	}
	else {
		m_nLife--;
	}

	//弾の移動
	SetPos(GetPos() + m_move);

	//当たり判定
	bool bHit = AttackCollision();
	////当たっている場合
	//if (bHit) {
	//	//終了処理
	//	Uninit();
	//	return;
	//}

	//パーティクル生成
	if (m_bCreateParticle) {
		D3DXVECTOR3 posBullet = GetPos();

		//補完用パーティクルの生成
		if (m_bCreateParticleFirst) {
			for (int nCnt = 0; nCnt < m_nNumParticle; nCnt++)
			{
				float fRatio = (nCnt + 1) / (float)(m_nNumParticle + 1);	//前回と今回のエフェクトの差分の割合
				D3DXVECTOR3 posParticle = posBullet + (m_posLastParticle - posBullet) * fRatio;	//補完パーティクルの位置
				//パーティクルの生成
				CParticle::Create(posParticle, m_nLifeParticle, m_fSizeParticle + (m_fAddSizeParticle * fRatio), m_fAddSizeParticle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_colStartParticle, m_colEndParticle);
			}
		}

		CParticle::Create(posBullet, m_nLifeParticle, m_fSizeParticle, m_fAddSizeParticle, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_colStartParticle, m_colEndParticle);
		//今回の生成位置を保存
		m_posLastParticle = posBullet;
		m_bCreateParticleFirst = true;
	}

	CObjectModel::Update();
}

//=============================================================================
// 弾の描画処理
//=============================================================================
void CBullet::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// パーティクル情報を設定
//=============================================================================
void CBullet::SetParticleInfo(int nLife, float fSize, float fAddSize, D3DXCOLOR colStart, D3DXCOLOR colEnd) {
	m_bCreateParticle = true;

	m_nLifeParticle = nLife;
	m_fSizeParticle = fSize;
	m_fAddSizeParticle = fAddSize;
	m_colStartParticle = colStart;
	m_colEndParticle = colEnd;
}

//=============================================================================
// 攻撃対象への当たり判定
//=============================================================================
bool CBullet::AttackCollision(void) {
	CObject* pObject = GetObjectTopAll();	//全オブジェクトのリストの先頭を取得
	D3DXVECTOR3 posBullet = GetPos();	//弾の位置

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//リストの次のオブジェクトのポインタを取得

		//オブジェクトタイプの確認
		bool bMatchType = false;
		if (pObject->GetObjType() & m_nObjtypeAttack) bMatchType = true;

		//死亡状態の取得
		bool bDeath = pObject->GetDeath();

		//当たり判定可能状態の取得
		bool bEnableCollision = pObject->GetEnableCollision();

		//攻撃可能な範囲にいるかどうか
		D3DXVECTOR2 vecObj = D3DXVECTOR2(pObject->GetPos().x - posBullet.x, pObject->GetPos().z - posBullet.z);	//攻撃位置とオブジェクトのベクトル
		float fDistObj = D3DXVec2Length(&vecObj);	//攻撃位置とオブジェクトの距離

		//すでに攻撃が当たっている場合
		bool bAttacked = false;	//すでに攻撃されているかどうか

		if (m_pListAttacked != nullptr) {
			//リストにすでに追加されている場合
			bAttacked = m_pListAttacked->MatchObject(pObject);
		}

		//次のループに飛ばす
		if (!bMatchType || bDeath || !bEnableCollision || fDistObj > MAX_ATTACK_DISTANCE || bAttacked) {
			pObject = pObjNext;	//リストの次のオブジェクトを代入
			continue;	
		}

		//---------------------------
		//当たり判定
		//---------------------------
		bool bCollision = false;	//当たり判定のフラグ
		D3DXVECTOR3 posHit;	//当たった位置

		//当たり判定があるオブジェクトの全モデルのワールドマトリックスを更新
		pObject->UpdateMtxWorldAll();	//当たり判定取得時に位置のズレが生じるため

		//当たり判定があるパーツの数の取得
		int nNumColParts = pObject->GetNumCollisionParts();

		//パーツごとに当たり判定
		for (int nCntColParts = 0; nCntColParts < nNumColParts; nCntColParts++)
		{
			int nNumCollision = 0;		//当たり判定の数
			D3DXVECTOR3* pPosColArray = nullptr;	//当たり判定の位置の配列のポインタ
			float fRadiusCol = 0.0f;	//当たり判定の半径

			//当たり判定の情報の取得
			pObject->GetCollisionInfo(nCntColParts, &nNumCollision, &pPosColArray, &fRadiusCol);

			//位置情報のメモリが確保されていない場合終了
			if (pPosColArray == nullptr) continue;

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//球と球の当たり判定
				D3DXVECTOR3 vecCol = pPosColArray[nCntCol] - posBullet;	//弾から当たり判定の球へのベクトル
				float fDistCol = D3DXVec3Length(&vecCol);	//当たり判定同士の距離
				//距離と当たり判定同士の距離を比較
				if (fDistCol < m_fRadiusCol + fRadiusCol) {
					bCollision = true;
					posHit = pPosColArray[nCntCol];
					break;
				}
			}

			//位置情報の破棄
			delete[] pPosColArray;		

			//すでに当たっていたら当たり判定のループ終了
			if (bCollision) break;
		}

		//---------------------------
		//当たり判定時の処理
		//---------------------------
		if (bCollision) {
			//オブジェクトにダメージを与える
			pObject->Damage(m_nDamage, DAMAGE_TYPE::BULLET, nullptr);

			//マネージャーの取得
			CManager* pManager = CManager::GetManager();
			//サウンドの取得
			CSound *pSound = nullptr;
			if (pManager != nullptr) pSound = pManager->GetSound();

			//攻撃エフェクトの生成
			CEffect::Create(posHit, CEffect::EFFECT_TYPE::DAMAGE_BULLET, 30.0f, 30.0f, false);
			//ダメージ音の再生
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_BULLET);

			//オブジェクトが死亡していない場合攻撃済みリストに追加
			if (!pObject->GetDeath()) {
				if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);
			}
		}

		pObject = pObjNext;	//リストの次のオブジェクトを代入
	}

	return false;
}