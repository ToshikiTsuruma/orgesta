//=============================================================================
//
// ���[�V�����I�u�W�F�N�g���� [objectMotion.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_MOTION_H_
#define _OBJECT_MOTION_H_

#include "main.h"
#include "object.h"
#include "model.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_KEY_MOTION (12)	//���[�V�������Ƃ̍ő�L�[��
//#define MAX_ATTACK_OBJECT (24)	//��x�ɍU���ł���ő�̃I�u�W�F�N�g��
#define DEAD_HEIGHT (-5000.0f)	//���S���鍂��
#define POWER_GRAVITY (0.6f)			//�d��
#define POWER_GRAVITY_GROUND (15.0f)	//�n��ɂ��鎞�̏d��

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObjectList;

//*****************************************************************************
// ���[�V�����I�u�W�F�N�g�N���X
//*****************************************************************************
class CObjectMotion : public CObject
{
public:
	//�p�[�c���̍\����
	typedef struct {
		CModel::MODELTYPE modelType;	//�p�[�c�̃��f���̃^�C�v
		int nIdx;			//�p�[�c�̃C���f�b�N�X
		int nIdxParent;		//�e�p�[�c�̃C���f�b�N�X
		D3DXVECTOR3 offset;	//�I�t�Z�b�g
	} PARTS_INFO;

	//�L�[�̍\����
	typedef struct
	{
		D3DXVECTOR3 pos;	//�ʒu
		D3DXVECTOR3 rot;	//�p�x
	} KEY;
	//�L�[���̍\����
	typedef struct {
		int nFrame;	//�Đ��t���[��
		KEY* pKeyArray;	//�e�p�[�c�̃L�[�v�f�̓��I�z��̐擪�|�C���^
	} KEY_INFO;
	//���[�V�������̍\����
	typedef struct {
		bool bLoop;		//���[�v���邩�ǂ���
		int nNumKey;	//�L�[�̑���
		KEY_INFO aKeyInfo[MAX_KEY_MOTION];	//�L�[���
	} MOTION_INFO;

	CObjectMotion();		//�f�t�H���g�R���X�g���N�^
	CObjectMotion(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CObjectMotion();	//�f�X�g���N�^
	static void LoadMotionText(char* pLoadDirectory, PARTS_INFO*& pPartsInfoArray, int& nNumParts, MOTION_INFO* pMotionInfoArray, int nNumMotionType);	//���[�V�����������[�h
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void);	//�`�揈��
	void SetMotion(int nType);	//���[�V�����̐ݒ�
	int GetMotionType(void);	//���[�V�����̎�ނ̎擾
	virtual void SetPos(D3DXVECTOR3 pos);	//�ʒu���W�̐ݒ�
	virtual D3DXVECTOR3 GetPos(void);		//�ʒu���W�̎擾
	virtual D3DXVECTOR3* GetPtrPos(void);	//�ʒu���W�̃|�C���^�̎擾
	virtual void SetRot(D3DXVECTOR3 rot);	//�p�x�̐ݒ�
	virtual D3DXVECTOR3 GetRot(void);		//�p�x�̎擾
	virtual void SetMove(D3DXVECTOR3 move) {}	//�ړ��ʂ̐ݒ�
	void UpdateMtxWorldAll(void);			//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V
	void SetColorOutlineAll(D3DXCOLOR col);	//�I�u�W�F�N�g�̑S���f���̗֊s�̐F�̎w��

	void InitObjAttacked(void);	//�U���ς݃��X�g�̏�����
	void Attack(int nObjtype, D3DXVECTOR3 pos, float fRadius, int nDamage, CObject::DAMAGE_TYPE typeDamage, int* pNumKill);	//�U��
	void GetPosCollision(D3DXVECTOR3* const pPosCollision, D3DXVECTOR3 posOffset, int nIdxParts);	//�����蔻��̈ʒu���擾
	D3DXMATRIX* GetPtrMtxWorld(void);	//���[���h�}�g���b�N�X�̃|�C���^���擾
	CModel* GetPtrModel(int nIdxParts);	//���f���̃|�C���^���擾
	bool GetTransMotion(void);	//���[�V�����̑J�ڔ���
	bool GetEndMotion(void);	//���[�V�����̏I������
	int GetCurKey(void);		//���݂̃��[�V�����̃L�[�̎擾
	int GetCurMotionCnt(void);	//���݂̃��[�V�����̃J�E���g�̎擾
	void SetDiffuseModelAll(D3DXCOLOR col, int nIdx);	//���f���̃f�B�t���[�Y�F��ݒ肷��
	void SetSpecularModelAll(D3DXCOLOR col, int nIdx);	//���f���̃X�y�L�����[�F��ݒ肷��
	void StartChangeDiffuseAll(int nIdxMat, D3DXCOLOR colDest, int nTimeFin);	//���f���̃}�e���A���̐F�̕ύX�̊J�n
	void SetColorGlowAll(D3DXCOLOR col);	//���f���̗֊s�̔����F��ݒ肷��
	bool FadeModelAll(float fDestAlpha, float fSpeedFade);	//���f���̐F���t�F�[�h������
	void SetDrawOutlineAll(bool bDraw);	//���f���̗֊s�̕\���ݒ�

private:
	//���[�V�����X�V�Ɏg�����
	typedef struct {
		D3DXVECTOR3 deltaRot;	//���[�V�����̊p�x�̍���
		D3DXVECTOR3 deltaPos;	//���[�V�����̈ʒu�̍���
		D3DXVECTOR3 curRot;		//���݁i�L�[�J�ڎ��j�̊p�x
		D3DXVECTOR3 curPos;		//���݁i�L�[�J�ڎ��j�̈ʒu
		D3DXVECTOR3 offsetPos;		//�I�u�W�F�N�g����e�p�[�c�ւ̃I�t�Z�b�g
	} UPDATE_MOTION_INFO;

	const int m_nNumParts;		//���f���̃p�[�c�̐�
	CModel** m_ppModelArray;	//"���f���̃|�C���^"�̓��I�z��̐擪�|�C���^
	UPDATE_MOTION_INFO* m_pUpdateMotionInfoArray;	//���[�V�����X�V���̔z��̃|�C���^

	const int m_nNumTypeMotion;			//���[�V�����̃^�C�v��
	MOTION_INFO* m_pMotionInfoArray;	//���[�V�������̓��I�z��̃|�C���^

	D3DXVECTOR3	m_pos;		//�ʒu���W
	D3DXVECTOR3 m_rot;		//�p�x
	D3DXMATRIX m_mtxWorld;	//���[���h�}�g���b�N�X
	bool m_bEndMotion;		//���[�V�������I�����Ă����Ԃ��ǂ���
	CObjectList* m_pListAttacked;	//�U���ς݂̃I�u�W�F�N�g�̃��X�g

	int m_nTypeMotion;		//���[�V�����̃^�C�v
	bool m_bLoopMotion;		//���[�v���邩�ǂ���
	int m_nKey;				//�L�[No.
	int m_nCntMotion;		//���[�V�����J�E���^�[
	bool m_bTransMotion;	//���[�V�����̕��
	bool m_bSetCurMotion;	//���݁i�L�[�J�ڎ��j��ݒ肵�����ǂ���
};

#endif // !_OBJECT_MOTION_H_