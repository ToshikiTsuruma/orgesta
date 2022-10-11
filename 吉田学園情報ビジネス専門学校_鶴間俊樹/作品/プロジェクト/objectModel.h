//=============================================================================
//
// ���f���I�u�W�F�N�g���� [objectModel.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_MODEL_H_
#define _OBJECT_MODEL_H_

#include "main.h"
#include "object.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// ���f���I�u�W�F�N�g�N���X
//*****************************************************************************
class CObjectModel : public CObject
{
public:

	CObjectModel();		//�f�t�H���g�R���X�g���N�^
	CObjectModel(CModel::MODELTYPE typeModel, bool bOutLine);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObjectModel();		//�f�X�g���N�^
	static CObjectModel* Create(CModel::MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, bool bOutLine);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��

	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//�ʒu���W�̎擾
	void SetMove(D3DXVECTOR3 move) { m_move = move; }	//�ړ����x�̐ݒ�
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		//�p�x�̐ݒ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }			//�p�x�̎擾
	void SetRotSpeed(D3DXVECTOR3 rotSpeed) { m_rotSpeed = rotSpeed; }	//�ړ����x�̐ݒ�

	CModel::MODELTYPE GetModelType(void);	//���f���̎�ނ̎擾
	CModel* GetPtrModel(void) { return m_pModel; }		//���f���̃|�C���^�̎擾
	void SetModelColor(D3DXCOLOR col, int nIdx);		//���f���̐F�̐ݒ�
	void SetModelGlowColor(D3DXCOLOR col);	//���f���̔����F�̐ݒ�

private:
	void Move(void);	//�ړ�
	void Rotate(void);	//��]

	CModel* m_pModel;	//���f���ւ̃|�C���^
	D3DXVECTOR3 m_pos;	//�ʒu
	D3DXVECTOR3 m_move;	//�ړ����x
	D3DXVECTOR3 m_rot;	//�p�x
	D3DXVECTOR3 m_rotSpeed;	//��]���x
	D3DXMATRIX m_mtxWorld;//���[���h�}�g���b�N�X
};

#endif // !_OBJECT_MODEL_H_