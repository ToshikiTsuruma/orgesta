//=============================================================================
//
// ���[�V�����I�u�W�F�N�g���� [objectMotion.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#define _CRT_SECURE_NO_WARNINGS
#include "objectMotion.h"
#include "renderer.h"
#include "manager.h"
#include "sound.h"
#include "objectList.h"
#include "effect.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_LOAD_TEXT (128)		//��x�Ƀt�@�C������ǂݍ��ރe�L�X�g�̃o�C�g��
#define MAX_ATTACK_DISTANCE (500.0f)	//�I�u�W�F�N�g�ɍU���\�ȍő�̋���
#define CRITICAL_DAMAGE (5)	//�N���e�B�J���_���[�W

//=============================================================================
// �ÓI�����o�ϐ��錾
//=============================================================================

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CObjectMotion::CObjectMotion() : m_nNumParts(0), m_nNumTypeMotion(0)
{

}

//=============================================================================
// �I�[�o�[���[�h���ꂽ�R���X�g���N�^
//=============================================================================
CObjectMotion::CObjectMotion(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline) : m_nNumParts(nNumParts), m_nNumTypeMotion(nNumTypeMotion)
{
	//���f���̃|�C���^�̐���
	m_ppModelArray = new CModel*[nNumParts];
	if (m_ppModelArray != nullptr && pPartsInfoArray != nullptr) {
		//���f���𐶐�
		for (int nCnt = 0; nCnt < nNumParts; nCnt++)
		{
			CModel* pParentParts = nullptr;	//�e���f���̃|�C���^
			int nIdxParent = pPartsInfoArray[nCnt].nIdxParent;	//�e�C���f�b�N�X
			if (nIdxParent >= 0 && nIdxParent < nNumParts) pParentParts = m_ppModelArray[nIdxParent];
			m_ppModelArray[nCnt] = CModel::Create(pPartsInfoArray[nCnt].modelType, pPartsInfoArray[nCnt].offset, D3DXVECTOR3(0.0f, 0.0f, 0.0f), pParentParts, bOutline);
		}
	}

	//���[�V�����X�V���̐���
	m_pUpdateMotionInfoArray = new UPDATE_MOTION_INFO[nNumParts];
	if (m_pUpdateMotionInfoArray != nullptr) {
		//������
		for (int nCnt = 0; nCnt < nNumParts; nCnt++)
		{
			m_pUpdateMotionInfoArray[nCnt].deltaPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pUpdateMotionInfoArray[nCnt].deltaRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pUpdateMotionInfoArray[nCnt].curPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_pUpdateMotionInfoArray[nCnt].curRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			if (pPartsInfoArray != nullptr) m_pUpdateMotionInfoArray[nCnt].offsetPos = pPartsInfoArray[nCnt].offset;
		}
	}

	//���[�V�������̐���
	m_pMotionInfoArray = new MOTION_INFO[nNumTypeMotion];
	if (m_pMotionInfoArray != nullptr && pMotionInfoArray != nullptr) {
		//���[�V�������̐ݒ�
		for (int nCntMotion = 0; nCntMotion < nNumTypeMotion; nCntMotion++)
		{
			for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
			{
				m_pMotionInfoArray[nCntMotion].bLoop = pMotionInfoArray[nCntMotion].bLoop;
				m_pMotionInfoArray[nCntMotion].nNumKey = pMotionInfoArray[nCntMotion].nNumKey;
				m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame = pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].nFrame;

				//�L�[���̐���
				m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray = new KEY[nNumParts];

				if (m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr && pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
					//���̏�����
					for (int nCntKey = 0; nCntKey < nNumParts; nCntKey++)
					{
						m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].pos = pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].pos;
						m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].rot = pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray[nCntKey].rot;
					}
				}
			}
		}
	}

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);
	m_bEndMotion = false;
	m_pListAttacked = new CObjectList;

	m_nTypeMotion = 0;
	m_bLoopMotion = false;
	m_nKey = 0;
	m_nCntMotion = 0;
	m_bTransMotion = false;
	m_bSetCurMotion = false;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CObjectMotion::~CObjectMotion()
{
	if (m_ppModelArray != nullptr) {
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++)
		{
			if (m_ppModelArray[nCntParts] != nullptr) {
				//���f���̔j��
				m_ppModelArray[nCntParts]->Uninit();
				delete m_ppModelArray[nCntParts];
				m_ppModelArray[nCntParts] = nullptr;
			}
		}
		//���f���̃|�C���^�̔z��̔j��
		delete[] m_ppModelArray;
		m_ppModelArray = nullptr;
	}

	if (m_pUpdateMotionInfoArray != nullptr) {
		//���[�V�����̍X�V���̔j��
		delete[] m_pUpdateMotionInfoArray;
		m_pUpdateMotionInfoArray = nullptr;
	}

	if (m_pMotionInfoArray != nullptr) {
		for (int nCntMotion = 0; nCntMotion < m_nNumTypeMotion; nCntMotion++)
		{
			for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
			{
				if (m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray != nullptr) {
					//���[�V���������̃L�[�̔z��̔j��
					delete[] m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray;
					m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeyInfo].pKeyArray = nullptr;
				}
			}
		}
		//���[�V�������̔z��̔j��
		delete[] m_pMotionInfoArray;
		m_pMotionInfoArray = nullptr;
	}

	if (m_pListAttacked != nullptr) delete m_pListAttacked;
}

//=============================================================================
// ���[�V�����������[�h
//=============================================================================
void CObjectMotion::LoadMotionText(char* pLoadDirectory, PARTS_INFO*& pPartsInfoArray, int& nNumParts, MOTION_INFO* pMotionInfoArray, int nNumMotionType) {
	//-------------------------------------
	//�t�@�C���̓ǂݍ���
	//-------------------------------------
	FILE *pFile;	//�t�@�C���ւ̃|�C���^
	char sLoadText[MAX_LOAD_TEXT];		//��s���ǂݍ��ރt�@�C���̃e�L�X�g
	char *pLoadText;	//�t�@�C���̃e�L�X�g�𕪊�����������

	//���f���擾�Ɏg�p����ϐ�
	int nNumLoadModel = 0;	//���[�h�������f����
	CModel::MODELTYPE* pLoadModelTypeArray = nullptr;	//���[�h�������f���̎�ނ̔z��̃|�C���^
	int nCntLoadModel = 0;	//���f���̃f�B���N�g����ǂݍ��񂾐�

	//�p�[�c�ݒ�Ɏg�p����ϐ�
	bool bSetCharacter = false;	//�L�����N�^�[�ݒ蒆���ǂ���
	bool bSetNumParts = false;	//�p�[�c���ݒ肵�����ǂ���
	bool bSetParts = false;	//�p�[�c�ݒ蒆���ǂ���
	int nCntParts = 0;		//�p�[�c�ݒ萔

	//���[�V�������擾�Ɏg�p����ϐ�
	bool bMotionSet = false;	//���[�V�����ݒ蒆���ǂ���
	bool bKeySet = false;		//�L�[���ݒ蒆���ǂ���
	bool bKey = false;			//�L�[�̈ʒu�Ɗp�x��ݒ蒆���ǂ���
	int nCntMotion = 0;	//���[�V�����̐ݒ萔
	int nCntKeySet = 0;	//�L�[�ݒ萔
	int nCntKey = 0;	//�L�[�ݒ肵���p�[�c��

	//�t�@�C�����J��
	pFile = fopen(pLoadDirectory, "r");

	//�t�@�C���ǂݍ��ݎ��s��
	if (pFile == nullptr) return;

	//�ǂݍ��݊J�n�ʒu�܂ōs���΂�
	while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //��s���Ƃɕ�������擾
	{
		pLoadText = strtok(sLoadText, " =\t\n");	//������̕����i�� �^�u ���s ���j
		if (pLoadText == nullptr) continue;
		//�ǂݍ��݊J�n
		if (strcmp(pLoadText, "SCRIPT") == 0) {
			break;
		}
	}

	while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //��s���Ƃɕ�������擾
	{
		pLoadText = strtok(sLoadText, " =\t\n");	//������̕����i�� �^�u ���s ���j
		if (pLoadText == nullptr) continue;
		//�R�����g
		if (strstr(pLoadText, "#") != nullptr) {
			continue;
		}
		//�ǂݍ��ݏI��
		if (strcmp(pLoadText, "END_SCRIPT") == 0) {
			break;
		}

		//-------------------------------------------------
		//���f���ǂݍ���
		//-------------------------------------------------
		//���f�����ݒ�
		if (strcmp(pLoadText, "NUM_MODEL") == 0) {
			pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
			if (pLoadText != nullptr) {
				nNumLoadModel = atoi(pLoadText);	//������𐔎��ɕϊ�
				pLoadModelTypeArray = new CModel::MODELTYPE[nNumLoadModel];	//�ǂݍ��񂾃��f�������̃^�C�v�ۑ��p�̕ϐ��𐶐�
			}
			continue;
		}
		//���f���^�C�v�ݒ�
		if (strcmp(pLoadText, "MODEL_FILENAME") == 0 && pLoadModelTypeArray != nullptr && nCntLoadModel < nNumLoadModel) {
			pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
			if (pLoadText != nullptr) {
				for (int nCntModel = 0; nCntModel < (int)CModel::MODELTYPE::ENUM_MAX; nCntModel++)
				{
					//���f���̃f�B���N�g�����Ɣ�r����
					if (strcmp(pLoadText, CModel::GetPathName((CModel::MODELTYPE)nCntModel)) == 0) {
						//���f���̎�ނ̔ԍ���ݒ�
						pLoadModelTypeArray[nCntLoadModel] = (CModel::MODELTYPE)nCntModel;
						nCntLoadModel++;
						break;
					}
				}
			}
			continue;
		}

		//-------------------------------------------------
		//�L�����N�^�[���ǂݍ���
		//-------------------------------------------------
		//�L�����N�^�[�ݒ�J�n
		if (strcmp(pLoadText, "CHARACTERSET") == 0) {
			bSetCharacter = true;
			//�e�ϐ��̏�����
			bSetNumParts = false;
			nCntParts = 0;

			continue;
		}

		//�L�����N�^�[�ݒ蒆
		if (bSetCharacter) {
			//�L�����N�^�[�ݒ�I��
			if (strcmp(pLoadText, "END_CHARACTERSET") == 0) {
				bSetCharacter = false;

				continue;
			}
			//�p�[�c���ݒ�
			if (strcmp(pLoadText, "NUM_PARTS") == 0 && !bSetNumParts && pMotionInfoArray != nullptr) {
				pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
				if (pLoadText == nullptr) continue;

				nNumParts = atoi(pLoadText);	//������𐔎��ɕϊ�
				//�p�[�c�����p�[�c��������
				pPartsInfoArray = new PARTS_INFO[nNumParts];
				//���[�V�������̃L�[�����p�[�c������
				for (int nCntMotionInfo = 0; nCntMotionInfo < nNumMotionType; nCntMotionInfo++)
				{
					for (int nCntKeyInfo = 0; nCntKeyInfo < MAX_KEY_MOTION; nCntKeyInfo++)
					{
						pMotionInfoArray[nCntMotionInfo].aKeyInfo[nCntKeyInfo].pKeyArray = new KEY[nNumParts];
					}
				}
				bSetNumParts = true;

				continue;
			}


			//-------------------------------------------------
			//�p�[�c���ǂݍ���
			//-------------------------------------------------
			if (pPartsInfoArray == nullptr || !bSetNumParts) continue;
			//�p�[�c�ݒ�J�n
			if (strcmp(pLoadText, "PARTSSET") == 0 && nCntParts < nNumParts) {
				bSetParts = true;
				continue;
			}

			if (!bSetParts) continue;

			//�p�[�c�ݒ�I��
			if (strcmp(pLoadText, "END_PARTSSET") == 0) {
				bSetParts = false;
				//�J�E���^�[���Z
				nCntParts++;
				continue;
			}
			//�ԍ��ݒ�
			if (strcmp(pLoadText, "INDEX") == 0) {
				pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
				if (pLoadText != nullptr) {
					int nIdx = atoi(pLoadText);	//������𐔎��ɕϊ�
					//�C���f�b�N�X�̐ݒ�
					pPartsInfoArray[nCntParts].nIdx = nIdx;
					//���f���̃^�C�v��ݒ�
					pPartsInfoArray[nCntParts].modelType = pLoadModelTypeArray[nIdx];
				}
				continue;
			}
			//�e�p�[�c�̔ԍ��ݒ�
			if (strcmp(pLoadText, "PARENT") == 0) {
				pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
				if (pLoadText != nullptr) {
					pPartsInfoArray[nCntParts].nIdxParent = atoi(pLoadText);	//������𐔎��ɕϊ�
				}
				continue;
			}
			//�I�t�Z�b�g�̐ݒ�
			if (strcmp(pLoadText, "POS") == 0) {
				pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
				if (pLoadText != nullptr) {
					pPartsInfoArray[nCntParts].offset.x = (float)atof(pLoadText);	//X���W�̐ݒ�
					pLoadText = strtok(nullptr, " ");	//������̕���
					pPartsInfoArray[nCntParts].offset.y = (float)atof(pLoadText);	//Y���W�̐ݒ�
					pLoadText = strtok(nullptr, " ");	//������̕���
					pPartsInfoArray[nCntParts].offset.z = (float)atof(pLoadText);	//Z���W�̐ݒ�
				}
				continue;
			}
		}

		//-------------------------------------------------
		//���[�V�����̓ǂݍ���
		//-------------------------------------------------
		if (pMotionInfoArray == nullptr) continue;
		//���[�V�����Z�b�g
		if (strcmp(pLoadText, "MOTIONSET") == 0 && nCntMotion < nNumMotionType) {
			bMotionSet = true;
			nCntKeySet = 0;
			continue;
		}
		//���[�V�����Z�b�g�̏I��
		if (strcmp(pLoadText, "END_MOTIONSET") == 0) {
			bMotionSet = false;
			nCntMotion++;
			continue;
		}

		//----------------------------
		//���[�V�����̐ݒ�
		//----------------------------
		if (!bMotionSet) continue;

		//���[�v
		if (strcmp(pLoadText, "LOOP") == 0) {
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].bLoop = atoi(pLoadText) != 0;
			}
			continue;
		}
		//�L�[��
		if (strcmp(pLoadText, "NUM_KEY") == 0) {
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].nNumKey = atoi(pLoadText);
			}
			continue;
		}
		//�L�[�Z�b�g
		if (strcmp(pLoadText, "KEYSET") == 0 && nCntKeySet < MAX_KEY_MOTION) {
			bKeySet = true;
			nCntKey = 0;
			continue;
		}
		//�G���h�L�[�Z�b�g
		if (strcmp(pLoadText, "END_KEYSET") == 0) {
			nCntKeySet++;
			bKeySet = false;
			continue;
		}
		//�t���[��
		if (strcmp(pLoadText, "FRAME") == 0 && bKeySet) {
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].nFrame = atoi(pLoadText);
			}
			continue;
		}
		//�e�p�[�c���Ƃ̐ݒ�
		if (strcmp(pLoadText, "KEY") == 0 && bKeySet && nCntKey < nNumParts) {
			bKey = true;
			continue;
		}
		//�e�p�[�c���Ƃ̐ݒ�̏I��
		if (strcmp(pLoadText, "END_KEY") == 0 && bKeySet) {
			bKey = false;
			nCntKey++;
			continue;
		}

		//----------------------------
		//�L�[�̐ݒ�
		//----------------------------
		if (pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray == nullptr || !bMotionSet || !bKeySet || !bKey) continue;
		//�ʒu���W�̐ݒ�
		if (strcmp(pLoadText, "POS") == 0) {
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].pos.x = (float)atof(pLoadText);
			}
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].pos.y = (float)atof(pLoadText);
			}
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].pos.z = (float)atof(pLoadText);
			}
			continue;
		}
		//�p�x�̐ݒ�
		if (strcmp(pLoadText, "ROT") == 0) {
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].rot.x = (float)atof(pLoadText);
			}
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].rot.y = (float)atof(pLoadText);
			}
			pLoadText = strtok(nullptr, " =#\t\n");
			if (pLoadText != nullptr) {
				pMotionInfoArray[nCntMotion].aKeyInfo[nCntKeySet].pKeyArray[nCntKey].rot.z = (float)atof(pLoadText);
			}
			continue;
		}
	}

	//�t�@�C�������
	fclose(pFile);

	//���I�m�ۂ������f���̎�ނ�j��
	if (pLoadModelTypeArray != nullptr) {
		delete[] pLoadModelTypeArray;
	}
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̏���������
//=============================================================================
HRESULT CObjectMotion::Init(void) {
	//���[�V������0�t���[���̏�ԂŃX�^�[�g������
	if (m_ppModelArray != nullptr) {
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			if (m_ppModelArray[nCntParts] == nullptr || m_pMotionInfoArray == nullptr || m_pUpdateMotionInfoArray == nullptr) continue;
			if (m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray == nullptr) continue;

			m_ppModelArray[nCntParts]->SetPos(m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].pos + m_pUpdateMotionInfoArray[nCntParts].offsetPos);
			m_ppModelArray[nCntParts]->SetRot(m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].rot);
		}
	}
	return S_OK;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̏I������
//=============================================================================
void CObjectMotion::Uninit(void) {

	//�I�u�W�F�N�g�̔j��
	Release();
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̍X�V����
//=============================================================================
void CObjectMotion::Update(void) {
	if (m_ppModelArray == nullptr) return;	//���f���̔z��null�̏ꍇ�I��

	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] == nullptr) {
			//���f����null�̏ꍇ�I��
			return;
		}
		else {
			//�e���f���̍X�V����
			m_ppModelArray[nCnt]->Update();
		}
	}

	//���[�V�������null�̏ꍇ�I��
	if (m_pMotionInfoArray == nullptr || m_pUpdateMotionInfoArray == nullptr) return;
	
	//���[�V�����̃L�[���null�̏ꍇ�I��
	for (int nCntMotion = 0; nCntMotion < m_nNumTypeMotion; nCntMotion++)
	{
		for (int nCntKey = 0; nCntKey < MAX_KEY_MOTION; nCntKey++)
		{
			if (m_pMotionInfoArray[nCntMotion].aKeyInfo[nCntKey].pKeyArray == nullptr) return;
		}
	}

	//----------------------------------------------------------
	//���[�V�����̑J��
	//----------------------------------------------------------
	if (m_bTransMotion) {
		//���[�V�����J�E���^�[�̉��Z
		m_nCntMotion++;

		//���݂̃��f���̃p�[�c���Ƃ̈ʒu�Ɗp�x�̕ۑ�
		if (!m_bSetCurMotion) {
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curRot = m_ppModelArray[nCntParts]->GetRot();
			}
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curPos = m_ppModelArray[nCntParts]->GetPos();
			}
			m_bSetCurMotion = true;

			//�p�x�̍���
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaRot = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].rot - m_pUpdateMotionInfoArray[nCntParts].curRot;
			}
			//�ʒu�̍���
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaPos = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].pKeyArray[nCntParts].pos - m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].offsetPos;
			}
		}

		//�p�x�̔��f
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 rotNext;	//���̃��f���̊p�x
			rotNext = m_pUpdateMotionInfoArray[nCntParts].curRot + (m_pUpdateMotionInfoArray[nCntParts].deltaRot * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].nFrame);
			//�p�x�̐ݒ�
			m_ppModelArray[nCntParts]->SetRot(rotNext);
		}
		//�ʒu�̔��f
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 posNext;	//���̃��f���̈ʒu���W
			posNext = m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].deltaPos * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].nFrame;
			//�ʒu���W�̐ݒ�
			m_ppModelArray[nCntParts]->SetPos(posNext);
		}
		//�J�E���^�[�����̒l�𒴂����玟�̃��[�V�������n�߂�
		if (m_nCntMotion >= m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[0].nFrame) {
			m_nKey = 0;
			m_nCntMotion = 0;
			m_bTransMotion = false;
			m_bSetCurMotion = false;
		}
	}
	//----------------------------------------------------------
	//�ʏ�̃��[�V�����̏���
	//----------------------------------------------------------
	else if (m_nKey < m_pMotionInfoArray[m_nTypeMotion].nNumKey - 1) {
		//���[�V�����J�E���^�[�̉��Z
		m_nCntMotion++;

		//���݂̃��f���̃p�[�c���Ƃ̈ʒu�Ɗp�x�̕ۑ�
		if (!m_bSetCurMotion) {
			m_bSetCurMotion = true;

			//���݁i�L�[�J�ڎ��j�̊p�x�̐ݒ�
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curRot = m_ppModelArray[nCntParts]->GetRot();
			}
			//���݁i�L�[�J�ڎ��j�̈ʒu�̐ݒ�
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].curPos = m_ppModelArray[nCntParts]->GetPos();
			}

			//�p�x�̍���
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaRot = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].pKeyArray[nCntParts].rot - m_pUpdateMotionInfoArray[nCntParts].curRot;
			}
			//�ʒu�̍���
			for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
				m_pUpdateMotionInfoArray[nCntParts].deltaPos = m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].pKeyArray[nCntParts].pos - m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].offsetPos;
			}
		}
		//�p�x�̔��f
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 rotNext;	//���̃��f���̊p�x
			rotNext = m_pUpdateMotionInfoArray[nCntParts].curRot + m_pUpdateMotionInfoArray[nCntParts].deltaRot * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].nFrame;
			//�p�x�̐ݒ�
			m_ppModelArray[nCntParts]->SetRot(rotNext);
		}
		//�ʒu�̔��f
		for (int nCntParts = 0; nCntParts < m_nNumParts; nCntParts++) {
			D3DXVECTOR3 posNext;	//���̃��f���̈ʒu���W
			posNext = m_pUpdateMotionInfoArray[nCntParts].curPos + m_pUpdateMotionInfoArray[nCntParts].deltaPos * (float)m_nCntMotion / (float)m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].nFrame;
			//�ʒu���W�̐ݒ�
			m_ppModelArray[nCntParts]->SetPos(posNext);
		}

		//�J�E���^�[�����̒l�𒴂����玟�̃��[�V�������n�߂�
		if (m_nCntMotion >= m_pMotionInfoArray[m_nTypeMotion].aKeyInfo[m_nKey + 1].nFrame) {
			m_nCntMotion = 0;
			m_bSetCurMotion = false;
			m_nKey++;

			//�Ō�̃L�[�I����
			if (m_nKey == m_pMotionInfoArray[m_nTypeMotion].nNumKey - 1)
			{
				//���[�v���[�V�����̏ꍇ
				if (m_bLoopMotion) {
					SetMotion(m_nTypeMotion);
				}
				//���[�v���[�V�����ł͖����ꍇ
				else {
					m_bEndMotion = true;
				}
			}
		}
	}
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̕`�揈��
//=============================================================================
void CObjectMotion::Draw(void) {
	if (m_ppModelArray == nullptr) return;	//���f���̔z��null�̏ꍇ�I��

	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//-----------------------------------
	//���[�V�����I�u�W�F�N�g�̃��[���h�}�g���b�N�X�̐ݒ�
	//-----------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//-----------------------------------
	//���f���̕`��
	//-----------------------------------
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] != nullptr) {
			m_ppModelArray[nCnt]->Draw();
		}
	}
}

//=============================================================================
// ���[�V�����̐ݒ�
//=============================================================================
void CObjectMotion::SetMotion(int nType) {
	//���[�V�����^�C�v�̐ݒ�
	m_nTypeMotion = nType;
	//���[�v���邩�ǂ����̐ݒ�
	if(m_pMotionInfoArray != nullptr) m_bLoopMotion = m_pMotionInfoArray[nType].bLoop;
	//�L�[�̏�����
	m_nKey = 0;
	//�J�E���^�[�̏�����
	m_nCntMotion = 0;
	//���[�V�����̕⊮
	m_bTransMotion = true;
	//���݁i�L�[�J�ڎ��j�̕ۑ�
	m_bSetCurMotion = false;
	//���[�V�����̏I�������false�ɐݒ�
	m_bEndMotion = false;
}

//=============================================================================
// �U���ς݃��X�g�̏�����
//=============================================================================
void CObjectMotion::InitObjAttacked(void) {
	if (m_pListAttacked != nullptr) m_pListAttacked->Init();
}

//=============================================================================
// �U��
//=============================================================================
void CObjectMotion::Attack(int nObjtype, D3DXVECTOR3 posAttack, float fRadiusAttack, int nDamage, CObject::DAMAGE_TYPE typeDamage, int* pNumKill) {
	CObject* pObject = GetObjectTopAll();	//�S�I�u�W�F�N�g�̃��X�g�̐擪���擾

	while (pObject != nullptr) {
		CObject* pObjNext = GetObjectNextAll(pObject);	//���X�g�̎��̃I�u�W�F�N�g�̃|�C���^���擾

		//�I�u�W�F�N�g�^�C�v�̊m�F
		bool bMatchType = false;
		if (pObject->GetObjType() & nObjtype) bMatchType = true;

		//���łɍU�����������Ă���ꍇ
		bool bAttacked = false;	//���łɍU������Ă��邩�ǂ���
		if (m_pListAttacked != nullptr) {
			//���X�g�ɂ��łɒǉ�����Ă���ꍇ
			bAttacked = m_pListAttacked->MatchObject(pObject);
		}

		//���S��Ԃ̎擾
		bool bDeath = pObject->GetDeath();

		//�����蔻��\��Ԃ̎擾
		bool bEnableCollision = pObject->GetEnableCollision();

		//�U���\�Ȕ͈͂ɂ��邩�ǂ���
		D3DXVECTOR2 vecObj = D3DXVECTOR2(pObject->GetPos().x - posAttack.x, pObject->GetPos().z - posAttack.z);	//�U���ʒu�ƃI�u�W�F�N�g�̃x�N�g��
		float fDistObj = D3DXVec2Length(&vecObj);	//�U���ʒu�ƃI�u�W�F�N�g�̋���

		//���̃��[�v�ɔ�΂�
		if (!bMatchType || bAttacked || bDeath || !bEnableCollision || fDistObj > MAX_ATTACK_DISTANCE) {
			pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
			continue;	
		}

		//---------------------------
		//�����蔻��
		//---------------------------
		bool bCollision = false;	//�����蔻��̃t���O
		D3DXVECTOR3 posHit;	//���������ʒu

		//�����蔻�肪����I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X���X�V
		pObject->UpdateMtxWorldAll();	//�����蔻��擾���Ɉʒu�̃Y���������邽��

		//�����蔻�肪����p�[�c�̐��̎擾
		int nNumColParts = pObject->GetNumCollisionParts();

		//�p�[�c���Ƃɓ����蔻��
		for (int nCntColParts = 0; nCntColParts < nNumColParts; nCntColParts++)
		{
			int nNumCollision = 0;		//�����蔻��̐�
			D3DXVECTOR3* pPosColArray = nullptr;	//�����蔻��̈ʒu�̔z��̃|�C���^
			float fRadiusCol = 0.0f;	//�����蔻��̔��a

			//�����蔻��̏��̎擾
			pObject->GetCollisionInfo(nCntColParts, &nNumCollision, &pPosColArray, &fRadiusCol);

			//�ʒu���̃��������m�ۂ���Ă��Ȃ��ꍇ�I��
			if (pPosColArray == nullptr) continue;

			for (int nCntCol = 0; nCntCol < nNumCollision; nCntCol++)
			{
				//���Ƌ��̓����蔻��
				D3DXVECTOR3 vecCol = pPosColArray[nCntCol] - posAttack;	//�����蔻�肩�瓖���蔻��ւ̃x�N�g��
				float fDistCol = D3DXVec3Length(&vecCol);	//�����蔻�蓯�m�̋���
				//�����Ɠ����蔻�蓯�m�̋������r
				if (fDistCol < fRadiusAttack + fRadiusCol) {
					bCollision = true;
					posHit = pPosColArray[nCntCol];
					break;
				}
			}

			//�ʒu���̔j��
			delete[] pPosColArray;		

			//���łɓ������Ă����瓖���蔻��̃��[�v�I��
			if (bCollision) break;
		}

		//---------------------------
		//�����蔻�莞�̏���
		//---------------------------
		if (bCollision) {
			bool bDead = false;	//�U�����ꂽ�I�u�W�F�N�g�����S�������ǂ���
			//�I�u�W�F�N�g�Ƀ_���[�W��^����
			pObject->Damage(nDamage, typeDamage, &bDead);
			//�|�������̉��Z
			if (bDead && pNumKill != nullptr) (*pNumKill)++;

			//�I�u�W�F�N�g�����S���Ă��Ȃ��ꍇ�U���ς݃��X�g�ɒǉ�
			if (!pObject->GetDeath()) {
				if (m_pListAttacked != nullptr) m_pListAttacked->AppendNode(pObject);
			}
		}

		pObject = pObjNext;	//���X�g�̎��̃I�u�W�F�N�g����
	}
}

//=============================================================================
// �����蔻��̈ʒu���擾
//=============================================================================
void CObjectMotion::GetPosCollision(D3DXVECTOR3* const pPosCollision, D3DXVECTOR3 posOffset, int nIdxParts) {
	//�ʒu�̃|�C���^��NULL�̏ꍇ�I��
	if (pPosCollision == nullptr) return;

	//�����蔻�肪���郂�f�����擾
	CModel* pModel = GetPtrModel(nIdxParts);

	if (pModel != nullptr) {
		D3DXMATRIX mtxParent;	//�����蔻�肪���郂�f���̃��[���h�}�g���b�N�X
		//���f���̃��[���h�ϊ��s����擾
		mtxParent = pModel->GetMtxWorld();

		D3DXMATRIX mtxWorld;	//�����蔻��̃��[���h�}�g���b�N�X
		D3DXMATRIX mtxTrans;	//�v�Z�p�}�g���b�N�X

		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, posOffset.x, posOffset.y, posOffset.z);
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//�Z�o�������[���h�}�g���b�N�X�Ɛe�̃}�g���b�N�X���|�����킹��
		D3DXMatrixMultiply(&mtxWorld,
			&mtxWorld,
			&mtxParent);

		//�U���ʒu�̐ݒ�
		pPosCollision->x = mtxWorld._41;
		pPosCollision->y = mtxWorld._42;
		pPosCollision->z = mtxWorld._43;

	}
	else {
		//���[�V�����I�u�W�F�N�g�̈ʒu���瓖���蔻��̈ʒu���擾		
		*pPosCollision = m_pos + posOffset;
	}
}

//=============================================================================
// ���[�V�����̎�ނ̎擾
//=============================================================================
int CObjectMotion::GetMotionType(void) {
	return m_nTypeMotion;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̈ʒu���̐ݒ�
//=============================================================================
void CObjectMotion::SetPos(D3DXVECTOR3 pos) {
	m_pos = pos;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̈ʒu���W�̎擾
//=============================================================================
D3DXVECTOR3 CObjectMotion::GetPos(void) {
	return m_pos;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̈ʒu���W�̃|�C���^�̎擾
//=============================================================================
D3DXVECTOR3* CObjectMotion::GetPtrPos(void) {
	return &m_pos;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̊p�x�̐ݒ�
//=============================================================================
void CObjectMotion::SetRot(D3DXVECTOR3 rot) {
	m_rot = rot;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̊p�x�̎擾
//=============================================================================
D3DXVECTOR3 CObjectMotion::GetRot(void) {
	return m_rot;
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V
//=============================================================================
void CObjectMotion::UpdateMtxWorldAll(void) {
	LPDIRECT3DDEVICE9 pDevice = nullptr;	//�f�o�C�X�ւ̃|�C���^
	//�}�l�[�W���[�̎擾
	CManager* pManager = CManager::GetManager();	
	//�����_���[�̎擾
	CRenderer* pRenderer = nullptr;			
	if (pManager != nullptr) pRenderer = pManager->GetRenderer();
	//�f�o�C�X�̎擾
	if (pRenderer != nullptr) pDevice = pRenderer->GetDevice();
	//�f�o�C�X��null�̏ꍇ�I��
	if (pDevice == nullptr) return;

	//���f���̔z��NULL�̏ꍇ�I��
	if (m_ppModelArray == nullptr) return;

	//-----------------------------------
	//���[�V�����I�u�W�F�N�g�̃��[���h�}�g���b�N�X�̐ݒ�
	//-----------------------------------
	D3DXMATRIX mtxRot, mtxTrans;	//�}�g���b�N�X

	//���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	//�����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//�ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//���[���h�}�g���b�N�X���X�V
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_ppModelArray[nCnt] != nullptr)m_ppModelArray[nCnt]->UpdateMtxWorld();
	}
}

//=============================================================================
// ���[�V�����I�u�W�F�N�g�̑S���f���̗֊s�̐F�̎w��
//=============================================================================
void CObjectMotion::SetColorOutlineAll(D3DXCOLOR col) {
	if (m_ppModelArray == nullptr) return;
	//�֊s�̐F��ύX
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++)
	{
		if (m_ppModelArray[nCnt] != nullptr)m_ppModelArray[nCnt]->SetColorOutline(col);
	}
}

//=============================================================================
// ���[���h�}�g���b�N�X�̃|�C���^���擾
//=============================================================================
D3DXMATRIX* CObjectMotion::GetPtrMtxWorld(void) {
	return &m_mtxWorld;
}

//=============================================================================
// ���f���̃|�C���^���擾
//=============================================================================
CModel* CObjectMotion::GetPtrModel(int nIdxParts) {
	if (nIdxParts >= 0 && nIdxParts < m_nNumParts && m_ppModelArray != nullptr) {
		return m_ppModelArray[nIdxParts];
	}
	return nullptr;
}

//=============================================================================
// ���[�V�����̑J�ڔ���
//=============================================================================
bool CObjectMotion::GetTransMotion(void) {
	return m_bTransMotion;
}

//=============================================================================
// ���[�V�����̏I������
//=============================================================================
bool CObjectMotion::GetEndMotion(void) {
	return m_bEndMotion;
}

//=============================================================================
// ���݂̃��[�V�����̃L�[�̎擾
//=============================================================================
int CObjectMotion::GetCurKey(void) {
	return m_nKey;
}

//=============================================================================
// ���݂̃��[�V�����̃J�E���g�̎擾
//=============================================================================
int CObjectMotion::GetCurMotionCnt(void) {
	return m_nCntMotion;
}

//=============================================================================
// ���f���̃f�B�t���[�Y�F��ݒ肷��
//=============================================================================
void CObjectMotion::SetDiffuseModelAll(D3DXCOLOR col, int nIdx) {
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		//���f����null�̏ꍇ���[�v���΂�
		if (m_ppModelArray[nCnt] == nullptr) {
			continue;
		}
		//�F�̐ݒ�
		m_ppModelArray[nCnt]->SetMaterialDiffuse(col, nIdx);
	}
}

//=============================================================================
// ���f���̃X�y�L�����[�F��ݒ肷��
//=============================================================================
void CObjectMotion::SetSpecularModelAll(D3DXCOLOR col, int nIdx) {
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		//���f����null�̏ꍇ���[�v���΂�
		if (m_ppModelArray[nCnt] == nullptr) {
			continue;
		}
		//�F�̐ݒ�
		m_ppModelArray[nCnt]->SetMaterialSpecular(col, nIdx);
	}
}

//=============================================================================
// ���f���̃}�e���A���̐F�̕ύX�̊J�n
//=============================================================================
void CObjectMotion::StartChangeDiffuseAll(int nIdxMat, D3DXCOLOR colDest, int nTimeFin) {
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		//���f����null�̏ꍇ���[�v���΂�
		if (m_ppModelArray[nCnt] == nullptr) continue;
		
		//�ڕW�F�̐ݒ�
		m_ppModelArray[nCnt]->StartChangeMaterialDiffuse(nIdxMat, colDest, nTimeFin);
	}
}

//=============================================================================
// ���f���̗֊s�̔����F��ݒ肷��
//=============================================================================
void CObjectMotion::SetColorGlowAll(D3DXCOLOR col) {
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		//���f����null�̏ꍇ���[�v���΂�
		if (m_ppModelArray[nCnt] == nullptr) {
			continue;
		}
		//�F�̐ݒ�
		m_ppModelArray[nCnt]->SetColorGlow(col);
	}
}

//=============================================================================
// ���f���̐F���t�F�[�h������
//=============================================================================
bool CObjectMotion::FadeModelAll(float fDestAlpha, float fSpeedFade) {
	bool bEndFade = false;
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		//���f����null�̏ꍇ���[�v���΂�
		if (m_ppModelArray[nCnt] == nullptr) {
			continue;
		}

		//�����x�̎擾
		float fAlpha = m_ppModelArray[nCnt]->GetMaterialDiffuseAlpha(0);
		//�����x�̉��Z
		fAlpha += fSpeedFade;
		//���Z���铧���x�����̒l
		if (fSpeedFade >= 0.0f) {
			if (fAlpha >= fDestAlpha) {
				fAlpha = fDestAlpha;
				bEndFade = true;
			}
		}
		//���̒l
		else {
			if (fAlpha <= fDestAlpha) {
				fAlpha = fDestAlpha;
				bEndFade = true;
			}
		}
		m_ppModelArray[nCnt]->SetMaterialDiffuseAlphaAll(fAlpha);	
	}

	return bEndFade;
}

//=============================================================================
// ���f���̗֊s�̕\���ݒ�
//=============================================================================
void CObjectMotion::SetDrawOutlineAll(bool bDraw) {
	for (int nCnt = 0; nCnt < m_nNumParts; nCnt++) {
		if (m_ppModelArray[nCnt] != nullptr) {
			m_ppModelArray[nCnt]->SetDrawOutline(bDraw);
		}
	}
}