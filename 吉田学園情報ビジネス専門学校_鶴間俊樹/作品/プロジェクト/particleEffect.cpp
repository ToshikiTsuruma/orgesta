//=============================================================================
//
// �p�[�e�B�N���G�t�F�N�g���� [particleEffect.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "particleEffect.h"

//=============================================================================
// �}�N����`
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CParticleEffect::CParticleEffect()
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CParticleEffect::CParticleEffect(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit, bool bRandomAngle)
{
	m_particleInfo = particleInfo;
	m_pos = pos;
	m_bLoop = false;
	if (nLife <= 0) m_bLoop = true;	//���C�t���O�ȉ��Ȃ烋�[�v
	m_nLife = nLife;
	m_nCntCreate = 0;
	m_nSpanCreate = nSpanCreate;
	m_fAngleEmit = fAngleEmit;
	m_bRandomAngle = bRandomAngle;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CParticleEffect::~CParticleEffect()
{

}

//=============================================================================
// �p�[�e�B�N���G�t�F�N�g�̐�������
//=============================================================================
CParticleEffect* CParticleEffect::Create(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nLife, int nSpanCreate, float fAngleEmit, bool bRandomAngle) {
	CParticleEffect* pParticleEffect;
	pParticleEffect = new CParticleEffect(particleInfo, pos, nLife, nSpanCreate, fAngleEmit, bRandomAngle);
	if (pParticleEffect == nullptr) return nullptr;

	pParticleEffect->Init();

	return pParticleEffect;
}

//=============================================================================
// �p�[�e�B�N���G�t�F�N�g�̏���������
//=============================================================================
HRESULT CParticleEffect::Init(void) {
	m_nCntCreate = m_nSpanCreate;	//����������p�[�e�B�N���𐶐��ł���悤�ɂ���
	return S_OK;
}

//=============================================================================
// �p�[�e�B�N���G�t�F�N�g�̏I������
//=============================================================================
void CParticleEffect::Uninit(void) {
	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// �p�[�e�B�N���G�t�F�N�g�̍X�V����
//=============================================================================
void CParticleEffect::Update(void) {
	//���[�v���Ȃ��ꍇ
	if (!m_bLoop) {
		//���C�t���O�ȉ�
		if (m_nLife <= 0) {
			//�G�t�F�N�g�j��
			Uninit();
			return;
		}
		else {
			//���C�t�̌���
			m_nLife--;
		}
	}

	if (m_nCntCreate >= m_nSpanCreate) {
		//�����J�E���g�̃��Z�b�g
		m_nCntCreate = 0;

		D3DXVECTOR3 moveParticle;	//�p�[�e�B�N���̈ړ���
		//�����_����XZ���ʂ̈ړ��̊p�x�����߂�
		float fRandRotXZ = (rand() % 624) / 100.0f;	//�����_���̊p�x

		float fAngleEmit = m_fAngleEmit;	//���o�p�x
		//�͈͓��̃����_���ŕ��˂���p�x�����߂�
		if (m_bRandomAngle) fAngleEmit = (rand() % (int)(m_fAngleEmit * 100.0f)) / 100.0f;	//�����_���̊p�x

		//�p�[�e�B�N���̈ړ��ʂ̂����߂�
		moveParticle = D3DXVECTOR3(sinf(fRandRotXZ) * m_particleInfo.fSpeedMove * sinf(fAngleEmit),
			cosf(fAngleEmit) * m_particleInfo.fSpeedMove,
			cosf(fRandRotXZ) * m_particleInfo.fSpeedMove * sinf(fAngleEmit));

		//�p�[�e�B�N���̐���
		CParticle::Create(m_pos, m_particleInfo.nLife, m_particleInfo.fSizeStart, m_particleInfo.fAddSize, moveParticle, m_particleInfo.addMove, m_particleInfo.colStart, m_particleInfo.colEnd);
	}

	//�����̃J�E���g�̉��Z
	m_nCntCreate++;
}

//=============================================================================
// �~��Ƀp�[�e�B�N������˂���
//=============================================================================
void CParticleEffect::EmitCircle(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreate, float fAngleEmit) {
	//�p�[�e�B�N�����������[�v
	for (int nCnt = 0; nCnt < nNumCreate; nCnt++)
	{
		D3DXVECTOR3 moveParticle;	//�p�[�e�B�N���̈ړ���
		//XZ���ʂ̕��˂̊p�x�����߂�
		float fRotEmit = (nCnt / (float)nNumCreate) * D3DX_PI * 2;
		//�p�[�e�B�N���̈ړ��ʂ̂����߂�
		moveParticle = D3DXVECTOR3(sinf(fRotEmit) * particleInfo.fSpeedMove * sinf(fAngleEmit),
		cosf(fAngleEmit) * particleInfo.fSpeedMove,
		cosf(fRotEmit) * particleInfo.fSpeedMove * sinf(fAngleEmit));
		//�p�[�e�B�N���̐���
		CParticle::Create(pos, particleInfo.nLife, particleInfo.fSizeStart, particleInfo.fAddSize, moveParticle, particleInfo.addMove, particleInfo.colStart, particleInfo.colEnd);
	}
}

//=============================================================================
// ����Ƀp�[�e�B�N������˂���
//=============================================================================
void CParticleEffect::EmitSphere(PARTICLE_INFO particleInfo, D3DXVECTOR3 pos, int nNumCreateXZ, int nNumCreateY, float fAddSpeed) {
	//�p�[�e�B�N�����������[�v
	for (int nCntXZ = 0; nCntXZ < nNumCreateXZ; nCntXZ++)
	{
		//XZ���ʂ̕��˂̊p�x�����߂�
		float fRotEmitXZ = (nCntXZ / (float)nNumCreateXZ) * D3DX_PI * 2;

		for (int nCntY = 0; nCntY < nNumCreateY; nCntY++)
		{
			//Y�����̕��˂̊p�x�����߂�
			float fRotEmitY = (nCntY / (float)(nNumCreateY - 1)) * D3DX_PI;

			D3DXVECTOR3 moveParticle;	//�p�[�e�B�N���̈ړ���
			//�p�[�e�B�N���̈ړ��ʂ̂����߂�
			moveParticle = D3DXVECTOR3(sinf(fRotEmitXZ) * sinf(fRotEmitY), cosf(fRotEmitY), cosf(fRotEmitXZ) * sinf(fRotEmitY)) * particleInfo.fSpeedMove;

			D3DXVECTOR3 addMove;	//������
			D3DXVec3Normalize(&addMove, &moveParticle);
			addMove *= fAddSpeed;

			//�p�[�e�B�N���̐���
			CParticle::Create(pos, particleInfo.nLife, particleInfo.fSizeStart, particleInfo.fAddSize, moveParticle, particleInfo.addMove + addMove, particleInfo.colStart, particleInfo.colEnd);
		}
	}
}