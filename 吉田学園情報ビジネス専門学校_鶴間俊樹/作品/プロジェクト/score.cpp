//=============================================================================
//
// �X�R�A���� [score.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "score.h"
#include "object2D.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CScore::CScore()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CScore::CScore(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize)
{
	m_pos = pos;
	m_fSize = fSize;
	m_nScore = 0;

	SetTexType(type);	//�ۑ��p

	//��������2D�I�u�W�F�N�g�𐶐�
	for (int nCnt = 0; nCnt < nNumDigit; nCnt++)
	{
		m_vNumberPtr.push_back(CObject2D::Create());
		if (m_vNumberPtr[nCnt] == nullptr) continue;

		//�i���o�[�̏�����
		m_vNumberPtr[nCnt]->SetTexType(type);
		m_vNumberPtr[nCnt]->SetPos(m_pos + D3DXVECTOR3(-m_fSize / 2.0f, m_fSize / 2.0f, 0.0f) + D3DXVECTOR3(-m_fSize, 0.0f, 0.0f) * (float)nCnt);	//�X�R�A�S�̂̉E�オpos�ƂȂ�悤��
		m_vNumberPtr[nCnt]->SetSize(D3DXVECTOR3(m_fSize, m_fSize, 0.0f));
	}

	m_nDigitMin = 1;
	//m_nDigitMin = nNumDigit;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CScore::~CScore()
{

}

//=============================================================================
// �X�R�A�̐�������
//=============================================================================
CScore* CScore::Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize) {
	CScore* pScore;
	pScore = new CScore(nNumDigit, type, pos, fSize);
	if (pScore == nullptr) return nullptr;

	pScore->Init();

	return pScore;
}

//=============================================================================
// �X�R�A�̏���������
//=============================================================================
HRESULT CScore::Init(void) {
	//�����̐ݒ�
	SetScore(m_nScore);

	return S_OK;
}

//=============================================================================
// �X�R�A�̏I������
//=============================================================================
void CScore::Uninit(void) {
	for (unsigned int nCnt = 0; nCnt < m_vNumberPtr.size(); nCnt++) {
		if (m_vNumberPtr[nCnt] == nullptr) continue;
		//�����̔j��
		m_vNumberPtr[nCnt]->Uninit();
		m_vNumberPtr[nCnt] = nullptr;
	}

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �X�R�A�̍X�V����
//=============================================================================
void CScore::Update(void) {
	//�i���o�[�̍X�V����
	for (unsigned int nCnt = 0; nCnt < m_vNumberPtr.size(); nCnt++) {
		if (m_vNumberPtr[nCnt] == nullptr) continue;
		//�X�V����
		(m_vNumberPtr[nCnt])->Update();
	}
}

//=============================================================================
// �X�R�A�̐ݒ�
//=============================================================================
void CScore::SetScore(int nScore) {
	m_nScore = nScore;

	//�����̒���
	int nScoreDigit = std::to_string(m_nScore).length();		//�X�R�A�̌���
	if (nScoreDigit < m_nDigitMin) nScoreDigit = m_nDigitMin;	//�X�R�A�̌������ŏ�����菭�Ȃ���Βu��������
	int nSizeNumberVec = m_vNumberPtr.size();				//�����̃x�N�^�[�̃T�C�Y
	int nDeltaDigit = nScoreDigit - nSizeNumberVec;			//�X�R�A�̌����ƌ��݂̌����̍���

	//�����ǉ�
	for (int nCnt = nSizeNumberVec; nCnt < nDeltaDigit + nSizeNumberVec; nCnt++)
	{
		//�V�����i���o�[��ǉ�
		m_vNumberPtr.push_back(CObject2D::Create());

		//�i���o�[�̏����ݒ�
		m_vNumberPtr[nCnt]->SetTexType(GetTexType());
		m_vNumberPtr[nCnt]->SetPos(m_pos + D3DXVECTOR3(-m_fSize / 2.0f, m_fSize / 2.0f, 0.0f) + D3DXVECTOR3(-m_fSize, 0.0f, 0.0f) * (float)nCnt);	//�X�R�A�S�̂̉E�オpos�ƂȂ�悤��
		m_vNumberPtr[nCnt]->SetSize(D3DXVECTOR3(m_fSize, m_fSize, 0.0f));
	}

	//�����폜
	if (nDeltaDigit < 0) {
		int nDeleteDigit = m_vNumberPtr.size() - m_nDigitMin;	//�폜�\�Ȍ���
		if (nDeleteDigit > abs(nDeltaDigit)) nDeleteDigit = abs(nDeltaDigit);	//�폜���錅�����폜�\�ȍő吔��菬�����ꍇ
		//�傫�����̌�������폜
		for (int nCnt = 0; nCnt < nDeleteDigit; nCnt++)
		{		
			//�I�u�W�F�N�g�̏I������
			if (m_vNumberPtr.back() != nullptr) m_vNumberPtr.back()->Uninit();
			//���X�g����폜
			m_vNumberPtr.pop_back();
		}
	}

	//UV���W�̍X�V
	for (unsigned int nCnt = 0; nCnt < m_vNumberPtr.size(); nCnt++) {
		(m_vNumberPtr[nCnt])->SetTexNumber((m_nScore % (int)pow(10, nCnt + 1)) / (int)pow(10, nCnt));
	}
}

//=============================================================================
// �X�R�A�̒ǉ�
//=============================================================================
void CScore::AddScore(int nScore) {
	//�X�R�A�̐ݒ�
	SetScore(m_nScore + nScore);
}

//=============================================================================
// �X�R�A�̎擾
//=============================================================================
int CScore::GetScore(void) {
	return m_nScore;
}