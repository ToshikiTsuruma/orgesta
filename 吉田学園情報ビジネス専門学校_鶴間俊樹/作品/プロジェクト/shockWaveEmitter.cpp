//=============================================================================
//
// 衝撃波放出処理 [shockWaveEmitter.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "shockWaveEmitter.h"

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CShockWaveEmitter::CShockWaveEmitter()
{
	m_nNumCreate = 0;
	m_nSpanCreate = 0;
	m_colAddCreate = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);

	m_posCreate = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fRadiusTop = 0.0f;
	m_fRadiusBottom = 0.0f;
	m_fAddRadius = 0.0f;
	m_fHeight = 0.0f;
	m_fAddHeight = 0.0f;
	m_nLife = 0;
	m_fRotSpeed = 0.0f;
}

//=============================================================================
// デストラクタ
//=============================================================================
CShockWaveEmitter::~CShockWaveEmitter()
{

}

//=============================================================================
// 衝撃波放出クラスの生成処理
//=============================================================================
CShockWaveEmitter* CShockWaveEmitter::Create(int nNumCreate, int nSpanCreate, D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed) {
	//衝撃波生成数0以下の場合生成しない
	if (nNumCreate <= 0) return nullptr;
	
	CShockWaveEmitter* pShockWaveEmitter;
	pShockWaveEmitter = new CShockWaveEmitter();
	if (pShockWaveEmitter == nullptr) return nullptr;

	pShockWaveEmitter->m_nNumCreate = nNumCreate;
	pShockWaveEmitter->m_nSpanCreate = nSpanCreate;
	pShockWaveEmitter->m_posCreate = pos;
	pShockWaveEmitter->m_fRadiusTop = fRadiusTop;
	pShockWaveEmitter->m_fRadiusBottom = fRadiusBottom;
	pShockWaveEmitter->m_fAddRadius = fAddRadius;
	pShockWaveEmitter->m_fHeight = fHeight;
	pShockWaveEmitter->m_fAddHeight = fAddHeight;
	pShockWaveEmitter->m_nLife = nLife;
	pShockWaveEmitter->m_fRotSpeed = fRotSpeed;

	pShockWaveEmitter->Init();

	return pShockWaveEmitter;
}

//=============================================================================
// 衝撃波放出クラスの初期化処理
//=============================================================================
HRESULT CShockWaveEmitter::Init(void) {
	m_nNumCreateEnd = 0;

	//最初に衝撃波を生成
	CreateShockWave();

	return S_OK;
}

//=============================================================================
// 衝撃波放出クラスの終了処理
//=============================================================================
void CShockWaveEmitter::Uninit(void) {
	//オブジェクトの破棄
	Release();
}

//=============================================================================
// 衝撃波放出クラスの更新処理
//=============================================================================
void CShockWaveEmitter::Update(void) {
	m_nCntCreate++;
	if (m_nCntCreate >= m_nSpanCreate) {
		CreateShockWave();
		m_nCntCreate = 0;
	}
}

//=============================================================================
// 衝撃波を生成
//=============================================================================
void CShockWaveEmitter::CreateShockWave(void) {
	float fRotSpeed = m_fRotSpeed;	//回転速度
	//交互に回転方向を変える
	if (m_nNumCreateEnd % 2 == 1) fRotSpeed *= -1.0f;

	int nLife = m_nLife - m_nSpanCreate * m_nNumCreateEnd;	//消滅時間を揃える場合の

	CShockwave* pShockWave = CShockwave::Create(m_posCreate, m_fRadiusTop, m_fRadiusBottom, m_fAddRadius, m_fHeight, m_fAddHeight, m_nLife, fRotSpeed);
	if (pShockWave != nullptr) pShockWave->AddColor(m_colAddCreate * (float)m_nNumCreateEnd);

	//生成カウントを増やす
	m_nNumCreateEnd++;
	//生成が完了したら終了
	if (m_nNumCreateEnd >= m_nNumCreate) Uninit();
}