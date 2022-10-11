//=============================================================================
//
// 魔術師敵処理 [enemy_sorcerer.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemy_sorcerer.h"
#include "effect.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "appleTree.h"
#include "particleEffect.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_enemySorcerer.txt"
#define MOVE_SPEED (0.6f)				//移動速度
#define ROTATE_SPEED (0.01f * D3DX_PI)	//回転速度
#define NUM_COLLISION (5)				//当たり判定の数
#define COLLISION_RADIUS (30.0f)		//当たり判定の半径
#define MAX_LIFE (200)					//体力の最大値
#define DISTANCE_ATTACK (400.0f)		//攻撃する距離
#define ATTACK_EXPLOSION_DAMAGE (150)	//爆発の攻撃力
#define LIFE_GAUGE_HEIGHT (200.0f)		//敵の位置からの体力ゲージの高さ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CEnemySorcerer::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CEnemySorcerer::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CEnemySorcerer::m_aMotionInfo[(int)CEnemySorcerer::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemySorcerer::CEnemySorcerer() : CEnemy(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, MOVE_SPEED, ROTATE_SPEED, MAX_LIFE, DISTANCE_ATTACK, LIFE_GAUGE_HEIGHT)
{
	//倒したスコアの設定
	SetKillScore(2000);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemySorcerer::~CEnemySorcerer()
{

}

//=============================================================================
// 魔術師敵の生成処理
//=============================================================================
CEnemySorcerer* CEnemySorcerer::Create(D3DXVECTOR3 pos) {
	CEnemySorcerer* pEnemySorcerer;
	pEnemySorcerer = new CEnemySorcerer();
	if (pEnemySorcerer != nullptr) {
		pEnemySorcerer->SetPos(pos);
		pEnemySorcerer->Init();
	}

	return pEnemySorcerer;
}

//=============================================================================
// モーション情報をロード
//=============================================================================
void CEnemySorcerer::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// モーション情報をアンロード
//=============================================================================
void CEnemySorcerer::UnloadMotionInfo(void) {
	m_nNumParts = 0;

	//パーツ情報の解放
	if (m_pPartsInfoArray != nullptr) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = nullptr;
	}
	//モーション情報の解放
	for (int nCntMotionInfo = 0; nCntMotionInfo < (int)MOTION_TYPE::ENUM_MAX; nCntMotionInfo++)
	{
		for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
		{
			if (m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
				delete[] m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray;
				m_aMotionInfo[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
			}
		}
	}
}

//=============================================================================
// 魔術師敵の初期化処理
//=============================================================================
HRESULT CEnemySorcerer::Init(void) {
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//モーションの初期設定

	//敵の初期化処理
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
// 魔術師敵の終了処理
//=============================================================================
void CEnemySorcerer::Uninit(void) {
	//火の玉の破棄
	if (m_pFireBoll != nullptr) {
		if (m_pFireBoll != nullptr) {
			m_pFireBoll->Uninit();
			m_pFireBoll = nullptr;
		}
	}
	//終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 魔術師敵の更新処理
//=============================================================================
void CEnemySorcerer::Update(void) {
	//敵の更新
	CEnemy::Update();
}

//=============================================================================
// 魔術師敵の描画処理
//=============================================================================
void CEnemySorcerer::Draw(void) {
	CEnemy::Draw();
}

//=============================================================================
// 魔術師敵の当たり判定の情報の取得
//=============================================================================
void CEnemySorcerer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//必要なポインタがnullの場合終了
	if (pNumCol == nullptr || pRadiusCol == nullptr || ppPosColArray == nullptr) return;

	//posのポインタがすでに存在している場合終了
	if (*ppPosColArray != nullptr) return;

	//当たり判定の数を設定
	*pNumCol = NUM_COLLISION;
	//当たり判定の半径を設定
	*pRadiusCol = COLLISION_RADIUS;
	//当たり判定の位置の配列を設定
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];	//動的確保

	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルのposからのオフセットの始点
	int nIdxModel = 0;	//胴体

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		//当たり判定の位置の取得
		GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
		posOffset += D3DXVECTOR3(0.0f, 20.0f, 0.0f);	//オフセットを加算
	}
}

//=============================================================================
// モーションの区分の取得
//=============================================================================
CEnemy::MOTION_CATEGORY CEnemySorcerer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//ニュートラル
	case MOTION_TYPE::NEUTRAL:
	default:
		return MOTION_CATEGORY::NEUTRAL;

		//攻撃
	case MOTION_TYPE::ATTACK:
		return MOTION_CATEGORY::ATTACK;

		//死亡
	case MOTION_TYPE::DEAD:
		return MOTION_CATEGORY::DEAD;
	}
}

//=============================================================================
// 死亡時のモーションを設定
//=============================================================================
void CEnemySorcerer::SetDeadMotion(void) {
	//火の玉の破棄
	if (m_pFireBoll != nullptr) {
		if (m_pFireBoll != nullptr) {
			m_pFireBoll->Uninit();
			m_pFireBoll = nullptr;
		}
	}
	//死亡モーション設定
	SetMotion((int)MOTION_TYPE::DEAD);
}

//=============================================================================
// 攻撃開始時
//=============================================================================
void CEnemySorcerer::AttackStart(void) {
	//攻撃モーションの設定
	SetMotion((int)MOTION_TYPE::ATTACK);
}

//=============================================================================
// 攻撃停止
//=============================================================================
void CEnemySorcerer::AttackStop(void) {
	//火の玉の破棄
	if (m_pFireBoll != nullptr) {
		if (m_pFireBoll != nullptr) {
			m_pFireBoll->Uninit();
			m_pFireBoll = nullptr;
		}
	}
}

//=============================================================================
// モーション終了時
//=============================================================================
void CEnemySorcerer::MotionEnd(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//攻撃モーション
	case MOTION_TYPE::ATTACK:
		SetMotion((int)MOTION_TYPE::NEUTRAL);
		break;
	}
}

//=============================================================================
// モーション中の行動
//=============================================================================
void CEnemySorcerer::MotionAct(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//サウンドの取得
	CSound* pSound = pManager->GetSound();
	if (pSound == nullptr) return;
	//攻撃対象のオブジェクトの取得
	CObject* pObjTarget = GetAttackTarget();

	D3DXVECTOR3 posTarget;	//攻撃対象の位置

	switch ((MOTION_TYPE)GetMotionType())
	{
		//攻撃
	case MOTION_TYPE::ATTACK:

		if (GetCurKey() == 0 && !GetTransMotion()) {
			//最初のタイミング
			if (GetCurMotionCnt() == 0 && pObjTarget != nullptr) {				
				posTarget = pObjTarget->GetPos();//攻撃対象の位置を取得
				posTarget.y += 650.0f;	//ターゲットの上
				//火の玉生成
				m_pFireBoll = CEffect::Create(posTarget, CEffect::EFFECT_TYPE::FIRE_BOLL, 50.0f, 50.0f, true);
				//落下速度を設定
				if (m_pFireBoll != nullptr) m_pFireBoll->SetMove(D3DXVECTOR3(0.0f, -0.5f, 0.0f));
			}

			//焼く音を再生
			if (GetCurMotionCnt() == 10) pSound->PlaySound(CSound::SOUND_LABEL::DAMAGE_FIRE);

			//火の玉膨張
			if (m_pFireBoll != nullptr) m_pFireBoll->AddSize(D3DXVECTOR3(0.5f, 0.5f, 0.5f));
		}
		//火の玉膨張 & ダメージ
		if (GetCurKey() == 1) {
			if (GetCurMotionCnt() == 0) {
				//落下速度を設定
				if (m_pFireBoll != nullptr) m_pFireBoll->SetMove(D3DXVECTOR3(0.0f, -1.0f, 0.0f));
			}
			//サイズの膨張
			if (m_pFireBoll != nullptr) m_pFireBoll->AddSize(D3DXVECTOR3(2.0f, 2.0f, 0.0f));
			//じわじわダメージを与える
			if(pObjTarget != nullptr) pObjTarget->Damage(1, DAMAGE_TYPE::FIRE, nullptr);
		}
		//爆発
		if (GetCurKey() == 3 && GetCurMotionCnt() == 0) {
			if (m_pFireBoll != nullptr) {
				//爆発エフェクト
				CEffect::Create(m_pFireBoll->GetPos(), CEffect::EFFECT_TYPE::EXPLOSION, 300.0f, 300.0f, false);

				//パーティクルエフェクト
				CParticleEffect::PARTICLE_INFO particleInfo;
				particleInfo.addMove = D3DXVECTOR3(0.0f, -1.0f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
				particleInfo.fAddSize = -4.0f; particleInfo.fSizeStart = 120.0f; particleInfo.fSpeedMove = 10.0f; particleInfo.nLife = 30;
				//球状にパーティクルを生成
				CParticleEffect::EmitSphere(particleInfo, m_pFireBoll->GetPos() + D3DXVECTOR3(0.0f, 70.0f, 0.0f), 12, 12, -0.0f);

				//火の玉の破棄
				if (m_pFireBoll != nullptr) {
					m_pFireBoll->Uninit();
					m_pFireBoll = nullptr;
				}
			}
			//爆発ダメージ
			if (pObjTarget != nullptr) pObjTarget->Damage(ATTACK_EXPLOSION_DAMAGE, DAMAGE_TYPE::EXPLOSION, nullptr);
			//爆発音
			pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);
		}
		break;	//_攻撃
	}
}