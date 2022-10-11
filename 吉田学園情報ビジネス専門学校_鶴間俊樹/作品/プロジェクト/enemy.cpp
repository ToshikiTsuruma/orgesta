//=============================================================================
//
// エネミー処理 [enemy.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemy.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "player.h"
#include "terrain.h"
#include "effect.h"
#include "billboard.h"
#include "particleEffect.h"
#include "gauge3D.h"
#include "appleTree.h"
#include "apple_black.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define FALLDOWN_TIME (35)	//死亡時の倒れる時間
#define FINISH_TIME_CHANGE_COLOR_GAMEOVER (60)	//死亡後の色変更にかかる時間
#define START_CHANGE_COLOR_DELETE (180)	//死亡後の破棄開始時間
#define FINISH_TIME_DELETE (60)		//死亡後の破棄にかかる時間

#define GOLD_RATE (7)	//金の敵になる確率
#define DEFAULT_GROW_VALUE (1)	//デフォルトの敵死亡時の林檎の木成長量
#define GOLD_GROW_VALUE (10)		//金の敵死亡時の林檎の木成長量

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
bool CEnemy::m_bGoldRush = false;

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemy::CEnemy() : m_fMoveSpeed(0.0f), m_fRotateSpeed(0.0f), m_nMaxLife(0), m_fDistAttack(0.0f), m_fHeightLifeGauge(0.0f)
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CEnemy::CEnemy(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, float fMoveSpeed, float fRotSpeed, int nLife, float fDistAttack, float fHeightLifeGauge)
	: CObjectMotion(pPartsInfoArray, nNumParts, pMotionInfoArray, nNumTypeMotion, false), m_fMoveSpeed(fMoveSpeed), m_fRotateSpeed(fRotSpeed), m_nMaxLife(nLife), m_fDistAttack(fDistAttack), m_fHeightLifeGauge(fHeightLifeGauge)
{
	SetUpdatePriority(UPDATE_PRIORITY::ENEMY);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定
	SetObjType(OBJTYPE_ENEMY);	//オブジェクトタイプの設定

	SetColorOutlineAll(D3DXCOLOR(0.8f, 0.0f, 0.0f, 1.0f));	//輪郭の色の設定
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));	//輪郭の発光色の設定

	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotDestY = 0.0f;
	m_nLife = nLife;
	m_bDead = false;
	m_nCntDead = 0;
	m_nKillScore = 0;

	m_bLand = false;
	m_pAttackTarget = nullptr;
	m_bAttackDist = false;
	m_bGoldEnemy = false;

	//体力ゲージの生成
	m_pGaugeLife = CGauge3D::Create(m_nMaxLife, false, m_nLife, FPS * 10, false);
	//ゲージの背景の設定
	m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 55.0f, 7.0f);
	m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	//ゲージの設定
	m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 50.0f, 4.0f);
	m_pGaugeLife->SetGaugeColor(D3DXCOLOR(1.0f, 0.2f, 0.0f, 1.0f));
	//ゲージを非表示
	m_pGaugeLife->SetGaugeDraw(false);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemy::~CEnemy()
{

}

//=============================================================================
// エネミーの初期化処理
//=============================================================================
HRESULT CEnemy::Init(void) {
	//当たり判定あり
	SetEnableCollision(true);

	//ランダムで金色の敵
	int nRandGold = rand() % GOLD_RATE;
	if (nRandGold == 0 || m_bGoldRush) {
		m_bGoldEnemy = true;
		SetDiffuseModelAll(D3DXCOLOR(0.7f, 0.5f, 0.0f, 1.0f), 0);
		SetSpecularModelAll(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f), 0);
	}

	//モーションオブジェクトの初期化処理
	CObjectMotion::Init();
	return S_OK;
}

//=============================================================================
// エネミーの終了処理
//=============================================================================
void CEnemy::Uninit(void) {
	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//終了処理
	CObjectMotion::Uninit();
}

//=============================================================================
// エネミーの更新処理
//=============================================================================
void CEnemy::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//----------------------------
	//死亡時
	//----------------------------
	if (m_bDead) {
		m_nCntDead++;

		//重力
		SetPos(GetPos() + D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f));

		//地形との当たり判定
		bool bLand = false;	//接地しているかどうか
		D3DXVECTOR3 posColTerrain = GetPos();

		bLand = CTerrain::Collision(posColTerrain);
		//接地時
		if (bLand) {
			SetPos(posColTerrain);	//位置の移動
		}

		//倒れる時間
		if (m_nCntDead == FALLDOWN_TIME) {
			//倒れる音
			if (pSound != nullptr)pSound->PlaySound(CSound::SOUND_LABEL::STUN);

			//パーティクルエフェクトの生成
			D3DXVECTOR3 posParticle;	//パーティクルの位置
			GetPosCollision(&posParticle, D3DXVECTOR3(0.0f, 20.0f, 0.0f), 0);	//胴体の位置の取得

			//パーティクルエフェクトの最初の色
			D3DXCOLOR colEffectStart = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
			if (m_bGoldEnemy) colEffectStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

			//パーティクルの情報
			CParticleEffect::PARTICLE_INFO particleInfo = { 60, 25.0f, -0.25f, 2.0f, D3DXVECTOR3(0.0f, -0.01f, 0.0f), colEffectStart, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) };
			//エフェクトの生成
			CParticleEffect::Create(particleInfo, posParticle, START_CHANGE_COLOR_DELETE - FALLDOWN_TIME, 10, 0.05f * D3DX_PI, true);

			//色を白く変更
			StartChangeDiffuseAll(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), FINISH_TIME_CHANGE_COLOR_GAMEOVER);
		}

		//エネミーの色の変更開始
		if (m_nCntDead == START_CHANGE_COLOR_DELETE) {
			//透明にしていく
			StartChangeDiffuseAll(0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f), FINISH_TIME_DELETE);
		}

		//エネミーの破棄
		if (m_nCntDead >= START_CHANGE_COLOR_DELETE + FINISH_TIME_DELETE) {
			//終了処理
			Uninit();
			return;
		}

		//モーションの更新
		CObjectMotion::Update();
		return;
	}


	//----------------------------
	//回転
	//----------------------------
	D3DXVECTOR3 rot = GetRot();	//角度の取得

	//目標角度と角度が不一致の場合
	if (m_rotDestY != GetRot().y) {	//GetMotionCategory() != MOTION_CATEGORY::ATTACK && 

		//目標角度へ回転させる

		//目標角度がマイナス、角度がプラスのときに、減算した結果が-πを超えた場合
		if (m_rotDestY - rot.y < -D3DX_PI) {
			//加算
			rot.y += m_fRotateSpeed;
		}
		//目標角度がプラス、角度がマイナスのときに、減算した結果がπを超えた場合
		else if (m_rotDestY - rot.y > D3DX_PI) {
			//減算
			rot.y -= m_fRotateSpeed;
		}
		//個々の正負は関係ない場合
		else if (m_rotDestY - rot.y > 0) {
			//加算
			rot.y += m_fRotateSpeed;
			//目標角度超過時の調整
			if (rot.y > m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}
		else if (m_rotDestY - rot.y < 0) {
			//減算
			rot.y -= m_fRotateSpeed;
			//目標角度超過時の調整
			if (rot.y < m_rotDestY) {
				rot.y = m_rotDestY;
			}
		}


		//π超過時の調整
		if (rot.y > D3DX_PI) {
			rot.y -= D3DX_PI * 2;
		}
		else if (rot.y < -D3DX_PI) {
			rot.y += D3DX_PI * 2;
		}

		//角度の設定
		SetRot(rot);
	}

	//----------------------------
	//移動
	//----------------------------
	//移動
	if (m_bMove) {
		m_move.x = sinf(rot.y + D3DX_PI) * m_fMoveSpeed;
		m_move.z = cosf(rot.y + D3DX_PI) * m_fMoveSpeed;
		//攻撃可能距離の場合は動かない
		if (m_bAttackDist) {
			m_move.x = 0.0f;
			m_move.z = 0.0f;
		}
	}

	//位置の設定
	SetPos(GetPos() + m_move);

	//----------------------------
	//当たり判定
	//----------------------------
	//地面との当たり判定
	bool bLand = false;	//接地しているかどうか
	D3DXVECTOR3 posColTerrain, vecStart, vecEnd;	//接触位置、開始ベクトル、終了ベクトル
	vecStart = GetPos();	//敵の位置を取得
	vecEnd = vecStart;
	vecEnd.y += 1.0f;	//上向きのベクトル

	bLand = CTerrain::Collision(posColTerrain, vecStart, vecEnd);
	//接地時
	if (bLand) {
		m_bLand = true;
		m_move.y = -POWER_GRAVITY_GROUND;
		SetPos(posColTerrain);	//位置の移動

		//攻撃対象を探す	攻撃中で無く、攻撃可能距離でない場合探す
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && !m_bAttackDist) SearchAttackTarget();
		//攻撃目標の方向への角度を目標角度として設定
		SetRotToTarget();

		//魔術師敵が移動モーションなしにしているので移動不可時に接地していれば移動モーション
		if (!m_bMove && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
			//移動モーションの設定
			SetMoveMotion();
			m_bMove = true;
		}
	}
	else {
		m_bLand = false;
		//重力を掛ける
		m_move.y -= POWER_GRAVITY;
	}

	//落下死
	if (GetPos().y < DEAD_HEIGHT) {
		Uninit();
		return;
	}

	//攻撃可能な距離かどうか計算
	AttackDistance();

	//攻撃可能な角度かどうか
	bool bAngleAttack = fabsf(m_rotDestY - GetRot().y) <= D3DX_PI * 0.2f || fabsf(m_rotDestY - GetRot().y) >= D3DX_PI * 2.0f - D3DX_PI * 0.2f;

	//攻撃開始 （攻撃モーション中　攻撃距離　地上　攻撃できる角度）
	if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_bAttackDist && m_bLand && bAngleAttack) {
		Attack();
	}

	//モーションの更新
	CObjectMotion::Update();

	//モーション終了時
	if (GetEndMotion()) {
		MotionEnd();
	}

	//オブジェクトの全モデルのワールドマトリックスを更新
	UpdateMtxWorldAll();
	//モーション中の処理
	MotionAct();

	if (m_pGaugeLife != nullptr) {
		//体力ゲージの位置を更新
		m_pGaugeLife->SetAllGaugePos(D3DXVECTOR3(GetPos().x, GetPos().y + m_fHeightLifeGauge, GetPos().z));
	}
}

//=============================================================================
// エネミーの描画処理
//=============================================================================
void CEnemy::Draw(void) {
	CObjectMotion::Draw();
}

//=============================================================================
// 移動量の設定
//=============================================================================
void CEnemy::SetMove(D3DXVECTOR3 move) {
	m_move = move;
	//弾き飛ばされたりしてジャンプするような事があればニュートラルモーションを設定（移動モーション中は移動を固定されるため）
	if (m_move.y > 0.0f) {
		SetMotion(0);
		AttackStop();
		m_bMove = false;
	}
}

//=============================================================================
// エネミーのダメージ
//=============================================================================
void CEnemy::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	if (m_bDead) return;

	//ダメージ
	m_nLife -= nDamage;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//攻撃のタイプごとの処理
	switch (typeDamage)
	{
	case DAMAGE_TYPE::PLAYER_PUNCH:
	case DAMAGE_TYPE::SHOCKWAVE:
		//ダメージエフェクトの生成
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 40.0f, 40.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//ダメージエフェクトの生成
		CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f), CEffect::EFFECT_TYPE::THUNDER, 50.0f, 50.0f, false);
		//ダメージ音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		break;
	}

	//死亡判定
	if (m_nLife <= 0) {
		//死亡
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;

		//体力ゲージの設定
		if (m_pGaugeLife != nullptr) {
			m_pGaugeLife->SetGaugeValue(m_nLife);
		}
	}
}

//=============================================================================
// エネミーの死亡
//=============================================================================
void CEnemy::Dead(void) {
	//死亡の設定
	m_bDead = true;
	//死亡カウントの初期化
	m_nCntDead = 0;
	//当たり判定を無効
	SetEnableCollision(false);
	//死亡モーションの設定
	SetDeadMotion();
	//輪郭を非表示
	SetDrawOutlineAll(false);

	//体力ゲージの破棄
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	//ゲームシーンの取得
	CGameScene* pGame = nullptr;
	if (pManager != nullptr) pGame = pManager->GetGameScene();
	//林檎の木の取得
	CAppleTree* pAppleTree = nullptr;
	if (pGame != nullptr) {
		//倒したスコアの加算
		pGame->AddGameScore(m_nKillScore);
		//倒した敵の数の加算
		pGame->AddNumKillEnemy(1);
		//リンゴの木を取得
		pAppleTree = pGame->GetAppleTree();
	}

	//林檎の木を成長させる
	if (pAppleTree != nullptr) {
		int nGrowValue = DEFAULT_GROW_VALUE; //成長量
		//金の敵の場合多く成長
		if (m_bGoldEnemy) nGrowValue = GOLD_GROW_VALUE;
		//木を成長
		pAppleTree->AddGrow(nGrowValue);
	}

	/*
	//サウンドの取得
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//音の再生
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::EXPLOSION);
	*/
}

//=============================================================================
// 攻撃目標を探す
//=============================================================================
void CEnemy::SearchAttackTarget(void) {
	CObject* pLastTarget = m_pAttackTarget;	//最後のターゲット

	//最も近い木のオブジェクトを取得
	m_pAttackTarget = GetNearObject(GetPos(), ENEMY_ATTACK_TARGET , OBJFLAG_ENABLECOLLISION, nullptr, nullptr, nullptr);

	//攻撃対象外なくなった場合停止
	if (m_pAttackTarget == nullptr && pLastTarget != nullptr) {
		//動きを停止させる
		SetMotion(0);
		m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
		m_bMove = false;
	}
}

//=============================================================================
// 攻撃目標の方向への角度に設定
//=============================================================================
void CEnemy::SetRotToTarget(void) {
	if (m_pAttackTarget == nullptr) return;

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//ゲームシーンの取得
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;

	D3DXVECTOR3 posTarget = m_pAttackTarget->GetPos();	//攻撃対象の位置を取得

	//敵から攻撃対象への角度
	m_rotDestY = (float)atan2(posTarget.x - GetPos().x, posTarget.z - GetPos().z);

	//モデルが反対なので反対向きに向ける
	if (m_rotDestY >= 0.0f) {
		m_rotDestY -= D3DX_PI;
		if (m_rotDestY < D3DX_PI * -2.0f) m_rotDestY += D3DX_PI * 2.0f;
	}
	else if (m_rotDestY < 0.0f) {
		m_rotDestY += D3DX_PI;
		if (m_rotDestY > D3DX_PI * 2.0f) m_rotDestY -= D3DX_PI * 2.0f;
	}
}

//=============================================================================
// 攻撃可能な距離かどうかを計算
//=============================================================================
void CEnemy::AttackDistance(void) {
	if (m_pAttackTarget == nullptr) {
		m_bAttackDist = false;
		return;
	}

	D3DXVECTOR3 posTarget = m_pAttackTarget->GetPos();	//攻撃対象の位置を取得
	float fRadiusTarget = m_pAttackTarget->GetRadius();	//攻撃対象の半径
	float fDistTarget = D3DXVec2Length(&D3DXVECTOR2(posTarget.x - GetPos().x, posTarget.z - GetPos().z));	//攻撃対象と敵との距離

	//攻撃可能かどうかの判定
	m_bAttackDist = fDistTarget - fRadiusTarget <= m_fDistAttack;
}

//=============================================================================
//　攻撃の処理
//=============================================================================
void CEnemy::Attack(void) {
	//攻撃済みリストの初期化
	InitObjAttacked();
	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bMove = false;
	//攻撃開始
	AttackStart();
}