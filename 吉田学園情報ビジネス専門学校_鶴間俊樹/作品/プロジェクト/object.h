//=============================================================================
//
// �I�u�W�F�N�g���� [object.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
//#define MAX_OBJECT (1024)
#define MAX_OBJECT_NAME (32)
//*****************************************************************************
//�O���錾
//*****************************************************************************

//*****************************************************************************
// �I�u�W�F�N�g�N���X
//*****************************************************************************
class CObject
{
public:
	//�I�u�W�F�N�g�̎�� (�����蔻���S�����ŕK�v�ȏꍇ�̂���)
	typedef enum {
		OBJTYPE_NONE		= 0b00000000,	//����
		OBJTYPE_TERRAIN		= 0b00000001,	//�n�`
		OBJTYPE_WALL		= 0b00000010,	//��	
		OBJTYPE_PLAYER		= 0b00000100,	//�v���C���[
		OBJTYPE_ENEMY		= 0b00001000,	//�G
		OBJTYPE_ITEM		= 0b00010000,	//�A�C�e��
		OBJTYPE_APPLETREE	= 0b00100000,	//�ь�̖�
		OBJTYPE_APPLE		= 0b01000000,	//�ь�
		OBJTYPE_SCAPEGOAT	= 0b10000000,	//����
		OBJTYPE_SEEDLING   = 0b100000000,	//�c��
	} OBJTYPE;

	//�I�u�W�F�N�g�̃t���O
	typedef enum {
		OBJFLAG_NONE = 0b00000000,	//����
		OBJFLAG_ENABLECOLLISION = 0b00000001,	//�����蔻�肪�L���ł���
	} OBJFLAG;

	//�X�V���i�X�V��~�ł��K�v�ȏꍇ�͒�`�j
	enum class UPDATE_PRIORITY {
		DEFAULT = 0,	//�X�V�����e�����Ȃ�����
		PLAYER,			//�v���C���[
		ENEMY,			//�G
		ENUM_MAX
	};	

	enum class DRAW_PRIORITY {
		NONE = 0,	
		BG,			//�w�i
		DEFAULT,	//�f�t�H���g
		CHARA,		//�v���C���[���̃L�����N�^�[�Ɋ֘A�������
		CLEAR,		//���߂���I�u�W�F�N�g
		EFFECT,		//�G�t�F�N�g
		BILLBOARD_UI,//UI�̃r���{�[�h
		UI_BG,		//UI�̔w�i
		UI,			//UI
		FRONT,		//�őO
		ENUM_MAX
	};	//�`�揇

	//�_���[�W�̎��
	enum class DAMAGE_TYPE {
		NONE = 0,
		PLAYER_PUNCH,	//�v���C���[�̃p���`
		ENEMY_PUNCH,	//�G�̍U��
		BULLET,			//�e
		SHOCKWAVE,		//�Ռ��g
		THUNDERBOLT,	//����
		FIRE,			//��
		EXPLOSION,		//����
		ENUM_MAX
	};

	CObject();			//�f�t�H���g�R���X�g���N�^
	virtual ~CObject();	//�f�X�g���N�^
	virtual HRESULT Init(void) = 0;	//����������
	virtual void Uninit(void) = 0;	//�I������
	virtual void Update(void) = 0;	//�X�V����
	virtual void Draw(void) = 0;	//�`�揈��

	static void ReleasePtrAll(CObject* pReleaseObj);	//���ׂẴI�u�W�F�N�g��ReleasePtr���s��
	virtual void ReleasePtr(CObject* pReleaseObj) {}	//���̃I�u�W�F�N�g���ێ����Ă���A�j�������\��̃I�u�W�F�N�g�̃|�C���^��null�ɂ���

	virtual void SetPos(D3DXVECTOR3 pos) {}	//�ʒu���W�̐ݒ�
	virtual D3DXVECTOR3 GetPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//�ʒu���W�̎擾
	virtual D3DXVECTOR3 GetLastPos(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }	//�Ō�̈ʒu���W�̎擾
	virtual D3DXVECTOR3 GetRot(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//�p�x�̎擾
	virtual void SetMove(D3DXVECTOR3 move) {}	//�ړ��ʂ̐ݒ�
	virtual D3DXVECTOR3 GetMove(void) { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }		//�ړ��ʂ̎擾
	virtual float GetRadius(void) { return 0.0f; }		//�����蔻��̔��a�̎擾
						
	virtual int GetNumCollisionParts(void) { return 1; }	//�����蔻�肪����p�[�c�̐��̎擾(��{�͂P�Ƃ���)
	virtual void GetCollisionInfo(int nIdxColParts, int* const pNumCol, D3DXVECTOR3** const ppPosCol, float* const pRadiusCol) {}	//�����蔻��̏��̎擾
	virtual void UpdateMtxWorldAll(void) {}			//�I�u�W�F�N�g�̑S���f���̃��[���h�}�g���b�N�X�̍X�V
	virtual void Heal(int nHeal) {}	//��
	virtual void Damage(int nDamage, DAMAGE_TYPE typeDamage, bool* pDead) {}	//�_���[�W
	virtual void Dead(void) {}						//���S����
	virtual bool GetItem(int nTypeItem) { return false; }	//�A�C�e���擾���̏���
	virtual void SetRatioWidth(float fRatio) {}		//���̊����̐ݒ�
	virtual void SetRatioHeight(float fRatio) {}	//�����̊����̐ݒ�
	virtual void SetColor(D3DXCOLOR col) {};	//�F�̐ݒ�

	static void ReleaseAll(void);	//�S�I�u�W�F�N�g�̉������
	static void UpdateAll(void);	//�S�I�u�W�F�N�g�̍X�V����
	static void DrawAll(void);		//�S�I�u�W�F�N�g�̕`�揈��
	static CObject* GetObjectTopAll(void);	//�S�I�u�W�F�N�g�̃��X�g��Top���擾
	static CObject* GetObjectNextAll(CObject* pObject);	//�S�I�u�W�F�N�g�̃��X�g��Next���擾
	static void ReleaseObjtype(int nObjtype);	//�w�肵���^�C�v�̃I�u�W�F�N�g�̉������
	static void DeadObjtype(int nObjtype);		//�w�肵���^�C�v�̎��S����

	static void SetUpdatePauseLevel(int nLevel) { m_nUpdatePauseLevel = nLevel; }	//�X�V�\�ȃ|�[�Y���x���ݒ�
	static int AddUpdatePauseLevel(void) { return ++m_nUpdatePauseLevel; }			//�X�V�\�ȃ|�[�Y���x�����Z
	static int SubUpdatePauseLevel(void) { return --m_nUpdatePauseLevel; }			//�X�V�\�ȃ|�[�Y���x�����Z
	void SetPauseLevel(int nLevel) { m_nPauseLevel = nLevel; }	//�|�[�Y���x���̐ݒ�
	int GetPauseLevel(void) { return m_nPauseLevel; }			//�|�[�Y���x���̎擾

	static CObject* GetNearObject(const D3DXVECTOR3 pos, int nObjType, int nObjFlag, D3DXVECTOR3* pPosNearObj, float* pfDistNearObj, D3DXVECTOR3* pVecNearObj);	//����ʒu����I�u�W�F�N�g�ւ̍ł��߂����������߂�

	bool GetDeath(void);	//���S�t���O�̎擾
	void SetObjType(OBJTYPE objType);	//�I�u�W�F�N�g�̎�ނ̐ݒ�
	OBJTYPE GetObjType(void);			//�I�u�W�F�N�g�̎�ނ̎擾
	void SetUpdatePriority(UPDATE_PRIORITY priority);	//�X�V�̗D�揇�ʂ̐ݒ�
	void SetDrawPriority(DRAW_PRIORITY priority);		//�`��̗D�揇�ʂ̐ݒ�

	void SetTexType(CTexture::TEXTURE_TYPE type);	//�e�N�X�`���̎�ނ̐ݒ�
	CTexture::TEXTURE_TYPE GetTexType(void);		//�e�N�X�`���̎�ނ̎擾
	void SetEnableCollision(bool bEnable);	//�����蔻��̗L����Ԃ�ݒ�
	bool GetEnableCollision(void);			//�����蔻��̗L����Ԃ��擾
	void SetDraw(bool bDraw) { m_bDraw = bDraw; }	//�`���Ԃ̐ݒ�
	bool GetDraw(void) { return m_bDraw; }			//�`���Ԃ̎擾

protected:
	void Release(void);	//�I�u�W�F�N�g�̉������

private:
	bool m_bDeath;		//���S�t���O
	OBJTYPE m_objType;	//�I�u�W�F�N�g�̎��
	UPDATE_PRIORITY m_updatePriority;	//�X�V�̗D�揇��
	DRAW_PRIORITY m_drawPriority;		//�`��̗D�揇��

	CTexture::TEXTURE_TYPE m_texType;	//�e�N�X�`���̃^�C�v
	bool m_bEnableCollision;	//�����蔻�肪�L��
	bool m_bDraw;	//�`�悷�邩�ǂ���

	static int m_nUpdatePauseLevel;	//�X�V�\�ȃ|�[�Y���x��
	int m_nPauseLevel;	//�|�[�Y�̃��x��

	//�S�I�u�W�F�N�g�̃��X�g
	static CObject* m_pTopAll;	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_pCurAll;	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevAll;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextAll;		//���̃I�u�W�F�N�g�ւ̃|�C���^
	
	//�X�V���̃��X�g
	static CObject* m_apTopUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_apCurUpdate[(int)UPDATE_PRIORITY::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevUpdate;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextUpdate;		//���̃I�u�W�F�N�g�ւ̃|�C���^

	//�`�揇�̃��X�g
	static CObject* m_apTopDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//�擪�̃I�u�W�F�N�g�̃|�C���^
	static CObject* m_apCurDraw[(int)DRAW_PRIORITY::ENUM_MAX];	//�Ō���̃I�u�W�F�N�g�̃|�C���^
	CObject* m_pPrevDraw;		//�O�̃I�u�W�F�N�g�ւ̃|�C���^
	CObject* m_pNextDraw;		//���̃I�u�W�F�N�g�ւ̃|�C���^
};

#endif // !_OBJECT_H_