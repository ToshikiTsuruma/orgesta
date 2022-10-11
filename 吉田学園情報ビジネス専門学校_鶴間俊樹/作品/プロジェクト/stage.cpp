//=============================================================================
//
// �X�e�[�W���� [stage.cpp]
// Author : �ߊԏr��
//
//=============================================================================
#include "stage.h"
#include "objectList.h"
#include "terrain.h"
#include "objectModel.h"

//=============================================================================
// �}�N����`
//=============================================================================
#define MAX_LOAD_TEXT (128)		//��x�Ƀt�@�C������ǂݍ��ރe�L�X�g�̃o�C�g��
#define GLOW_COLOR_STAGE_OBJECT (D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f))

//=============================================================================
// �f�t�H���g�R���X�g���N�^
//=============================================================================
CStage::CStage()
{
	//�I�u�W�F�N�g���X�g�̐���
	m_pStageObjectList = new CObjectList;
	m_pStageTerrain = nullptr;
}

//=============================================================================
// �f�X�g���N�^
//=============================================================================
CStage::~CStage()
{
	if (m_pStageObjectList != nullptr) {
		//�X�e�[�W�̃I�u�W�F�N�g�����ׂĔj������
		m_pStageObjectList->DeleteAllObject();
		//�I�u�W�F�N�g���X�g�̔j��
		delete m_pStageObjectList;
	}
	//�n�`�̔j��
	if (m_pStageTerrain != nullptr) m_pStageTerrain->Uninit();
}

//=============================================================================
// ����������
//=============================================================================
void CStage::Init(void) {

}

//=============================================================================
// �t�@�C����ǂݍ���ŃX�e�[�W�𐶐�
//=============================================================================
void CStage::CreateStage(char* strFileName) {
	//�����O�Ɍ��݂̃X�e�[�W��j��
	DestroyStage();

	//�ǂݍ��ރt�@�C�������Ȃ��ꍇ�I��
	if (strFileName == nullptr) return;

	FILE* pFile = nullptr;	//�t�@�C���ւ̃|�C���^

	//�t�@�C�����J��
	pFile = fopen(strFileName, "r");

	//�t�@�C����null�̏ꍇ�I��
	if (pFile == nullptr) return;

	char sLoadText[MAX_LOAD_TEXT];		//��s���ǂݍ��ރt�@�C���̃e�L�X�g
	char *pSplitText;	//�t�@�C���̃e�L�X�g�𕪊�����������

	//------------------------------
	//�ǂݍ��݊J�n�ʒu�܂ōs���΂�
	//------------------------------
	while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //��s���Ƃɕ�������擾
	{
		pSplitText = strtok(sLoadText, " =\t\n");	//������̕����i�� �^�u ���s ���j
		//���������e�L�X�g�̃|�C���^��null�̏ꍇ���[�v���΂�
		if (pSplitText == nullptr) continue;

		//�ǂݍ��݊J�n
		if (strcmp(pSplitText, "SCRIPT") == 0) {
			break;
		}
	}

	//------------------------------
	//�X�e�[�W�̐���
	//------------------------------
	bool bSetTerrain = false;	//�n�`�ݒ蒆���ǂ���
	bool bSetObject = false;	//�I�u�W�F�N�g�ݒ蒆���ǂ���
	TerrainData createTerrainData;	//��������n�`�̃f�[�^
	ObjectData createObjData;		//��������I�u�W�F�N�g�̃f�[�^

	while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //��s���Ƃɕ�������擾
	{
		pSplitText = strtok(sLoadText, " =\t\n");	//������̕����i�� �^�u ���s ���j
		//���������e�L�X�g�̃|�C���^��null�̏ꍇ���[�v���΂�
		if (pSplitText == nullptr) continue;

		//�R�����g
		if (strstr(pSplitText, "//") != nullptr) {
			continue;
		}
		//�ǂݍ��ݏI��
		if (strcmp(pSplitText, "END_SCRIPT") == 0) {
			break;
		}

		//------------------------------
		//�n�`�̐���
		//------------------------------
		//�n�`�̓ǂݍ��݊J�n
		if (strcmp(pSplitText, "SET_TERRAIN") == 0 && !bSetObject) {
			bSetTerrain = true;

			//��������n�`�̃f�[�^�̏�����
			createTerrainData.modelType = (CModel::MODELTYPE) - 1;
			createTerrainData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			createTerrainData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (bSetTerrain) {
			//�n�`�̓ǂݍ��ݏI��
			if (strcmp(pSplitText, "END_SET_TERRAIN") == 0) {
				bSetTerrain = false;

				m_pStageTerrain = CreateLoadTerrain(createTerrainData);
			}

			//�n�`�̃f�[�^��ǂݍ���
			LoadTerrainData(pSplitText, createTerrainData);
		}

		//------------------------------
		//�I�u�W�F�N�g�̐���
		//------------------------------
		//�I�u�W�F�N�g�̓ǂݍ��݊J�n
		if (strcmp(pSplitText, "SET_OBJECT") == 0 && !bSetTerrain) {
			bSetObject = true;

			//��������I�u�W�F�N�g�̃f�[�^�̏�����
			strcpy(createObjData.sObjName, "[_NO_NAME_]");
			createObjData.modelType = (CModel::MODELTYPE) - 1;
			createObjData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			createObjData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (bSetObject) {
			//�I�u�W�F�N�g�̓ǂݍ��ݏI��
			if (strcmp(pSplitText, "END_SET_OBJECT") == 0) {
				bSetObject = false;

				//�I�u�W�F�N�g�𐶐�����
				CObject* pObjectCreate = CreateLoadObject(createObjData);

				//�����ł��Ă��Ȃ��ꍇ���[�v���΂�
				if (pObjectCreate == nullptr) continue;

				//�I�u�W�F�N�g�̖��O��ݒ�
				//pObjectCreate->SetObjName(createObjData.sObjName);
				//�I�u�W�F�N�g���X�g�ɒǉ�
				AppendObject(pObjectCreate);
			}

			//�I�u�W�F�N�g�̃f�[�^��ǂݍ���
			LoadObjData(pSplitText, createObjData);
		}
	}
}

//=============================================================================
// ���݂̃X�e�[�W��j��
//=============================================================================
void CStage::DestroyStage(void) {
	//�n�`�̔j��
	if (m_pStageTerrain != nullptr) {
		m_pStageTerrain->Uninit();
		m_pStageTerrain = nullptr;
	}

	//���X�g�̃I�u�W�F�N�g�����ׂĔj��
	if (m_pStageObjectList != nullptr) {
		m_pStageObjectList->DeleteAllObject();
	}
}

//=============================================================================
// �I�u�W�F�N�g�̒ǉ�
//=============================================================================
void CStage::AppendObject(CObject* pObject) {
	//�I�u�W�F�N�g���X�g��null�̏ꍇ�I��
	if (m_pStageObjectList == nullptr) return;

	//���X�g�ɃI�u�W�F�N�g��ǉ�
	m_pStageObjectList->AppendNode(pObject);
}

//=============================================================================
// �n�`�̃f�[�^��ǂݍ���
//=============================================================================
void CStage::LoadTerrainData(const char* pLoadText, TerrainData& terrainData) {
	//�ǂݍ��񂾃e�L�X�g��null�̏ꍇ�I��
	if (pLoadText == nullptr) return;

	//���f���̎�ނ̐ݒ�
	if (strcmp(pLoadText, "MODEL") == 0) {
		pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;

		for (int nCntModel = 0; nCntModel < (int)CModel::MODELTYPE::ENUM_MAX; nCntModel++)
		{
			//���f���̃p�X�ƕ�������r����
			if (strcmp(pLoadText, CModel::GetPathName((CModel::MODELTYPE)nCntModel)) == 0) {
				//���f���̎�ނ̔ԍ���ݒ�
				terrainData.modelType = (CModel::MODELTYPE)nCntModel;
				break;
			}
		}
	}
	//�ʒu�̐ݒ�
	else if (strcmp(pLoadText, "POS") == 0) {
		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		//X���W�̐ݒ�
		terrainData.pos.x = (float)atof(pLoadText);	

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		terrainData.pos.y = (float)atof(pLoadText);	//Y���W�̐ݒ�

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		terrainData.pos.z = (float)atof(pLoadText);	//Z���W�̐ݒ�
	}
	//�p�x�̐ݒ�
	else if (strcmp(pLoadText, "ROT") == 0) {
		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		//X���W�̐ݒ�
		terrainData.rot.x = (float)atof(pLoadText);	

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		terrainData.rot.y = (float)atof(pLoadText);	//Y���W�̐ݒ�

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		terrainData.rot.z = (float)atof(pLoadText);	//Z���W�̐ݒ�
	}
}

//=============================================================================
// �I�u�W�F�N�g�̃f�[�^��ǂݍ���
//=============================================================================
void CStage::LoadObjData(const char* pLoadText, ObjectData& objData) {
	//�ǂݍ��񂾃e�L�X�g��null�̏ꍇ�I��
	if (pLoadText == nullptr) return;

	//�I�u�W�F�N�g���̐ݒ�
	if (strcmp(pLoadText, "NAME") == 0) {
		pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr || strlen(pLoadText) >= MAX_OBJECT_NAME) return;

		//������̃R�s�[
		strcpy(objData.sObjName, pLoadText);
	}
	//���f���̎�ނ̐ݒ�
	else if (strcmp(pLoadText, "MODEL") == 0) {
		pLoadText = strtok(nullptr, " =#\t\n");	//������̕���
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;

		for (int nCntModel = 0; nCntModel < (int)CModel::MODELTYPE::ENUM_MAX; nCntModel++)
		{
			//���f���̃p�X�ƕ�������r����
			if (strcmp(pLoadText, CModel::GetPathName((CModel::MODELTYPE)nCntModel)) == 0) {
				//���f���̎�ނ̔ԍ���ݒ�
				objData.modelType = (CModel::MODELTYPE)nCntModel;
				break;
			}
		}
	}
	//�ʒu�̐ݒ�
	else if (strcmp(pLoadText, "POS") == 0) {
		//������̕���
		pLoadText = strtok(nullptr, " =");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		//X���W�̐ݒ�
		objData.pos.x = (float)atof(pLoadText);

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		objData.pos.y = (float)atof(pLoadText);	//Y���W�̐ݒ�

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		objData.pos.z = (float)atof(pLoadText);	//Z���W�̐ݒ�
	}
	//�p�x�̐ݒ�
	else if (strcmp(pLoadText, "ROT") == 0) {
		//������̕���
		pLoadText = strtok(nullptr, " =");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		//X���W�̐ݒ�
		objData.rot.x = (float)atof(pLoadText);

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		objData.rot.y = (float)atof(pLoadText);	//Y���W�̐ݒ�

		//������̕���
		pLoadText = strtok(nullptr, " ");
		//������null�̏ꍇ�I��
		if (pLoadText == nullptr) return;
		objData.rot.z = (float)atof(pLoadText);	//Z���W�̐ݒ�
	}
}

//=============================================================================
// ���[�h�����n�`�𐶐�
//=============================================================================
CTerrain* CStage::CreateLoadTerrain(TerrainData& terrainData) {
	//�������郂�f���̎�ނ����݂���ꍇ
	if ((int)terrainData.modelType < 0 || (int)terrainData.modelType >= (int)CModel::MODELTYPE::ENUM_MAX) return nullptr;
	//�n�`�𐶐�����
	CTerrain* pTerrain = CTerrain::Create(terrainData.modelType, terrainData.pos, terrainData.rot);
	if (pTerrain == nullptr) return nullptr;

	CModel* pModel = pTerrain->GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(GLOW_COLOR_STAGE_OBJECT);
	}

	return pTerrain;
}

//=============================================================================
// ���[�h�����I�u�W�F�N�g�𐶐�
//=============================================================================
CObject* CStage::CreateLoadObject(ObjectData& objData) {
	//�������郂�f���̎�ނ����݂��Ȃ��ꍇ���[�v���΂�
	if ((int)objData.modelType < 0 || (int)objData.modelType >= (int)CModel::MODELTYPE::ENUM_MAX) return nullptr;

	//���f���I�u�W�F�N�g�𐶐�����
	CObjectModel* pCreateObj = CObjectModel::Create(objData.modelType, objData.pos, objData.rot, false);
	if (pCreateObj == nullptr) return nullptr;

	CModel* pModel = pCreateObj->GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(GLOW_COLOR_STAGE_OBJECT);
	}

	return pCreateObj;
}

//=============================================================================
// �I�u�W�F�N�g���X�g�̎擾
//=============================================================================
CObjectList* CStage::GetObjList(void) {
	return m_pStageObjectList;
}