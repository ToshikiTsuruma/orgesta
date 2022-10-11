//=============================================================================
//
// パーティクルエフェクト処理 [particleEffect.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "particleEffect.h"

//=============================================================================
// マクロ定義
//=============================================================================

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CParticleEffect::CParticleEffect()
{

}

//=============================================================================
// オーバーロードされたコンストラクタ
//=============================================================================
CParticleEffect::CParticleEffect(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit, bool bRandomAngle)
{
	m_particleInfo = particleInfo;
	m_pos = pos;
	m_bLoop = false;
	if (nLife <= 0) m_bLoop = true;	//ライフが０以下ならループ
	m_nLife = nLife;
	m_nCntCreate = 0;
	m_nSpanCreate = nSpanCreate;
	m_fAngleEmit = fAngleEmit;
	m_bRandomAngle = bRandomAngle;
}

//=============================================================================
// デストラクタ
//=============================================================================
CParticleEffect::~CParticleEffect()
{

}

//=============================================================================
// パーティクルエフェクトの生成処理
//=============================================================================
CParticleEffect* CParticleEffect::Create(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit, bool bRandomAngle) {
	CParticleEffect* pParticleEffect;
	pParticleEffect = new CParticleEffect(particleInfo, pos, nLife, nSpanCreate, fAngleEmit, bRandomAngle);
	if (pParticleEffect == nullptr) return nullptr;

	pParticleEffect->Init();

	return pParticleEffect;
}

//=============================================================================
// パーティクルエフェクトの初期化処理
//=============================================================================
HRESULT CParticleEffect::Init(void) {
	m_nCntCreate = m_nSpanCreate;	//生成時からパーティクルを生成できるようにする
	return S_OK;
}

//=============================================================================
// パーティクルエフェクトの終了処理
//=============================================================================
void CParticleEffect::Uninit(void) {
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// パーティクルエフェクトの更新処理
//=============================================================================
void CParticleEffect::Update(void) {
	//ループしない場合
	if (!m_bLoop) {
		//ライフが０以下
		if (m_nLife <= 0) {
			//エフェクト破棄
			Uninit();
			return;
		}
		else {
			//ライフの減少
			m_nLife--;
		}
	}

	if (m_nCntCreate >= m_nSpanCreate) {
		//生成カウントのリセット
		m_nCntCreate = 0;

		D3DXVECTOR3 moveParticle;	//パーティクルの移動量
		//ランダムでXZ平面の移動の角度を決める
		float fRandRotXZ = (rand() % 624) / 100.0f;	//ランダムの角度

		float fAngleEmit = m_fAngleEmit;	//放出角度
		//範囲内のランダムで放射する角度を決める
		if (m_bRandomAngle) fAngleEmit = (rand() % (int)(m_fAngleEmit * 100.0f)) / 100.0f;	//ランダムの角度

		//パーティクルの移動量のを決める
		moveParticle = D3DXVECTOR3(sinf(fRandRotXZ) * m_particleInfo.fSpeedMove * sinf(fAngleEmit),
			cosf(fAngleEmit) * m_particleInfo.fSpeedMove,
			cosf(fRandRotXZ) * m_particleInfo.fSpeedMove * sinf(fAngleEmit));

		//パーティクルの生成
		CParticle::Create(m_pos, m_particleInfo.nLife, m_particleInfo.fSizeStart, m_particleInfo.fAddSize, moveParticle, m_particleInfo.addMove, m_particleInfo.colStart, m_particleInfo.colEnd);
	}

	//生成のカウントの加算
	m_nCntCreate++;
}

//=============================================================================
// 円状にパーティクルを放射する
//=============================================================================
void CParticleEffect::EmitCircle(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreate, float fAngleEmit) {
	//パーティクル生成分ループ
	for (int nCnt = 0; nCnt < nNumCreate; nCnt++)
	{
		D3DXVECTOR3 moveParticle;	//パーティクルの移動量
		//XZ平面の放射の角度を決める
		float fRotEmit = (nCnt / (float)nNumCreate) * D3DX_PI * 2;
		//パーティクルの移動量のを決める
		moveParticle = D3DXVECTOR3(sinf(fRotEmit) * particleInfo.fSpeedMove * sinf(fAngleEmit),
		cosf(fAngleEmit) * particleInfo.fSpeedMove,
		cosf(fRotEmit) * particleInfo.fSpeedMove * sinf(fAngleEmit));
		//パーティクルの生成
		CParticle::Create(pos, particleInfo.nLife, particleInfo.fSizeStart, particleInfo.fAddSize, moveParticle, particleInfo.addMove, particleInfo.colStart, particleInfo.colEnd);
	}
}

//=============================================================================
// 球状にパーティクルを放射する
//=============================================================================
void CParticleEffect::EmitSphere(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreateXZ, int nNumCreateY, float fAddSpeed) {
	//パーティクル生成分ループ
	for (int nCntXZ = 0; nCntXZ < nNumCreateXZ; nCntXZ++)
	{
		//XZ平面の放射の角度を決める
		float fRotEmitXZ = (nCntXZ / (float)nNumCreateXZ) * D3DX_PI * 2;

		for (int nCntY = 0; nCntY < nNumCreateY; nCntY++)
		{
			//Y方向の放射の角度を決める
			float fRotEmitY = (nCntY / (float)(nNumCreateY - 1)) * D3DX_PI;

			D3DXVECTOR3 moveParticle;	//パーティクルの移動量
			//パーティクルの移動量のを決める
			moveParticle = D3DXVECTOR3(sinf(fRotEmitXZ) * sinf(fRotEmitY), cosf(fRotEmitY), cosf(fRotEmitXZ) * sinf(fRotEmitY)) * particleInfo.fSpeedMove;

			D3DXVECTOR3 addMove;	//加速量
			D3DXVec3Normalize(&addMove, &moveParticle);
			addMove *= fAddSpeed;

			//パーティクルの生成
			CParticle::Create(pos, particleInfo.nLife, particleInfo.fSizeStart, particleInfo.fAddSize, moveParticle, particleInfo.addMove + addMove, particleInfo.colStart, particleInfo.colEnd);
		}
	}
}