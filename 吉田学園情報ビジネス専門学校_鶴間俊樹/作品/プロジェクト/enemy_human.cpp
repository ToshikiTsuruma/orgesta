//=============================================================================
//
// �l�`�G���� [enemy_human.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "enemy_human.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define TEXT_FILE_NAME_LOAD_MOTION "data/MOTION/motion_enemyHuman.txt"
#define MOVE_SPEED (0.7f)			//�ړ����x
#define ROTATE_SPEED (0.03f * D3DX_PI)		//��]���x
#define NUM_COLLISION (10)			//�����蔻��̐�
#define COLLISION_RADIUS (15.0f)	//�����蔻��̔��a
#define MAX_LIFE (70)				//�̗͂̍ő�l
#define DISTANCE_ATTACK (60.0f)		//�U�����鋗��
#define ATTACK_DAMAGE (40)	//�U����
#define LIFE_GAUGE_HEIGHT (100.0f)	//�G�̈ʒu����̗̑̓Q�[�W�̍���

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================
int CEnemyHuman::m_nNumParts = 0;
CObjectMotion::PARTS_INFO* CEnemyHuman::m_pPartsInfoArray = nullptr;
CObjectMotion::MOTION_INFO CEnemyHuman::m_aMotionInfo[(int)CEnemyHuman::MOTION_TYPE::ENUM_MAX] = {};

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CEnemyHuman::CEnemyHuman() : CEnemy(m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX, MOVE_SPEED, ROTATE_SPEED, MAX_LIFE, DISTANCE_ATTACK, LIFE_GAUGE_HEIGHT)
{
	//�|�����X�R�A�̐ݒ�
	SetKillScore(500);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CEnemyHuman::~CEnemyHuman()
{

}

//=============================================================================
// �l�`�G�̐�������
//=============================================================================
CEnemyHuman* CEnemyHuman::Create(D3DXVECTOR3 pos) {
	CEnemyHuman* pEnemyHuman;
	pEnemyHuman = new CEnemyHuman();
	if (pEnemyHuman != nullptr) {
		pEnemyHuman->SetPos(pos);
		pEnemyHuman->Init();
	}

	return pEnemyHuman;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CEnemyHuman::LoadMotionInfo(void) {
	//���[�h����O�ɓ��I�m�ۂ�����������delete����
	UnloadMotionInfo();

	LoadMotionText(TEXT_FILE_NAME_LOAD_MOTION, m_pPartsInfoArray, m_nNumParts, &m_aMotionInfo[0], (int)MOTION_TYPE::ENUM_MAX);
}

//=============================================================================
// ���[�V���������A�����[�h
//=============================================================================
void CEnemyHuman::UnloadMotionInfo(void) {
	m_nNumParts = 0;

	//�p�[�c���̉��
	if (m_pPartsInfoArray != nullptr) {
		delete[] m_pPartsInfoArray;
		m_pPartsInfoArray = nullptr;
	}
	//���[�V�������̉��
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
// �l�`�G�̏���������
//=============================================================================
HRESULT CEnemyHuman::Init(void) {
	SetMotion((int)MOTION_TYPE::NEUTRAL);	//���[�V�����̏����ݒ�

	//�G�̏���������
	CEnemy::Init();
	return S_OK;
}

//=============================================================================
// �l�`�G�̏I������
//=============================================================================
void CEnemyHuman::Uninit(void) {
	//�I������
	CEnemy::Uninit();
}

//=============================================================================
// �l�`�G�̍X�V����
//=============================================================================
void CEnemyHuman::Update(void) {
	//�G�̍X�V
	CEnemy::Update();
}

//=============================================================================
// �l�`�G�̕`�揈��
//=============================================================================
void CEnemyHuman::Draw(void) {
	CEnemy::Draw();
}

//=============================================================================
// �l�`�G�̓����蔻��̏��̎擾
//=============================================================================
void CEnemyHuman::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
	//�K�v�ȃ|�C���^��null�̏ꍇ�I��
	if (pNumCol == nullptr || pRadiusCol == nullptr || ppPosColArray == nullptr) return;

	//pos�̃|�C���^�����łɑ��݂��Ă���ꍇ�I��
	if (*ppPosColArray != nullptr) return;

	//�����蔻��̐���ݒ�
	*pNumCol = NUM_COLLISION;
	//�����蔻��̔��a��ݒ�
	*pRadiusCol = COLLISION_RADIUS;
	//�����蔻��̈ʒu�̔z���ݒ�
	*ppPosColArray = new D3DXVECTOR3[NUM_COLLISION];	//���I�m��

	D3DXVECTOR3 posOffset = D3DXVECTOR3(0.0f, 0.0f, 0.0f);	//���f����pos����̃I�t�Z�b�g�̎n�_
	int nIdxModel = 0;	//����

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		//�����蔻��̈ʒu�̎擾
		GetPosCollision(*ppPosColArray + nCntCol, posOffset, nIdxModel);
		posOffset += D3DXVECTOR3(0.0f, 5.0f, 0.0f);	//�I�t�Z�b�g�����Z
	}
}

//=============================================================================
// ���[�V�����̋敪�̎擾
//=============================================================================
CEnemy::MOTION_CATEGORY CEnemyHuman::GetMotionCategory(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�j���[�g����
	case MOTION_TYPE::NEUTRAL:
	default:
		return MOTION_CATEGORY::NEUTRAL;

		//�ړ�
	case MOTION_TYPE::WALK:
	case MOTION_TYPE::DASH:
		return MOTION_CATEGORY::MOVE;

		//�U��
	case MOTION_TYPE::ATTACK:
		return MOTION_CATEGORY::ATTACK;

		//���S
	case MOTION_TYPE::DEAD:
		return MOTION_CATEGORY::DEAD;
	}
}

//=============================================================================
// �ړ����̃��[�V������ݒ�
//=============================================================================
void CEnemyHuman::SetMoveMotion(void) {
	SetMotion((int)MOTION_TYPE::WALK);
}

//=============================================================================
// ���S���̃��[�V������ݒ�
//=============================================================================
void CEnemyHuman::SetDeadMotion(void) {
	SetMotion((int)MOTION_TYPE::DEAD);
}

//=============================================================================
// �U���J�n��
//=============================================================================
void CEnemyHuman::AttackStart(void) {
	//�U�����[�V�����̐ݒ�
	SetMotion((int)MOTION_TYPE::ATTACK);
}

//=============================================================================
// �U����~
//=============================================================================
void CEnemyHuman::AttackStop(void) {

}

//=============================================================================
// ���[�V�����I����
//=============================================================================
void CEnemyHuman::MotionEnd(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�U�����[�V����
	case MOTION_TYPE::ATTACK:
		SetMotion((int)MOTION_TYPE::NEUTRAL);
		break;
	}
}

//=============================================================================
// ���[�V�������̍s��
//=============================================================================
void CEnemyHuman::MotionAct(void) {
	switch ((MOTION_TYPE)GetMotionType())
	{
		//�U��
	case MOTION_TYPE::ATTACK:
		//�U���̃^�C�~���O
		if ((GetCurKey() == 1 || GetCurKey() == 2) && !GetTransMotion()) {
			const int nNumCollision = 1;	//�U���̓����蔻��̐�
			D3DXVECTOR3 aPosCol[nNumCollision] = {};	//�����蔻��̈ʒu
			int nIdxModel = 3;	//�E��
			D3DXVECTOR3 posOffset = D3DXVECTOR3(-30.0f, 0.0f, 0.0f);	//���f������̃I�t�Z�b�g

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//�����蔻��̈ʒu�̎擾
				GetPosCollision(&aPosCol[nCntCol], posOffset, nIdxModel);
				//�I�t�Z�b�g�����Z
				posOffset += D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
			}

			//�����蔻��̐����U������
			for (int nCntAttack = 0; nCntAttack < nNumCollision; nCntAttack++)
			{
				//�U��
				CObjectMotion::Attack(ENEMY_ATTACK_TARGET, aPosCol[nCntAttack], 10.0f, ATTACK_DAMAGE, DAMAGE_TYPE::ENEMY_PUNCH, nullptr);
			}
		}
		break;	//_�U��
	}
}