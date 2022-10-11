//=============================================================================
//
// �ь�̖؂̏��� [appleTree.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "appleTree.h"
#include "renderer.h"
#include "manager.h"
#include "input.h"
#include "sound.h"
#include "appleMenu.h"
#include "gauge2D.h"
#include "gameScene.h"
#include "object2D.h"
#include "effect.h"
#include "particleEffect.h"

#include "apple_red.h"
#include "apple_green.h"
#include "apple_white.h"
#include "apple_black.h"
#include "apple_silver.h"
#include "apple_gold.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define NUM_COLLISION (8)			//�����蔻��̐�
#define COLLISION_RADIUS (20.0f)	//�����蔻��̔��a

#define DANGER_LIFE ((int)(MAX_LIFE_APPLETREE * 0.35f))		//�̗͂̊댯�l
#define COLOR_LIFE_GAUGE_SAFE (D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f))	//�̗̓o�[�̈��S���̐F
#define COLOR_LIFE_GAUGE_DANGER (D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f))	//�̗̓o�[�̊댯���̐F

#define DEAD_COLOR (D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))	//���S��̐F
#define TIME_FINISH_CHANGE_COLOR_DEAD (FPS * 2)	//���S��̐F�̕ύX���I�����鎞��

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CAppleTree::CAppleTree() : CObjectModel(CModel::MODELTYPE::OBJ_APPLE_TREE, false)
{
	m_nNumApple = 0;
	for (int nCnt = 0; nCnt < MAX_NUM_CREATE_APPLE; nCnt++)
	{
		m_apCreateApple[nCnt] = nullptr;
		m_aTypeCreateApple[nCnt] = (CGlowApple::APPLE_TYPE) -1;
	}

	m_pMenuApple = nullptr;
	m_nMaxLife = MAX_LIFE_APPLETREE;
	m_nLife = MAX_LIFE_APPLETREE;
	m_bDead = false;

	//�̗̓Q�[�W�̐���
	m_pGaugeLife = CGauge2D::Create(MAX_LIFE_APPLETREE, false, MAX_LIFE_APPLETREE, 0, true);
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(150.0f + 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeLife->CreateGauge(CTexture::TEXTURE_TYPE::GAUGE_HORIZONTAL, D3DXVECTOR3(150.0f + 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 200.0f, 20.0f);
		m_pGaugeLife->SetGaugeColor(COLOR_LIFE_GAUGE_SAFE);
		m_pGaugeLife->SetGaugeColorDanger(COLOR_LIFE_GAUGE_DANGER);
		m_pGaugeLife->SetDangerValue(DANGER_LIFE);
		m_pGaugeLife->CreateGaugeFrame(CTexture::TEXTURE_TYPE::HP_GAUGE_FRAME, D3DXVECTOR3(150.0f + 20.0f, SCREEN_HEIGHT - 30.0f, 0.0f), 200.0f, 20.0f);
	}
	//HP�A�C�R���̐���
	m_pIconHP = CObject2D::Create(D3DXVECTOR3(52.0f, SCREEN_HEIGHT - 30.0f, 0.0f), CTexture::TEXTURE_TYPE::ICON_HP, 40.0f, 40.0f);

	//�����Q�[�W�̐���
	m_nGrowValue = 0;
	m_nGrowValueMax = 3;
	m_pGaugeGrow = CGauge2D::Create(m_nGrowValueMax, false, m_nGrowValue, 0, false);
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->CreateGaugeBG(CTexture::TEXTURE_TYPE::NONE, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 10.0f, 0.0f), SCREEN_WIDTH, 20.0f);
		m_pGaugeGrow->SetGaugeBGColor(D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
		m_pGaugeGrow->CreateGauge(CTexture::TEXTURE_TYPE::GAUGE_HORIZONTAL, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 10.0f, 0.0f), SCREEN_WIDTH - 6.0f, 20.0f - 6.0f);
		m_pGaugeGrow->SetGaugeColor(D3DXCOLOR(0.0f, 0.4f, 1.0f, 1.0f));
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
		m_pGaugeGrow->CreateGaugeFrame(CTexture::TEXTURE_TYPE::GROW_GAUGE_FRAME, D3DXVECTOR3(SCREEN_WIDTH / 2.0f, 10.0f, 0.0f), SCREEN_WIDTH, 20.0f);
	}

	SetObjType(OBJTYPE_APPLETREE);
	SetEnableCollision(true);
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CAppleTree::~CAppleTree()
{

}

//=============================================================================
// �ь�̖؂̐�������
//=============================================================================
CAppleTree* CAppleTree::Create(D3DXVECTOR3 pos) {
	CAppleTree* pAppleTree;
	pAppleTree = new CAppleTree();
	if (pAppleTree == nullptr) return nullptr;

	pAppleTree->SetPos(pos);
	pAppleTree->Init();

	return pAppleTree;
}

//=============================================================================
// �ь�̖؂̏���������
//=============================================================================
HRESULT CAppleTree::Init(void) {
	CModel* pModel = GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(D3DXCOLOR(1.0f, 0.8f, 0.4f, 1.0f));
	}

	CObjectModel::Init();
	return S_OK;
}

//=============================================================================
// �ь�̖؂̏I������
//=============================================================================
void CAppleTree::Uninit(void) {
	//�ь�̃��j���[�̔j��
	if (m_pMenuApple != nullptr) {
		m_pMenuApple->Uninit();
		m_pMenuApple = nullptr;
	}
	//�̗̓Q�[�W�̔j��
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->Uninit();
		m_pGaugeLife = nullptr;
	}
	//�����Q�[�W�̔j��
	if (m_pGaugeGrow != nullptr) {
		m_pGaugeGrow->Uninit();
		m_pGaugeGrow = nullptr;
	}
	//HP�A�C�R���̔j��
	if (m_pIconHP != nullptr) {
		m_pIconHP->Uninit();
		m_pIconHP = nullptr;
	}

	//�����S�̔j��
	for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
	{
		if (m_apCreateApple[nCntApple] == nullptr) continue;

		//�������������S�����S������
		m_apCreateApple[nCntApple]->Dead();
		m_apCreateApple[nCntApple] = nullptr;
	}


	CObjectModel::Uninit();
}

//=============================================================================
// �ь�̖؂̍X�V����
//=============================================================================
void CAppleTree::Update(void) {
	CObjectModel::Update();

	//���S���̍X�V
	if (m_bDead) {
		//���S��̃J�E���g��i�߂�
		m_nCntDead++;

		return;
	}

#ifdef _DEBUG
	CManager* pManager = CManager::GetManager();	//�}�l�[�W���[�̎擾
	CInput* pInput = nullptr;	//���݂̓��̓f�o�C�X�ւ̃|�C���^
	if (pManager != nullptr) pInput = pManager->GetInputCur();

	if (pInput != nullptr) {
		//����������
		if (pInput->GetTrigger(CInput::CODE::DEBUG_2)) AddGrow(100);
	}
#endif

}

//=============================================================================
// �ь�̖؂̕`�揈��
//=============================================================================
void CAppleTree::Draw(void) {
	CObjectModel::Draw();
}

//=============================================================================
// �ь�̐���
//=============================================================================
CGlowApple* CAppleTree::CreateApple(CGlowApple::APPLE_TYPE typeApple, D3DXVECTOR3 posCreate, CAppleTree* pParentTree) {
	CGlowApple* pCreateApple = nullptr;	//�������������S

	switch (typeApple)
	{
	case CGlowApple::APPLE_TYPE::RED:
		pCreateApple = CAppleRed::Create(posCreate, pParentTree);
		break;
	case CGlowApple::APPLE_TYPE::GREEN:
		pCreateApple = CAppleGreen::Create(posCreate, pParentTree);
		break;
	case CGlowApple::APPLE_TYPE::WHITE:
		pCreateApple = CAppleWhite::Create(posCreate, pParentTree);
		break;
	case CGlowApple::APPLE_TYPE::BLACK:
		pCreateApple = CAppleBlack::Create(posCreate, pParentTree);
		break;
	case CGlowApple::APPLE_TYPE::SILVER:
		pCreateApple = CAppleSilver::Create(posCreate, pParentTree);
		break;
	case CGlowApple::APPLE_TYPE::GOLD:
		pCreateApple = CAppleGold::Create(posCreate, pParentTree);
		break;
	}

	return pCreateApple;
}

//=============================================================================
// �ь炪��������
//=============================================================================
void CAppleTree::YieldApple(CGlowApple::APPLE_TYPE typeApple) {
	//�ь�̈ʒu���擾
	D3DXVECTOR3 posCreate = GetPos() + GetOffsetPosApple(m_nNumApple);	//�؂̈ʒu + �I�t�Z�b�g

	//�����S�̐���
	CGlowApple* pCreateApple = CreateApple(typeApple, posCreate, this);

	//�������ɃG�t�F�N�g�𐶐�
	CParticleEffect::PARTICLE_INFO particleInfo;	//�p�[�e�B�N�����
	particleInfo.addMove = D3DXVECTOR3(0.0f, -0.8f, 0.0f); particleInfo.colEnd = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); particleInfo.colStart = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
	particleInfo.fAddSize = -0.8f; particleInfo.fSizeStart = 40.0f; particleInfo.fSpeedMove = 10.0f; particleInfo.nLife = 60;
	//���d�˂Đ���
	for (int nCnt = 0; nCnt < 8; nCnt++)
	{
		CParticleEffect::Create(particleInfo, posCreate, 1, 1, 0.1f * D3DX_PI, true);
	}

	//�������������S��z��ɓo�^
	if (m_nNumApple >= 0 && m_nNumApple < MAX_NUM_CREATE_APPLE) {
		m_apCreateApple[m_nNumApple] = pCreateApple;
		m_aTypeCreateApple[m_nNumApple] = typeApple;
	}
	//�ь�̐����������Z
	m_nNumApple++;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;	//�T�E���h�ւ̃|�C���^
	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�������̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::CREATE_APPLE);

	//���j���[�����
	if (m_pMenuApple != nullptr) {
		m_pMenuApple->Uninit();
		m_pMenuApple = nullptr;
	}

	if (m_nNumApple < MAX_NUM_CREATE_APPLE) {
		//�g�p���������x�������������
		m_nGrowValue -= m_nGrowValueMax;

		//�����̕K�v�ʂ�ݒ�
		int nGrowValueMax = 0;	//�V���������K�v��
		//�����S�̐��ɂ���ĕύX
		switch (m_nNumApple)
		{
		case 0:
			nGrowValueMax = 3;
			break;
		case 1:
			nGrowValueMax = 80;
			break;
		case 2:
			nGrowValueMax = 120;
			break;
		case 3:
			nGrowValueMax = 200;
			break;
		case 4:
			nGrowValueMax = 300;
			break;
		case 5:
			nGrowValueMax = 500;
			break;
		default:
			nGrowValueMax = 999;
			break;
		}
		//�K�v�ʂ̐ݒ�
		m_nGrowValueMax = nGrowValueMax;

		//�����Q�[�W�̐ݒ�
		if (m_pGaugeGrow != nullptr) {
			m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
			m_pGaugeGrow->SetMaxValue(m_nGrowValueMax);
		}
		//���������ꂽ�����ʂ��K�v�ʂ𒴂��Ă����ꍇ�A�A���Ő��������邽�߂�0�̐����ʂ𑫂�
		AddGrow(0);
	}
	else {
		//�����Q�[�W�̂̐F��ύX
		if (m_pGaugeGrow != nullptr) {
			m_pGaugeGrow->SetGaugeColor(D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));
		}
	}
}

//=============================================================================
// �̗͂��񕜂���
//=============================================================================
void CAppleTree::Heal(int nHeal) {
	m_nLife += nHeal;
	if (m_nLife > m_nMaxLife) m_nLife = m_nMaxLife;

	if (m_pGaugeLife != nullptr) {
		//�̗̓Q�[�W�̐ݒ�
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// �ь�̖؂̃_���[�W
//=============================================================================
void CAppleTree::Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();

	D3DXVECTOR3 posEffectOffset = D3DXVECTOR3(0.0f, 100.0f, 0.0f);	//�G�t�F�N�g�����ʒu�̃I�t�Z�b�g
	float afRandPos[3];	//�I�t�Z�b�g�ɉ��Z���郉���_���Ȓl
	//�I�t�Z�b�g�������_���ł��炷
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		int nRandPos = rand() % 40 - 20;
		afRandPos[nCnt] = (float)nRandPos;
	}
	//�I�t�Z�b�g�Ƀ����_���Ȓl�����Z
	posEffectOffset.x += afRandPos[0];
	posEffectOffset.y += afRandPos[1];
	posEffectOffset.z += afRandPos[2];

	//�U���̃^�C�v���Ƃ̏���
	switch (typeDamage)
	{
	case DAMAGE_TYPE::PLAYER_PUNCH:
		//�U���G�t�F�N�g�̐���
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::DAMAGE_PLAYER, 60.0f, 60.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		//�_���[�W�ʂ�{�ɂ���
		//nDamage *= FOLD_PLAYER_DAMAGE;
		break;
	case DAMAGE_TYPE::ENEMY_PUNCH:
		//�U���G�t�F�N�g�̐���
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::DAMAGE_ENEMY, 60.0f, 60.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_PUNCH);
		break;
	case DAMAGE_TYPE::FIRE:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::FIRE, 100.0f, 100.0f, false);
		break;
	case DAMAGE_TYPE::THUNDERBOLT:
		//�_���[�W�G�t�F�N�g�̐���
		CEffect::Create(GetPos() + posEffectOffset, CEffect::EFFECT_TYPE::THUNDER, 100.0f, 100.0f, false);
		//�_���[�W���̍Đ�
		if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::DAMAGE_THUNDER);
		//�_���[�W�ʂ�{�ɂ���
		//nDamage *= FOLD_PLAYER_DAMAGE;
		break;
	}

	//���S���Ă����ꍇ�I��
	if (m_bDead) return;

	//�_���[�W
	m_nLife -= nDamage;

	if (m_nLife <= 0) {
		m_nLife = 0;
		//���S
		if (pDead != nullptr) *pDead = true;
		Dead();
	}
	else {
		if (pDead != nullptr) *pDead = false;
	}
	//�̗̓Q�[�W�̐ݒ�
	if (m_pGaugeLife != nullptr) {
		m_pGaugeLife->SetGaugeValue(m_nLife);
	}
}

//=============================================================================
// ���S
//=============================================================================
void CAppleTree::Dead(void) {
	//���S�̐ݒ�
	m_bDead = true;
	//���S�J�E���g�̏�����
	m_nCntDead = 0;

	CEffect::Create(GetPos() + D3DXVECTOR3(0.0f, 400.0f, 0.0f), CEffect::EFFECT_TYPE::DEATH, 666.0f, 666.0f, false);

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	CSound* pSound = nullptr;
	CGameScene* pGame = nullptr;

	//�T�E���h�̎擾
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Q�[���̎擾
	if (pManager != nullptr) pGame = pManager->GetGameScene();

	//���S���̍Đ�
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::DEAD_TREE);
	//�Q�[���I�[�o�[�ݒ�
	if (pGame != nullptr) pGame->GameOver();

	//���f���̎擾
	CModel* pModel = GetPtrModel();
	//���f���̐F�̕ύX���J�n
	if (pModel != nullptr) pModel->StartChangeMaterialDiffuse(0, DEAD_COLOR, TIME_FINISH_CHANGE_COLOR_DEAD);

	//-----------------------
	//�����S�̔j��
	//-----------------------
	for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
	{
		if (m_apCreateApple[nCntApple] == nullptr) continue;

		//�����S�̈ʒu�Ɏ��S�G�t�F�N�g
		CEffect::Create(m_apCreateApple[nCntApple]->GetPos(), CEffect::EFFECT_TYPE::DEATH, 66.6f, 66.6f, false);

		//�������������S�����S������
		m_apCreateApple[nCntApple]->Dead();
		m_apCreateApple[nCntApple] = nullptr;
	}

	//���т����S
	CObject::DeadObjtype(OBJTYPE_SCAPEGOAT);
}

//=============================================================================
// ����
//=============================================================================
void CAppleTree::GrowUp(void) {
	//���łɐ����ς݂̏ꍇ�����S���Ă���ꍇ�I��
	if (m_pMenuApple != nullptr || m_bDead) return;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	if (pManager == nullptr) return;
	//�Q�[���̎擾
	CGameScene* pGame = pManager->GetGameScene();
	if (pGame == nullptr) return;
	//�T�E���h�̎擾
	CSound* pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�Q�[���I����̏ꍇ�I��
	if (pGame->GetGameClear() || pGame->GetGameOver()) return;

	//���j���[�𐶐�
	m_pMenuApple = CAppleMenu::Create(D3DXVECTOR3(0.0f, 2000.0f, 3000.0f), this);
	//������
	if (pSound != nullptr) pSound->PlaySound(CSound::SOUND_LABEL::GROW_UP);
}

//=============================================================================
// �ь琶���ʒu�̎擾
//=============================================================================
D3DXVECTOR3 CAppleTree::GetOffsetPosApple(int nIdxApple) {
	switch (nIdxApple)
	{

	case 0:
		return D3DXVECTOR3(-60.97f, 181.21f, 26.97f);
		break;

	case 1:
		return D3DXVECTOR3(169.03f, 264.21f, -43.97f);
		break;

	case 2:
		return D3DXVECTOR3(-120.97f, 477.21f, -133.97f);
		break;

	case 3:
		return D3DXVECTOR3(254.03f, 537.21f, 154.03f);
		break;

	case 4:
		return D3DXVECTOR3(-103.97f, 583.21f, 104.03f);
		break;

	case 5:
		return D3DXVECTOR3(-227.97f, 479.21f, -51.97f);
		break;


	default:
		return D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		break;
	}
}

//=============================================================================
// �ь�̖؂̓����蔻��̏��̎擾
//=============================================================================
void CAppleTree::GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosColArray, float* const pRadiusCol) {
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

	for (int nCntCol = 0; nCntCol < NUM_COLLISION; nCntCol++)
	{
		(*ppPosColArray)[nCntCol] = D3DXVECTOR3(GetPos().x, GetPos().y + 10.0f * nCntCol, GetPos().z);
	}
}

//=============================================================================
// �̗͂̍ő�l�̐ݒ�
//=============================================================================
void CAppleTree::SetMaxLife(int nMaxLife) {
	//�ő�l��ݒ�
	m_nMaxLife = nMaxLife;	

	if (m_pGaugeLife != nullptr) {
		//�̗̓Q�[�W�̍ő�l�̐ݒ�
		m_pGaugeLife->SetMaxValue(nMaxLife);
	}
}

//=============================================================================
// �̗͂̍ő�l�̑���
//=============================================================================
void CAppleTree::AddMaxLife(int nAddLife) {
	SetMaxLife(m_nMaxLife + nAddLife);
}

//=============================================================================
// �����x�𑝂₷
//=============================================================================
void CAppleTree::AddGrow(int nAddValue) {
	//��������ɒB���Ă���ꍇ�I��
	if (m_nNumApple >= MAX_NUM_CREATE_APPLE) return;

	m_nGrowValue += nAddValue;

	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();
	//�T�E���h�̎擾
	CSound *pSound = nullptr;
	if (pManager != nullptr) pSound = pManager->GetSound();
	//�o���l�擾���̍Đ�
	if (pSound != nullptr) pSound->CSound::PlaySound(CSound::SOUND_LABEL::GETEXP);

	//���������𖞂������ꍇ
	if (m_nGrowValue >= m_nGrowValueMax) {
		//����
		GrowUp();
	}

	if (m_pGaugeGrow != nullptr) {
		//�����Q�[�W�̐ݒ�
		m_pGaugeGrow->SetGaugeValue(m_nGrowValue);
	}
}

//=============================================================================
// �����S�̎�ނ̎擾
//=============================================================================
CGlowApple::APPLE_TYPE CAppleTree::GetCreateAppleType(int nIdx) {
	if (nIdx < 0 || nIdx >= MAX_NUM_CREATE_APPLE) return (CGlowApple::APPLE_TYPE) -1;

	return m_aTypeCreateApple[nIdx];
}

//=============================================================================
// �����S�̐����擾
//=============================================================================
int CAppleTree::GetNumApple(CGlowApple::APPLE_TYPE type) {
	int nNumApple = 0;
	for (int nCntApple = 0; nCntApple < MAX_NUM_CREATE_APPLE; nCntApple++)
	{
		if(m_aTypeCreateApple[nCntApple] == type) nNumApple++;
	}
	return nNumApple;
}