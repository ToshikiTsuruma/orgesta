//=============================================================================
//
// �Ռ��g���o���� [shockWaveEmitter.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "shockWaveEmitter.h"

//=============================================================================
// �f�t�H���g�R���X�g���N�^
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
// �f�X�g���N�^
//=============================================================================
CShockWaveEmitter::~CShockWaveEmitter()
{

}

//=============================================================================
// �Ռ��g���o�N���X�̐�������
//=============================================================================
CShockWaveEmitter* CShockWaveEmitter::Create(int nNumCreate, int nSpanCreate, D3DXVECTOR3 pos, float fRadiusTop, float fRadiusBottom, float fAddRadius, float fHeight, float fAddHeight, int nLife, float fRotSpeed) {
	//�Ռ��g������0�ȉ��̏ꍇ�������Ȃ�
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
// �Ռ��g���o�N���X�̏���������
//=============================================================================
HRESULT CShockWaveEmitter::Init(void) {
	m_nNumCreateEnd = 0;

	//�ŏ��ɏՌ��g�𐶐�
	CreateShockWave();

	return S_OK;
}

//=============================================================================
// �Ռ��g���o�N���X�̏I������
//=============================================================================
void CShockWaveEmitter::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �Ռ��g���o�N���X�̍X�V����
//=============================================================================
void CShockWaveEmitter::Update(void) {
	m_nCntCreate++;
	if (m_nCntCreate >= m_nSpanCreate) {
		CreateShockWave();
		m_nCntCreate = 0;
	}
}

//=============================================================================
// �Ռ��g�𐶐�
//=============================================================================
void CShockWaveEmitter::CreateShockWave(void) {
	float fRotSpeed = m_fRotSpeed;	//��]���x
	//���݂ɉ�]������ς���
	if (m_nNumCreateEnd % 2 == 1) fRotSpeed *= -1.0f;

	int nLife = m_nLife - m_nSpanCreate * m_nNumCreateEnd;	//���Ŏ��Ԃ𑵂���ꍇ��

	CShockwave* pShockWave = CShockwave::Create(m_posCreate, m_fRadiusTop, m_fRadiusBottom, m_fAddRadius, m_fHeight, m_fAddHeight, m_nLife, fRotSpeed);
	if (pShockWave != nullptr) pShockWave->AddColor(m_colAddCreate * (float)m_nNumCreateEnd);

	//�����J�E���g�𑝂₷
	m_nNumCreateEnd++;
	//����������������I��
	if (m_nNumCreateEnd >= m_nNumCreate) Uninit();
}