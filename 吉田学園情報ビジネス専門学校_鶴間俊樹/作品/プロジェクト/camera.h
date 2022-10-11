//=============================================================================
//
// �J�������� [camera.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// �J�����N���X
//*****************************************************************************
class CCamera
{
public:
	CCamera();		//�f�t�H���g�R���X�g���N�^
	CCamera(float fMaxDrawDist);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CCamera();	//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	void SetCamera(void);	//�J�����̃Z�b�g
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }		//�ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPos(void) { return m_pos; }			//�ʒu���W�̎擾
	void SetPosV(D3DXVECTOR3 posV) { m_posV = posV; }	//���_�̈ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPosV(void) { return m_posV; }		//���_�̈ʒu���W�̎擾
	void SetPosR(D3DXVECTOR3 posR) { m_posR = posR; }	//�����_�̈ʒu���W�̐ݒ�
	D3DXVECTOR3 GetPosR(void) { return m_posR; }		//�����_�̈ʒu���W�̎擾
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }		//�p�x�̐ݒ�
	void AddRot(D3DXVECTOR3 rot) { m_rot += rot; }		//�p�x�̒ǉ�
	D3DXVECTOR3 GetRot(void) { return m_rot; }			//�p�x�̎擾
	void SetDistance(float fDistance) { m_fDist = fDistance; }	//�����̐ݒ�
	float GetDistance(void) { return m_fDist; }			//�����̎擾
	void SetLockControll(bool bLock) { m_bLockControll = bLock; }	//����̐�����ݒ�

private:
	void Move(void);	//�ړ�
	void Rotate(void);	//��]

	const float m_fMaxDrawDist;	//�`��\�ȍő�̋���
	D3DXVECTOR3 m_pos;			//�J�����̈ʒu
	D3DXVECTOR3 m_rot;			//�J�����̌���
	D3DXVECTOR3 m_posV;			//���_
	D3DXVECTOR3 m_posR;			//�����_
	float m_fDist;				//���_�ƒ����_�̋���
	D3DXVECTOR3 m_vecU;			//������x�N�g��
	D3DXMATRIX m_mtxProjection;	//�v���W�F�N�V�����}�g���b�N�X
	D3DXMATRIX m_mtxView;		//�r���[�}�g���b�N�X
	D3DVIEWPORT9 m_viewport;	//�r���[�|�[�g

	bool m_bLockControll;	//����̐���
};

#endif // !_CAMERA_H_