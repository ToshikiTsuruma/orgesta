//=============================================================================
//
// 敵スポナー処理 [enemySpawner.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "enemySpawner.h"
#include "manager.h"
#include "sound.h"
#include "gameScene.h"
#include "timer.h"
#include "enemy_normal.h"
#include "enemy_human.h"
#include "enemy_sorcerer.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CEnemySpawner::CEnemySpawner()
{
	m_nLevel = 0;
	m_nCntSpawn = 0;
	m_nSpanSpawn = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CEnemySpawner::~CEnemySpawner()
{

}

//=============================================================================
// 敵スポナーの生成処理
//=============================================================================
CEnemySpawner* CEnemySpawner::Create(int nSpan, float fRadius, int nDistMin, int nDistMax) {
	CEnemySpawner* pEnemySpawner;
	pEnemySpawner = new CEnemySpawner();
	if (pEnemySpawner == nullptr) return nullptr;

	pEnemySpawner->m_nSpanSpawn = nSpan;
	pEnemySpawner->m_fRadiusSpawn = fRadius;
	pEnemySpawner->m_nDistEnemyMin = nDistMin;
	pEnemySpawner->m_nDistEnemyMax = nDistMax;
	pEnemySpawner->Init();

	return pEnemySpawner;
}

//=============================================================================
// 敵スポナーの初期化処理
//=============================================================================
HRESULT CEnemySpawner::Init(void) {
	m_nCntSpawn = 0;

	return S_OK;
}

//=============================================================================
// 敵スポナーの終了処理
//=============================================================================
void CEnemySpawner::Uninit(void) {
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 敵スポナーの更新処理
//=============================================================================
void CEnemySpawner::Update(void) {
	//生成タイミング
	if (m_nCntSpawn > m_nSpanSpawn) {
		m_nCntSpawn = 0;

		//敵をスポーンさせる
		SetEnemyCircle(D3DXVECTOR3(0.0f, 1500.0f, 0.0f), 1, m_fRadiusSpawn, m_nDistEnemyMin, m_nDistEnemyMax);

		//マネージャーの取得
		CManager* pManager = CManager::GetManager();
		//サウンドの取得
		CSound* pSound = nullptr;
		if (pManager != nullptr) pSound = pManager->GetSound();
		//スポーン音の再生
		if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::SPAWN_ENEMY);
	}
	else {
		m_nCntSpawn++;
	}

	//タイマーによるレベルの設定
	SetTimeLevel();
}

//=============================================================================
// 時間経過によるレベルの設定
//=============================================================================
void CEnemySpawner::SetTimeLevel(void) {
	CManager* pManager = CManager::GetManager();	//マネージャーの取得
	if (pManager == nullptr) return;
	CGameScene* pGameScene = pManager->GetGameScene();;	//ゲームシーンのポインタ
	if (pGameScene == nullptr) return;
	CTimer* pTimer = pGameScene->GetTimer();;	//タイマーのポインタ
	if (pTimer == nullptr) return;

	//時間が切り替わった瞬間のみ
	if (pTimer->GetCountTimer() != 0) return;

	switch (pTimer->GetScore())
	{
	case 260:
		//スポーン間隔の減少
		AddSpan(-60);
		//レベルの設定
		SetLevel(1);
		break;

	case 220:
		//スポーン間隔の減少
		AddSpan(-120);
		break;
		//レベルの設定
		SetLevel(2);

	case 180:
		//スポーン間隔の減少
		AddSpan(-140);
		//レベルの設定
		SetLevel(3);
		//ゴールドラッシュ有効
		CEnemy::SetGoldRush(true);
		break;

	case 100:
		//スポーン間隔の減少
		AddSpan(-160);
		//ゴールドラッシュ無効
		CEnemy::SetGoldRush(false);
		break;

	case 60:
		//スポーン間隔の減少
		AddSpan(-180);
		//レベルの設定
		SetLevel(0);	//一番移動速度が早い敵のみにする
		break;

	case 40:
		//レベルの設定
		SetLevel(4);	//一番強い敵は多く出る
		//スポーンの半径を縮める
		SetSpawnRadius(1000.0f);
		break;

	case 10:
		//スポーン間隔の減少
		m_nSpanSpawn = 60;
		//レベルの設定
		SetLevel(5);	//混ぜる
		break;
	}
}

//=============================================================================
// 同心円状にランダムに角度ずらして敵を配置する
//=============================================================================
//引数：中心の位置　円の数　円ごとの距離（半径）　（同じ円の一つ前の敵との）最短距離、最長距離
void CEnemySpawner::SetEnemyCircle(D3DXVECTOR3 posCenter, int nNumCircle, float fDistCircle, int nMinDist, int nMaxDist) {
	int nCntCircle = 0;	//作成した木の円の数
	float fRotCreate = 0.0f;	//配置する角度
	float fRandStartRot = (rand() % 314) * 0.01f;	//配置の開始角度の位置をランダム 前方が過疎気味になるため

	while (nCntCircle < nNumCircle)
	{
		int nRandDist = rand() % (nMaxDist + 1 - nMinDist) + nMinDist;	//一つ前の木との距離
		fRotCreate += nRandDist / ((nCntCircle + 1) * fDistCircle * D3DX_PI) * D3DX_PI;	//ランダムな距離 / 円周の長さの半分 * pi でラジアンを求める

		//一周配置した場合
		if (fRotCreate >= D3DX_PI * 2.0f) {
			nCntCircle++;	//カウントの加算
			fRotCreate = 0.0f;	//配置する角度のリセット
			fRandStartRot = (rand() % 314) * 0.01f;	//配置する角度のランダム値を再設定
		}
		else {
			//敵を生成する位置の設定
			D3DXVECTOR3 posCreate = posCenter;	//配置する位置
			posCreate.x += sinf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);
			posCreate.z += cosf(fRotCreate + fRandStartRot) * fDistCircle * (nCntCircle + 1);

			int aRateType[(int)ENEMYTYPE::ENUM_MAX] = {};	//種類ごとの生成確率
			//生成確率を取得
			GetSpawnRate(m_nLevel, aRateType);
			//それぞれのタイプ以前のタイプの割合の合計を足す
			for (int nCnt = 1; nCnt < (int)ENEMYTYPE::ENUM_MAX; nCnt++)
			{
				aRateType[nCnt] += aRateType[nCnt - 1];
			}

			//敵の種類をランダムで決める
			int nRandType = rand() % 100;
			//敵の生成
			if (nRandType < aRateType[(int)ENEMYTYPE::NORMAL]) {
				//通常敵
				CEnemyNormal::Create(posCreate);
			}
			else if (nRandType < aRateType[(int)ENEMYTYPE::HUMAN]) {
				//人形敵
				CEnemyHuman::Create(posCreate);
			}
			else if (nRandType < aRateType[(int)ENEMYTYPE::SORCERER]) {
				//魔術師敵
				CEnemySorcerer::Create(posCreate);
			}
		}
	}
}

//=============================================================================
// レベルごとの生成確率の取得
//=============================================================================
void CEnemySpawner::GetSpawnRate(int nLevel, int* pRateArray) {
	if (pRateArray == nullptr) return;
		 
	switch (nLevel)
	{
	case 0:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 100;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 0;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;

	case 1:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 60;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 40;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;

	case 2:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 65;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 5;
		break;

	case 3:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 10;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 80;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 10;
		break;

	case 4:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 10;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 60;
		break;

	case 5:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 30;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 30;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 40;
		break;

	default:
		pRateArray[(int)ENEMYTYPE::NORMAL] = 100;
		pRateArray[(int)ENEMYTYPE::HUMAN] = 0;
		pRateArray[(int)ENEMYTYPE::SORCERER] = 0;
		break;
	}
}