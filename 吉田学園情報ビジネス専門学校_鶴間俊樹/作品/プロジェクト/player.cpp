//=============================================================================
//
// プレイヤー処理 [player.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"
#include "sound.h"
#include "input.h"
#include "gameScene.h"

#include "wallCylinder.h"
#include "terrain.h"
#include "effect.h"
#include "particle.h"
#include "particleEffect.h"
#include "object2D.h"
#include "billboard.h"
#include "appleTree.h"

#include "shockWaveEmitter.h"
#include "thunderEmitter.h"

//=============================================================================
// マクロ定義
//=============================================================================
//--------------------------------
//ファイル名
//--------------------------------
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_player.txt"

//--------------------------------
//移動
//--------------------------------
#define ADD_MOVE_SPEED (1.2f)			//加速
#define DEC_MOVE_SPEED_GROUND (0.8f)	//地上での減速
#define DEC_MOVE_SPEED_AIR (0.05f)		//空中での減速
#define MAX_MOVE_SPEED_WALK (10.0f)		//歩行速度
//#define MAX_MOVE_SPEED_DASH (12.0f)		//走行速度
#define MAX_MOVE_SPEED_AIR (2.0f)		//空中速度
#define ROTATE_SPEED (0.1f)				//回転速度

//--------------------------------
//当たり判定
//--------------------------------
#define COLLISION_RADIUS (70.0f)		//当たり判定の半径	壁とかに使う
#define NUM_COLLISION (10)				//当たり判定の数

//--------------------------------
//攻撃
//--------------------------------
#define ATTACK_DAMAGE_PUNCH (60)	//パンチの攻撃力
#define ATTACK_DAMAGE_STANP (180)	//踏みつけの攻撃力

//--------------------------------
//ゲームオーバー時
//--------------------------------
#define FINISH_TIME_CHANGE_COLOR_GAMEOVER (120)	//ゲームオーバー後の色変更にかかる時間
#define START_CHANGE_COLOR_CLEAR (180)	//ゲームオーバー後の透明色へ変更する開始時間
#define FINISH_TIME_CLEAR (60)			//ゲームオーバー後の透明色への変更にかかる時間

//--------------------------------
//その他
//--------------------------------
#define COLOR_OUTLINE (D3DXCOLOR(0.2f, 0.5f, 1.0f, 1.0f))	//モデルの輪郭の色

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CPlayer::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CPlayer::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CPlayer::m_aMotionInfo[(int)CPlayer::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CPlayer::CPlayer() : CObjectMotion(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, false)
{
	SetObjType(OBJTYPE_PLAYER);	//オブジェクトタイプの設定
	SetUpdatePriority(UPDATE_PRIORITY::PLAYER);	//更新順の設定
	SetDrawPriority(DRAW_PRIORITY::CHARA);	//描画順の設定

	SetColorOutlineAll(COLOR_OUTLINE);	//輪郭の色の設定
	SetDiffuseModelAll(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), 0);	//モデルの色の設定
	SetColorGlowAll(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));	//輪郭の発光色の設定

	m_lastPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_destRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fAddMoveSpeed = 0.0f;
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_bLand = false;
	m_nAddDamage = 0;
	m_bValidAttack = false;
	m_nNumKillEnemy = 0;
	m_nCntLockAct = 0;

	m_nCntGameover = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CPlayer::~CPlayer()
{

}

//=============================================================================
// プレイヤーの生成処理
//=============================================================================
CPlayer* CPlayer::Create(D3DXVECTOR3 pos) {
	CPlayer* pPlayer;
	pPlayer = new CPlayer();
	if (pPlayer == nullptr) return nullptr;

	pPlayer->CObjectMotion::SetPos(pos);
	pPlayer->Init();

	return pPlayer;
}

//=============================================================================
// モーション情報をロード
//=============================================================================
void CPlayer::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// モーション情報をアンロード
//=============================================================================
void CPlayer::UnloadMotionInfo(void) {
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
// プレイヤーの初期化処理
//=============================================================================
HRESULT CPlayer::Init(void) {
	//プレイヤーの初期設定
	CObjectMotion::SetRot(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	m_destRot.y =  D3DX_PI;	//奥向き

	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CObjectMotion::Init();
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//モーションの初期設定

	return S_OK;
}

//=============================================================================
// プレイヤーの終了処理
//=============================================================================
void CPlayer::Uninit(void) {
	//終了処理
	CObjectMotion::Uninit();
}

//=============================================================================
// プレイヤーの更新処理
//=============================================================================
void CPlayer::Update(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();

	CInput* pInput = nullptr;
	CSound *pSound = nullptr;
	CCamera* pCamera = nullptr;
	CGameScene* pGame = nullptr;

	if (pManager != nullptr) {
		//現在の入力デバイスの取得
		pInput = pManager->GetInputCur();
		//サウンドの取得
		pSound = pManager->GetSound();
		//カメラの取得
		pCamera = pManager->GetCamera();
		//ゲームシーンの取得
		pGame = pManager->GetGameScene();
	}

	//位置情報のポインタの取得
	D3DXVECTOR3* pPosPlayer = GetPtrPos();

	
	//----------------------------
	//ゲームオーバー時
	//----------------------------
	if (pGame != nullptr) {
		//ゲームクリア時
		if (pGame->GetGameClear()) {
			//攻撃等モーション終了時
			if (GetEndMotion()) {
				EndMotion();
			}
			//モーションのアップデート
			CObjectMotion::Update();
			return;
		}
		//ゲームオーバー時
		if (pGame->GetGameOver()) {
			SetMotion((int)MOTION_TYPE::NEUTRAL);	//ニュートラルから動かないように
			//モーションのアップデート
			CObjectMotion::Update();	//モデルの更新が必要なため

			//すべて終わったら更新なし
			if (m_nCntGameover >= START_CHANGE_COLOR_CLEAR + FINISH_TIME_CLEAR) return;

			//カウンターの加算
			m_nCntGameover++;

			//透明の変更を開始
			if (m_nCntGameover == START_CHANGE_COLOR_CLEAR) {
				//プレイヤーの色を透明にする
				StartChangeDiffuseAll(0, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f), FINISH_TIME_CLEAR);
			}
			return;
		}
	}
	//ゲーム以外
	else {
		//モーションのアップデート
		CObjectMotion::Update();
		return;
	}

	//----------------------------
	//カメラの設定
	//----------------------------
	float fRotCameraY = 0.0f;	//カメラの角度
	if (pCamera != nullptr) {
		fRotCameraY = pCamera->GetRot().y;	//カメラの角度を取得
	}

	//----------------------------
	//硬直中
	//----------------------------
	if (m_nCntLockAct > 0) {
		m_nCntLockAct--;
	}

	//----------------------------
	//攻撃
	//----------------------------
	if (pInput != nullptr) {
		if (GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_nCntLockAct <= 0) {
			StartAttack();
		}
	}

	//----------------------------
	//移動
	//----------------------------
	//落下
	if (!m_bLand) {
		m_move.y -= POWER_GRAVITY;	//重力を加算
	}

	//移動と回転
	if (pInput != nullptr && GetMotionCategory() != MOTION_CATEGORY::ATTACK && m_nCntLockAct <= 0) {
		Move(pInput, fRotCameraY);
	}

	//減速
	DecMove();

	//----------------------------
	//移動の反映
	//----------------------------
	//最後の位置座標の保存
	m_lastPos = *pPosPlayer;
	//移動
	*pPosPlayer += m_move;

	//----------------------------
	//当たり判定
	//----------------------------
	Collision(*pPosPlayer);

	//----------------------------
	//モーションの更新
	//----------------------------
	CObjectMotion::Update();

	//----------------------------
	//モーション終了時
	//----------------------------
	if (GetEndMotion()) {
		EndMotion();
	}

	//----------------------------
	//モーション中の処理
	//----------------------------
	MotionAction();

	//----------------------------
	//攻撃のタイミング
	//----------------------------
	if (m_bValidAttack) {
		//プレイヤーの全モデルのワールドマトリックスを更新
		UpdateMtxWorldAll();	//当たり判定取得時に位置のズレが生じるため

		switch ((MOTION_TYPE)GetMotionType())
		{
		case MOTION_TYPE::PUNCH:
			AttackCollision(2, 4, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(-50.0f, 0.0f, 0.0f), ATTACK_DAMAGE_PUNCH + m_nAddDamage);
			AttackCollision(4, 4, 50.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(50.0f, 0.0f, 0.0f), ATTACK_DAMAGE_PUNCH + m_nAddDamage);
			break;

		case MOTION_TYPE::STAMP:
			AttackCollision(6, 3, 50.0f, D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, -30.0f, 0.0f), ATTACK_DAMAGE_STANP + m_nAddDamage);
			AttackCollision(8, 3, 50.0f, D3DXVECTOR3(0.0f, 0.0f, -30.0f), D3DXVECTOR3(0.0f, -30.0f, 0.0f), ATTACK_DAMAGE_STANP + m_nAddDamage);
			break;
		}
	}
}

//=============================================================================
// プレイヤーの描画処理
//=============================================================================
void CPlayer::Draw(void) {
	if (m_nCntGameover >= START_CHANGE_COLOR_CLEAR + FINISH_TIME_CLEAR) return;	//フェード完了時は描画しない 影や透過の影響をなくすため

	CObjectMotion::Draw();
}


//=============================================================================
// プレイヤーのゲームクリア時の処理
//=============================================================================
void CPlayer::GameClear(void) {
	//ニュートラルモーションの設定
	if(GetMotionCategory() != MOTION_CATEGORY::ATTACK) SetMotion((int)MOTION_TYPE::NEUTRAL);

	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_nCntLockAct = 0;
}

//=============================================================================
// プレイヤーのゲームオーバー時の処理
//=============================================================================
void CPlayer::GameOver(void) {
	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);
	m_nCntLockAct = 0;

	//プレイヤーの色を黒くする
	StartChangeDiffuseAll(0, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), FINISH_TIME_CHANGE_COLOR_GAMEOVER);

	//パーティクルエフェクトの生成
	CParticleEffect::PARTICLE_INFO particleInfo;	//パーティクル情報
	particleInfo.addMove = D3DXVECTOR3(0.0f, 0.08f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -1.0f; particleInfo.fSizeStart = 60.0f; particleInfo.fSpeedMove = 3.5f; particleInfo.nLife = 60;
	//エフェクトを生成
	CParticleEffect::Create(particleInfo, GetPos(), FINISH_TIME_CHANGE_COLOR_GAMEOVER, 1, 0.45f * D3DX_PI, true);
}

//=============================================================================
// プレイヤーの当たり判定の情報の取得
//=============================================================================
void CPlayer::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//当たり判定の数を設定
	if (pNumCol != nullptr) *pNumCol = NUM_COLLISION;
	//当たり判定の半径を設定
	if (pRadiusCol != nullptr) *pRadiusCol = COLLISION_RADIUS;
	//当たり判定の位置の配列のポインタがnullの場合終了
	if (ppPosColArray == nullptr) {
		return;
	}
	//posのポインタが空ではない場合終了
	if (*ppPosColArray != nullptr) {
		return;
	}

	//posの変数を当たり判定の数分配列で確保
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];

	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//モデルのposからのオフセット
	int nIdxModel = 0;	//胴体

	//当たり判定の位置の取得
	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
		posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//オフセットを加算
	}
}

//=============================================================================
// プレイヤーの最後の位置座標の取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetLastPos(void) {
	return m_lastPos;
}

//=============================================================================
// プレイヤーの移動量の設定
//=============================================================================
void CPlayer::SetMove(D3DXVECTOR3 move) { m_move = move; }

//=============================================================================
// プレイヤーの移動量の取得
//=============================================================================
D3DXVECTOR3 CPlayer::GetMove(void) { return m_move; }

//=============================================================================
// プレイヤーの当たり判定の半径の取得
//=============================================================================
float CPlayer::GetRadius(void) {
	return COLLISION_RADIUS;
}

//=============================================================================
// 攻撃開始
//=============================================================================
void CPlayer::StartAttack(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//入力デバイスの取得
	CInput* pInput = pManager->GetInputCur();
	if (pInput == nullptr) return;

	//パンチ
	if (pInput->GetPress(CInput::CODE::ATTACK_1)) {
		//モーションの設定
		SetMotion((int)MOTION_TYPE::PUNCH);
	}
	//踏みつけ
	else if (pInput->GetPress(CInput::CODE::ATTACK_2)) {
		//モーションの設定
		SetMotion((int)MOTION_TYPE::STAMP);
	}
}

//=============================================================================
// 移動
//=============================================================================
void CPlayer::Move(CInput* pInput, float fRotCameraY) {
	if (pInput == nullptr) return;

	//上下左右キー入力状態の取得
	const bool bPressUp = pInput->GetPress(CInput::CODE::MOVE_UP);
	const bool bPressDown = pInput->GetPress(CInput::CODE::MOVE_DOWN);
	const bool bPressLeft = pInput->GetPress(CInput::CODE::MOVE_LEFT);
	const bool bPressRight = pInput->GetPress(CInput::CODE::MOVE_RIGHT);

	bool bDiagonalMove = (bPressUp != bPressDown) && (bPressLeft != bPressRight);	//斜め移動
	bool bRotateUp, bRotateDown, bRotateLeft, bRotateRight;	//回転する方向
	bRotateUp = bRotateDown = bRotateLeft = bRotateRight = false;

	D3DXVECTOR3 moveAddSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//追加する移動量
	D3DXVECTOR3 moveMaxSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動量の最大

	//------------------------
	//移動速度の最大を計算
	//------------------------
	float fMaxSpeed;	//移動速度の最大
	//地上
	//if (pInput->GetPress(CInput::CODE::DASH)) {
	//	fMaxSpeed = MAX_MOVE_SPEED_DASH;
	//}
	//else {
		fMaxSpeed = MAX_MOVE_SPEED_WALK;
	//}
	//空中
	if (!m_bLand) {
		fMaxSpeed = MAX_MOVE_SPEED_AIR;
	}
	//移動速度の加算
	fMaxSpeed += m_fAddMoveSpeed;

	//------------------------
	//移動量の決定
	//------------------------
	//前後移動のどちらかのみ押されている場合
	if (bPressUp != bPressDown) {
		if (bPressUp) {
			moveAddSpeed.z = ADD_MOVE_SPEED;
			moveMaxSpeed.z = fMaxSpeed;
			bRotateUp = true;
		}
		else if (bPressDown) {
			moveAddSpeed.z = -ADD_MOVE_SPEED;
			moveMaxSpeed.z = -fMaxSpeed;
			bRotateDown = true;
		}
		//斜め移動
		if (bDiagonalMove) {
			moveAddSpeed.z *= 0.7f;
			moveMaxSpeed.z *= 0.7f;
		}
	}
	//左右移動のどちらかのみ押されている場合
	if (bPressLeft != bPressRight) {
		if (bPressLeft) {
			moveAddSpeed.x = -ADD_MOVE_SPEED;
			moveMaxSpeed.x = -fMaxSpeed;
			bRotateLeft = true;
		}
		else if (bPressRight) {
			moveAddSpeed.x = ADD_MOVE_SPEED;
			moveMaxSpeed.x = fMaxSpeed;
			bRotateRight = true;
		}
		//斜め移動
		if (bDiagonalMove) {
			moveAddSpeed.x *= 0.7f;
			moveMaxSpeed.x *= 0.7f;
		}
	}

	//------------------------
	//カメラの角度に合わせて移動量の最大量を設定
	//------------------------
	D3DXVECTOR3 moveMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//移動の最大量
	moveMax.x += moveMaxSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
	moveMax.x += moveMaxSpeed.z * sinf(fRotCameraY);

	moveMax.z += moveMaxSpeed.x * -sinf(fRotCameraY);
	moveMax.z += moveMaxSpeed.z * cosf(fRotCameraY);

	//------------------------
	//加速
	//------------------------
	//X方向の移動量が最大量未満の場合
	if ((moveMax.x > 0 && m_move.x < moveMax.x) || (moveMax.x < 0 && m_move.x > moveMax.x)) {
		//カメラの角度に合わせて移動量を追加
		m_move.x += moveAddSpeed.x * sinf(fRotCameraY + 0.5f * D3DX_PI);
		m_move.x += moveAddSpeed.z * sinf(fRotCameraY);
		//加速時に移動量が最大を超えた場合
		if (moveMax.x > 0) {
			if (m_move.x > moveMax.x) {
				m_move.x = moveMax.x;
			}
		}
		else if (moveMax.x < 0) {
			if (m_move.x < moveMax.x) {
				m_move.x = moveMax.x;
			}
		}
	}
	//Z方向の移動量が最大量未満の場合
	if ((moveMax.z > 0 && m_move.z < moveMax.z) || (moveMax.z < 0 && m_move.z > moveMax.z)) {
		//カメラの角度に合わせて移動量を追加
		m_move.z += moveAddSpeed.x * -sinf(fRotCameraY);
		m_move.z += moveAddSpeed.z * cosf(fRotCameraY);
		//加速時に移動量が最大を超えた場合
		if (moveMax.z > 0) {
			if (m_move.z > moveMax.z) {
				m_move.z = moveMax.z;
			}
		}
		else if (moveMax.z < 0) {
			if (m_move.z < moveMax.z) {
				m_move.z = moveMax.z;
			}
		}
	}

	//------------------------
	//回転方向の決定
	//------------------------		
	bool bInputMove = false;	//移動キーを押しているかどうか
	bInputMove = bRotateUp || bRotateDown || bRotateLeft || bRotateRight;	//どれかが押されている場合
	//移動キー押下中
	if (bInputMove) {
		//キーを押していた方向を目標の角度にする
		//上
		if (bRotateUp) {
			if (bRotateRight) {
				m_destRot.y = D3DX_PI * -0.75f + fRotCameraY;
			}
			else if (bRotateLeft) {
				m_destRot.y = D3DX_PI * 0.75f + fRotCameraY;
			}
			else {
				m_destRot.y = D3DX_PI + fRotCameraY;
			}
		}
		//下
		else if (bRotateDown) {
			if (bRotateRight) {
				m_destRot.y = D3DX_PI * -0.25f + fRotCameraY;
			}
			else if (bRotateLeft) {
				m_destRot.y = D3DX_PI * 0.25f + fRotCameraY;
			}
			else {
				m_destRot.y = 0.0f + fRotCameraY;
			}
		}
		//左右
		else {
			if (bRotateRight) {
				m_destRot.y = D3DX_PI * -0.5f + fRotCameraY;
			}
			else if (bRotateLeft) {
				m_destRot.y = D3DX_PI * 0.5f + fRotCameraY;
			}
		}

		//パイ超過時
		if (m_destRot.y > D3DX_PI) {
			m_destRot.y = -D3DX_PI * 2 + m_destRot.y;
		}
		else if (m_destRot.y < -D3DX_PI) {
			m_destRot.y = D3DX_PI * 2 + m_destRot.y;
		}

		D3DXVECTOR3 rotPlayer = CObjectMotion::GetRot();//角度の取得
		float fdeltaRot;	//角度の差分
		//現在の角度と目標の角度の差分の計算
		if (m_destRot.y >= 0.0f) {
			if (rotPlayer.y >= 0.0f) {
				fdeltaRot = m_destRot.y - rotPlayer.y;
			}
			else if (rotPlayer.y < 0.0f) {
				if (m_destRot.y - rotPlayer.y >= D3DX_PI) {
					fdeltaRot = -D3DX_PI - rotPlayer.y - D3DX_PI + m_destRot.y;
				}
				else if (m_destRot.y - rotPlayer.y < D3DX_PI) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
			}
		}
		else if (m_destRot.y < 0.0f) {
			if (rotPlayer.y >= 0.0f) {
				if (rotPlayer.y - m_destRot.y >= D3DX_PI) {
					fdeltaRot = D3DX_PI - rotPlayer.y + D3DX_PI + m_destRot.y;
				}
				else if (rotPlayer.y - m_destRot.y < D3DX_PI) {
					fdeltaRot = m_destRot.y - rotPlayer.y;
				}
			}
			else if (rotPlayer.y < 0.0f) {
				fdeltaRot = m_destRot.y - rotPlayer.y;
			}
		}

		//回転の反映
		rotPlayer.y += fdeltaRot * ROTATE_SPEED;

		//パイ超過時
		if (rotPlayer.y > D3DX_PI) {
			rotPlayer.y = -D3DX_PI;
		}
		else if (rotPlayer.y < -D3DX_PI) {
			rotPlayer.y = D3DX_PI;
		}

		//角度の設定
		CObjectMotion::SetRot(rotPlayer);
	}

	//------------------------
	//地上にいる時のモーション切り替え
	//------------------------
	if (m_bLand && GetMotionCategory() != MOTION_CATEGORY::ATTACK) {
		//移動キーを押している時のモーション切り替え
		if (bInputMove) {
			if (GetMotionCategory() != MOTION_CATEGORY::MOVE) {
				SetMotion((int)MOTION_TYPE::WALK);
			}
		}
		//移動キーを押していない場合のモーション切り替え
		if (!bInputMove) {
			//モーションの設定
			if (GetMotionCategory() == MOTION_CATEGORY::MOVE) {
				SetMotion((int)MOTION_TYPE::NEUTRAL);
			}
		}
	}
}

//=============================================================================
// 移動量の減少
//=============================================================================
void CPlayer::DecMove(void) {
	float fDecSpeed = DEC_MOVE_SPEED_GROUND;	//減速量
	if (!m_bLand) fDecSpeed = DEC_MOVE_SPEED_AIR;	//空中
	D3DXVECTOR2 vecMoveDec = D3DXVECTOR2(-m_move.x, -m_move.z);	//移動量ベクトルの逆
	D3DXVec2Normalize(&vecMoveDec, &vecMoveDec);	//正規化
	vecMoveDec *= fDecSpeed;	//移動量の加算

	//減速
	m_move.x += vecMoveDec.x;
	m_move.z += vecMoveDec.y;

	//X方向の移動量の停止
	if (vecMoveDec.x > 0.0f) {
		if (m_move.x > 0.0f) {
			m_move.x = 0.0f;
		}
	}
	else if (vecMoveDec.x < 0.0f) {
		if (m_move.x < 0.0f) {
			m_move.x = 0.0f;
		}
	}
	//Z方向の移動量の停止
	if (vecMoveDec.y > 0.0f) {
		if (m_move.z > 0.0f) {
			m_move.z = 0.0f;
		}
	}
	else if (vecMoveDec.x < 0.0f) {
		if (m_move.z < 0.0f) {
			m_move.z = 0.0f;
		}
	}
}

//=============================================================================
// 当たり判定
//=============================================================================
void CPlayer::Collision(D3DXVECTOR3& pos) {
	//壁との当たり判定
	CWallCylinder::Collision(&pos, m_lastPos, COLLISION_RADIUS);

	//地面との当たり判定
	bool bLand = false;	//接地しているかどうか
	D3DXVECTOR3 posColTerrain = pos;	//接触位置、開始ベクトル、終了ベクトル

	bLand = CTerrain::Collision(posColTerrain);

	//接地時
	if (bLand) {
		m_move.y = -POWER_GRAVITY_GROUND;

		pos = posColTerrain;	//位置の移動
		//着地
		if (!m_bLand) {
			m_bLand = true;
		}
	}
	//落下
	else if (!bLand) {
		//地上から空中
		if (m_bLand) {
			m_bLand = false;
		}
	}
}

//=============================================================================
// モーション終了時
//=============================================================================
void CPlayer::EndMotion(void) {

	//硬直ありのモーション
	if (GetMotionType() == (int)MOTION_TYPE::PUNCH) {
		m_nCntLockAct = 25;
	}
	else if (GetMotionType() == (int)MOTION_TYPE::STAMP) {
		m_nCntLockAct = 35;
	}

	//ニュートラルモーションを設定
	SetMotion((int)MOTION_TYPE::NEUTRAL);
	//移動量を重力のみにする
	m_move = D3DXVECTOR3(0.0f, -POWER_GRAVITY_GROUND, 0.0f);

	m_bValidAttack = false;
}

//=============================================================================
// モーション中の処理
//=============================================================================
void CPlayer::MotionAction(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	bool bBeginAttack = false;	//攻撃開始時かどうか

	CGameScene* pGameScene = nullptr;
	if(pManager != nullptr) pGameScene = pManager->GetGameScene();

	CAppleTree* pAppleTree = nullptr;
	if (pGameScene != nullptr) pAppleTree = pGameScene->GetAppleTree();

	int nNumShockwave = 0;
	int nNumThunder = 0;

	if (pAppleTree != nullptr) {
		nNumShockwave = pAppleTree->CAppleTree::GetNumApple(CGlowApple::APPLE_TYPE::WHITE);
		nNumThunder = pAppleTree->CAppleTree::GetNumApple(CGlowApple::APPLE_TYPE::SILVER);
		nNumThunder *= 2;
	}

	switch ((MOTION_TYPE)GetMotionType())
	{

		//-------------------------
		//移動
		//-------------------------
	case MOTION_TYPE::WALK:
		//足が地面に触れたタイミングだった場合
		if ((GetCurKey() == 0 && GetCurMotionCnt() == 14 && !GetTransMotion()) || (GetCurKey() == 2 && GetCurMotionCnt() == 14)) {
			//歩行時の音
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::FOOTSTEP);

			//生成時にエフェクトを生成
			CParticleEffect::PARTICLE_INFO particleInfo;	//パーティクル情報
			particleInfo.addMove = D3DXVECTOR3(0.0f, -0.5f, 0.0f); particleInfo.colEnd = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); particleInfo.colStart = D3DXCOLOR(0.5f, 1.0f, 0.5f, 1.0f);
			particleInfo.fAddSize = -1.0f; particleInfo.fSizeStart = 30.0f; particleInfo.fSpeedMove = 6.5f; particleInfo.nLife = 30;

			D3DXVECTOR3 posFoot;//動かしている方の足のエフェクトの位置
			int nIdxModel = 0;	//足のインデックス

			if (GetCurKey() == 0) {
				nIdxModel = 6;	//右足
			}
			else if (GetCurKey() == 2) {
				nIdxModel = 8;	//左足
			}
			//足の位置の取得
			GetPosCollision(&posFoot, D3DXVECTOR3(0.0f, -85.0f, 5.0f), nIdxModel);
			//１回分のエフェクトを同時に重ねて表示
			for (int nCnt = 0; nCnt < 6; nCnt++)
			{
				CParticleEffect::Create(particleInfo, posFoot, 2, 1, 0.08f * D3DX_PI, true);
			}

			//衝撃波生成
			//CShockWaveEmitter::Create(m_nNumShockWave, 4, posFoot, 30.0f, 2.0f, 4.0f, 40.0f, -1.0f, 15, D3DX_PI * 0.02f);
		
		}
		break;

		//-------------------------
		//パンチの攻撃
		//-------------------------
		//パンチ
	case MOTION_TYPE::PUNCH:
		//攻撃のタイミングだった場合
		if ((GetCurKey() == 1 || (GetCurKey() == 2 && GetCurMotionCnt() == 0)) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
		}
		else {
			m_bValidAttack = false;
		}
		//攻撃が地面に当たったタイミングだった場合
		if (GetCurKey() == 1 && GetCurMotionCnt() == 5) {
			//衝撃音
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOCK_PUNCH);

			//衝撃波生成
			D3DXVECTOR3 posWave = GetPos() + D3DXVECTOR3(sinf(GetRot().y + D3DX_PI) * 200.0f, -5.0f, cosf(GetRot().y + D3DX_PI) * 200.0f);	//プレイヤーの前方
			CShockWaveEmitter* pSWEmitter = CShockWaveEmitter::Create(nNumShockwave + 1, 4, posWave, 40.0f, 0.0f, 22.0f, 90.0f, -1.0f, 13, D3DX_PI * 0.02f);
			//生成数ごとに足す色の設定
			if (pSWEmitter != nullptr) pSWEmitter->SetColAddCreate(D3DXCOLOR(-0.15f, -0.15f, 0.0f, 0.0f));

			//雷の生成
			if (nNumThunder > 0) CThunderEmitter::CreateStraight(nNumThunder + 1, 5, posWave, posWave - GetPos(), 100.0f);
		}
		break;

		//踏みつけ
	case MOTION_TYPE::STAMP:
		//攻撃のタイミングだった場合
		if ((GetCurKey() == 1 || (GetCurKey() == 2 && GetCurMotionCnt() == 0)) && !GetTransMotion()) {
			//攻撃開始時
			if (!m_bValidAttack) {
				bBeginAttack = true;
			}
			m_bValidAttack = true;
		}
		else {
			m_bValidAttack = false;
		}
		//攻撃が地面に当たったタイミングだった場合
		if (GetCurKey() == 1 && GetCurMotionCnt() == 3) {
			//衝撃音
			if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOCK_STAMP);

			//衝撃波生成
			D3DXVECTOR3 posWave = GetPos() + D3DXVECTOR3(0.0f, -5.0f, 0.0f);
			CShockWaveEmitter* pSWEmitter = CShockWaveEmitter::Create(nNumShockwave + 2, 4, posWave, 50.0f, 0.0f, 30.0f, 110.0f, -1.0f, 15, D3DX_PI * 0.02f);
			//生成数ごとに足す色の設定
			if (pSWEmitter != nullptr) pSWEmitter->SetColAddCreate(D3DXCOLOR(0.0f, -0.05f, -0.15f, 0.0f));

			//雷の生成
			if(nNumThunder > 0) CThunderEmitter::CreateRound(nNumThunder + 1, 3, posWave, GetRot().y + D3DX_PI, 400.0f);
		}
		break;

	default:
		m_bValidAttack = false;
		break;
	}

	//攻撃開始時
	if (bBeginAttack) {
		AttackBegin();
	}
}

//=============================================================================
// 攻撃の当たり判定
//=============================================================================
void CPlayer::AttackCollision(int nIdxModel, const int nNumCol, float fRadiusCol, D3DXVECTOR3 posOffsetStart, D3DXVECTOR3 posOffsetAdd, int nDamage) {
	D3DXVECTOR3* pPosColArray = new D3DXVECTOR3[nNumCol];	//当たり判定の位置を生成
	int nNumKillEnemy = 0;	//敵を倒した数

	D3DXVECTOR3 posOffset = posOffsetStart;	//モデルからのオフセット

	for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
	{
		//当たり判定の位置の取得
		GetPosCollision(&pPosColArray[nCntCol], posOffset, nIdxModel);
		//オフセットを加算
		posOffset += posOffsetAdd;
	}

	for (int nCntCol = 0; nCntCol < nNumCol; nCntCol++)
	{
		//攻撃
		CObjectMotion::Attack(OBJTYPE_ENEMY | OBJTYPE_APPLETREE | OBJTYPE_SCAPEGOAT, pPosColArray[nCntCol], fRadiusCol, nDamage, DAMAGE_TYPE::PLAYER_PUNCH, &nNumKillEnemy);	//敵
	}

	//攻撃の先端にパーティクルの生成
	CParticle::Create(pPosColArray[nNumCol - 1], 20, 55.0f, -1.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.0f, 0.5f, 1.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));

	delete[] pPosColArray;	//動的確保した当たり判定の位置のメモリを解放

	//敵を倒した数の加算
	m_nNumKillEnemy += nNumKillEnemy;
}

//=============================================================================
// 攻撃開始時
//=============================================================================
void CPlayer::AttackBegin(void) {
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	//攻撃済みリストの初期化
	InitObjAttacked();	

	switch ((MOTION_TYPE)GetMotionType())
	{
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		//拳を振る音の再生
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SWISH_PUNCH);
		break;
	}
}

//=============================================================================
// モーションの区分の取得
//=============================================================================
CPlayer::MOTION_CATEGORY CPlayer::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//移動
	case MOTION_TYPE::WALK:
		return MOTION_CATEGORY::MOVE;
		break;

		//攻撃
	case MOTION_TYPE::PUNCH:
	case MOTION_TYPE::STAMP:
		return MOTION_CATEGORY::ATTACK;
			break;

		//その他
	default:
		return MOTION_CATEGORY::NONE;
		break;
	}
}