//=============================================================================
//
// �������o���� [thunderEmitter.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "thunderEmitter.h"
#include "thunder.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CThunderEmitter::CThunderEmitter() : m_nNumEmitDest(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CThunderEmitter::CThunderEmitter(int nNumEmitDest) : m_nNumEmitDest(nNumEmitDest)
{
	//�����̐����ʒu�𓮓I�m��
	m_pPosEmitArray = new D3DXVECTOR3[nNumEmitDest];
	m_nNumEmit = 0;
	m_nCntEmit = 0;
	m_nSpanEmit = 0;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CThunderEmitter::~CThunderEmitter()
{
	//�����̐����ʒu�̔j��
	if (m_pPosEmitArray != nullptr) {
		delete[] m_pPosEmitArray;
		m_pPosEmitArray = nullptr;
	}
}

//=============================================================================
// ���i�̗�����������
//=============================================================================
CThunderEmitter* CThunderEmitter::CreateStraight(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, D3DXVECTOR3 vecStraight, float fDist) {
	//�������O�̏ꍇ�I��
	if (nNumEmitDest <= 0) return nullptr;

	CThunderEmitter* pThunderEmitter;
	pThunderEmitter = new CThunderEmitter(nNumEmitDest);
	if (pThunderEmitter == nullptr) return nullptr;

	pThunderEmitter->m_nSpanEmit = nSpanEmit;
	pThunderEmitter->Init();

	//�����̐����ʒu�̐ݒ�
	if (pThunderEmitter->m_pPosEmitArray != nullptr) {
		D3DXVECTOR3 vecNor;	//���������̖@���x�N�g��
		D3DXVec3Normalize(&vecNor, &vecStraight);	//���K��

		for (int nCnt = 0; nCnt < nNumEmitDest; nCnt++)
		{
			(pThunderEmitter->m_pPosEmitArray)[nCnt] = pos + vecNor * fDist * (float)nCnt;
		}
	}

	return pThunderEmitter;
}

//=============================================================================
// �~�`�̗�����������
//=============================================================================
CThunderEmitter* CThunderEmitter::CreateRound(int nNumEmitDest, int nSpanEmit, D3DXVECTOR3 pos, float rotY, float fRadius) {
	//�������O�̏ꍇ�I��
	if (nNumEmitDest <= 0) return nullptr;

	CThunderEmitter* pThunderEmitter;
	pThunderEmitter = new CThunderEmitter(nNumEmitDest);
	if (pThunderEmitter == nullptr) return nullptr;

	pThunderEmitter->m_nSpanEmit = nSpanEmit;
	pThunderEmitter->Init();

	//�����̐����ʒu�̐ݒ�
	if (pThunderEmitter->m_pPosEmitArray != nullptr) {
		for (int nCnt = 0; nCnt < nNumEmitDest; nCnt++)
		{
			(pThunderEmitter->m_pPosEmitArray)[nCnt] = pos + D3DXVECTOR3(sinf(rotY + D3DX_PI * 2 * (nCnt / (float)nNumEmitDest)) * fRadius, 0.0f, cosf(rotY + D3DX_PI * 2 * (nCnt / (float)nNumEmitDest)) * fRadius);
		}
	}

	return pThunderEmitter;
}

//=============================================================================
// �������o�̏���������
//=============================================================================
HRESULT CThunderEmitter::Init(void) {
	m_nNumEmit = 0;
	m_nCntEmit = 0;

	return S_OK;
}

//=============================================================================
// �������o�̏I������
//=============================================================================
void CThunderEmitter::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �������o�̍X�V����
//=============================================================================
void CThunderEmitter::Update(void) {
	//�����^�C�~���O
	if (m_nCntEmit > m_nSpanEmit) {
		m_nCntEmit = 0;

		//�����̐���
		if(m_pPosEmitArray != nullptr) CThunder::Create(m_pPosEmitArray[m_nNumEmit]);

		//�����J�E���g�̉��Z
		m_nNumEmit++;

		//�����I��
		if (m_nNumEmit >= m_nNumEmitDest) {
			Uninit();
			return;
		}
	}
	else {
		m_nCntEmit++;
	}
}