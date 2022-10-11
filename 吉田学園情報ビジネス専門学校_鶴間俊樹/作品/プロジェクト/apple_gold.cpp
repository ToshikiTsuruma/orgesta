//=============================================================================
//
// 金林檎処理 [apple_gold.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "apple_gold.h"
#include "appleTree.h"
#include "bullet.h"
#include "manager.h"
#include "sound.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define SPAN_BULLET (30)	//弾の生成間隔
#define BULLET_MOVE_SPEED (30.0f)	//弾の移動速度
#define	MAX_DISTANCE_BULLET (1500.0f)	//弾が発射可能な最大の距離
#define	BULLET_DAMAGE (5)	//弾のダメージ

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CAppleGold::CAppleGold()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CAppleGold::CAppleGold(CAppleTree* pTree) : CGlowApple(pTree)
{

}

//=============================================================================
// デストラクタ
//=============================================================================
CAppleGold::~CAppleGold()
{

}

//=============================================================================
// 金林檎の生成処理
//=============================================================================
CAppleGold* CAppleGold::Create(D3DXVECTOR3 pos, CAppleTree* pTree) {
	//ポインタがリンゴの木ではない場合null
	pTree = dynamic_cast<CAppleTree*>(pTree);

	CAppleGold* pAppleGold = new CAppleGold(pTree);
	if (pAppleGold == nullptr) return nullptr;

	pAppleGold->SetPos(pos);
	pAppleGold->Init();

	return pAppleGold;
}

//=============================================================================
// 金林檎の初期化処理
//=============================================================================
HRESULT CAppleGold::Init(void) {
	CModel* pModelApple = GetPtrModel();
	//モデルの色を変更
	if (pModelApple != nullptr) {
		pModelApple->SetMaterialDiffuse(GetAppleColor(APPLE_TYPE::GOLD), 0);
	}

	//発光表現のためにパーティクルを生成
	CreateGlowParticle(APPLE_TYPE::GOLD);

	CGlowApple::Init();
	return S_OK;
}

//=============================================================================
// 金林檎の終了処理
//=============================================================================
void CAppleGold::Uninit(void) {
	CGlowApple::Uninit();
}

//=============================================================================
// 金林檎の更新処理
//=============================================================================
void CAppleGold::Update(void) {
	if (m_nCntBullet == SPAN_BULLET) {
		m_nCntBullet = 0;
		//弾の発射
		ShootBullet();
	}
	else {
		m_nCntBullet++;
	}

	CGlowApple::Update();
}

//=============================================================================
// 金林檎の描画処理
//=============================================================================
void CAppleGold::Draw(void) {
	CGlowApple::Draw();
}

//=============================================================================
// 弾の発射
//=============================================================================
void CAppleGold::ShootBullet(void) {
	bool bSuccessGetPos = false;		//位置の取得が成功したかどうか

	D3DXVECTOR3 posApple = GetPos();	//林檎の位置
	posApple.y -= 15.0f;	//位置の調整
	D3DXVECTOR3 posEnemy;	//敵の位置
	float fDistEnemy;		//敵への距離

	//一番近い敵の位置を取得
	bSuccessGetPos = GetNearObject(posApple, OBJTYPE_ENEMY, OBJFLAG_ENABLECOLLISION, &posEnemy, &fDistEnemy, nullptr) != nullptr;

	//位置の取得失敗した場合生成しない
	if (!bSuccessGetPos || fDistEnemy > MAX_DISTANCE_BULLET) return;

	D3DXVECTOR3 vecEnemy = posEnemy - posApple;	//敵へのベクトル
	vecEnemy.y += 50.0f;	//敵の高さに合わせる

	//正規化
	D3DXVec3Normalize(&vecEnemy, &vecEnemy);

	//弾の移動速度
	D3DXVECTOR3 moveBullet = vecEnemy * BULLET_MOVE_SPEED;	//正規化した敵への方向に移動速度を乗算

	//弾の生成
	CBullet* pBullet = CBullet::Create(CModel::MODELTYPE::OBJ_APPLE_BULLET, posApple, moveBullet, 120, OBJTYPE_ENEMY, 15.0f, BULLET_DAMAGE);
	//弾の初期設定
	if (pBullet != nullptr) {
		pBullet->SetParticleInfo(10, 25.0f, -2.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	//軌道のパーティクルの設定
		pBullet->SetNumParticle(3);	//補完用のパーティクルの数を設定
	}
	//マネージャーの取得
	CManager* pManager = CManager::GetManager();
	//サウンドの取得
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//弾発射音の再生
	if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::SHOOT_BULLET);
}