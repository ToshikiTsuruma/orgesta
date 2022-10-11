//=============================================================================
//
// �X�e�[�W���� [stage.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "model.h"
#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define TEXT_FILE_NAME_STAGE_GAME "data/STAGE/stage001.txt"

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjectList;
class CTerrain;

//*****************************************************************************
// �X�e�[�W�N���X
//*****************************************************************************
class CStage
{
public:
	CStage();	//�f�t�H���g�R���X�g���N�^
	~CStage();	//�f�X�g���N�^
	void Init(void);	//����������
	void CreateStage(char* strFileName);	//�t�@�C����ǂݍ���ŃX�e�[�W�𐶐�
	void DestroyStage(void);	//���݂̃X�e�[�W��j��
	void AppendObject(CObject* pObject);	//�I�u�W�F�N�g�̒ǉ�
	CObjectList* GetObjList(void);	//�I�u�W�F�N�g���X�g�̎擾

private:
	//�n�`�̐������̏��
	struct TerrainData {
		CModel::MODELTYPE modelType;	//�n�`�̃��f���̎��
		D3DXVECTOR3 pos;		//�ʒu
		D3DXVECTOR3 rot;		//�p�x
	};

	//�I�u�W�F�N�g�̐������̏��
	struct ObjectData {
		char sObjName[MAX_OBJECT_NAME];				//�I�u�W�F�N�g�̖��O
		CModel::MODELTYPE modelType;	//�I�u�W�F�N�g�̃��f���̎��
		D3DXVECTOR3 pos;		//�ʒu
		D3DXVECTOR3 rot;		//�p�x
	};

	void LoadTerrainData(const char* pLoadText, TerrainData& terrainData);	//�n�`�̃f�[�^�����[�h
	void LoadObjData(const char* pLoadText, ObjectData& objData);			//�I�u�W�F�N�g�̃f�[�^�����[�h
	CTerrain* CreateLoadTerrain(TerrainData& terrainData);	//���[�h�����n�`�𐶐�
	CObject* CreateLoadObject(ObjectData& objData);	//���[�h�����I�u�W�F�N�g�𐶐�

	//�c�[���̂�

	CTerrain* m_pStageTerrain;	//�X�e�[�W�̒n�`
	CObjectList* m_pStageObjectList;	//�X�e�[�W�̃I�u�W�F�N�g�̃��X�g
};

#endif // !_STAGE_H_