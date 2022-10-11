//=============================================================================
//
// 通常敵処理 [enemy_normal.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemy_normal.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_enemyNormal.txt"
#define MOVE_SPEED (1.2f)			//移動速度
#define ROTATE_SPEED (0.05f * D3DX_PI)		//回転速度
#define NUM_COLLISION (1)			//当たり判定の数
#define COLLISION_RADIUS (25.0f)	//当たり判定の半径
#define MAX_LIFE (20)				//体力の最大値
#define DISTANCE_ATTACK (40.0f)		//攻撃する距離
#define ATTACK_DAMAGE (20)	//攻撃力
#define LIFE_GAUGE_HEIGHT (80.0f)	//敵の位置からの体力ゲージの高さ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
int CEnemyNormal::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CEnemyNormal::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CEnemyNormal::m_aMotionInfo[(int)CEnemyNormal::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemyNormal::CEnemyNormal() : CEnemy(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, MOVE_SPEED, ROTATE_SPEED, MAX_LIFE, DISTANCE_ATTACK, LIFE_GAUGE_HEIGHT)
{
	//倒したスコアの設定
	SetKillScore(100);
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemyNormal::~CEnemyNormal()
{

}

//=============================================================================
// 通常敵の生成処理
//=============================================================================
CEnemyNormal* CEnemyNormal::Create(D3DXVECTOR3 pos) {
	CEnemyNormal* pEnemyNormal;
	pEnemyNormal = new CEnemyNormal();
	if (pEnemyNormal != nullptr) {
		pEnemyNormal->SetPos(pos);
		pEnemyNormal->Init();
	}

	return pEnemyNormal;
}

//=============================================================================
// 通常敵のモーション情報をロード
//=============================================================================
void CEnemyNormal::LoadMotionInfo(void) {
	//ロードする前に動的確保したメモリをdeleteする
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// 通常敵のモーション情報をアンロード
//=============================================================================
void CEnemyNormal::UnloadMotionInfo(void) {
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
// 通常敵の初期化処理
//=============================================================================
HRESULT CEnemyNormal::Init(void) {
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//モーションの初期設定

	//敵の初期化処理
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
// 通常敵の終了処理
//=============================================================================
void CEnemyNormal::Uninit(void) {
	//終了処理
	CEnemy::Uninit();
}

//=============================================================================
// 通常敵の更新処理
//=============================================================================
void CEnemyNormal::Update(void) {
	//敵の更新
	CEnemy::Update();

}

//=============================================================================
// 通常敵の描画処理
//=============================================================================
void CEnemyNormal::Draw(void) {
	CEnemy::Draw();
}

//=============================================================================
// 通常敵の当たり判定の情報の取得
//=============================================================================
void CEnemyNormal::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
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
		posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//オフセットを加算
	}
}

//=============================================================================
// モーションの区分の取得
//=============================================================================
CEnemy::MOTION_CATEGORY CEnemyNormal::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//ニュートラル
	case MOTION_TYPE::NEUTRAL:
	default:
		return MOTION_CATEGORY::NEUTRAL;

		//移動
	case MOTION_TYPE::MOVE:
		return MOTION_CATEGORY::MOVE;

		//攻撃
	case MOTION_TYPE::ATTACK:
		return MOTION_CATEGORY::ATTACK;

		//死亡
	case MOTION_TYPE::DEAD:
		return MOTION_CATEGORY::DEAD;
	}
}

//=============================================================================
// 移動時のモーションを設定
//=============================================================================
void CEnemyNormal::SetMoveMotion(void) {
	SetMotion((int)MOTION_TYPE::MOVE);
}

//=============================================================================
// 死亡時のモーションを設定
//=============================================================================
void CEnemyNormal::SetDeadMotion(void) {
	SetMotion((int)MOTION_TYPE::DEAD);
}

//=============================================================================
// 攻撃開始時
//=============================================================================
void CEnemyNormal::AttackStart(void) {
	//攻撃モーションの設定
	SetMotion((int)MOTION_TYPE::ATTACK);
}

//=============================================================================
// 攻撃停止
//=============================================================================
void CEnemyNormal::AttackStop(void) {

}

//=============================================================================
// モーション終了時
//=============================================================================
void CEnemyNormal::MotionEnd(void) {
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
void CEnemyNormal::MotionAct(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//攻撃
	case MOTION_TYPE::ATTACK:
		//攻撃のタイミング
		if ((GetCurKey() == 1 || GetCurKey() == 2) && !GetTransMotion()) {
			const int nNumCollision = 1;	//攻撃の当たり判定の数
			D3DXVECTOR3 aPosCol[nNumCollision] = {};	//当たり判定の位置
			int nIdxModel = 2;	//右手
			D3DXVECTOR3 posOffset = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);	//モデルからのオフセット

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//当たり判定の位置の取得
				GetPosCollision(&aPosCol[nCntCol], posOffset, nIdxModel);
				//オフセットを加算
				posOffset += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
			}

			//当たり判定の数分攻撃する
			for (int nCntAttack = 0; nCntAttack < nNumCollision; nCntAttack++)
			{
				//攻撃
				CObjectMotion::Attack(ENEMY_ATTACK_TARGET, aPosCol[nCntAttack], 10.0f, ATTACK_DAMAGE, DAMAGE_TYPE::ENEMY_PUNCH, nullptr);
			}
		}
		break;	//_攻撃
	}
}