//=============================================================================
//
// �I�����j���[���� [selectMenu.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SELECT_MENU_H_
#define _SELECT_MENU_H_

#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// �I�����j���[�N���X
//*****************************************************************************
class CSelectMenu : public CObject
{
public:
	enum class SELECT_TYPE {
		VERTICAL = 0,	//�㉺
		HORIZONTAL,		//���E
		ENUM_MAX
	};

	CSelectMenu();		//�f�t�H���g�R���X�g���N�^
	CSelectMenu(int nNumSelect, bool bUseBG);		//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CSelectMenu();		//�f�X�g���N�^
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void) {}	//�`�揈��

	virtual void BeginChangeSelect(void) = 0;	//�I���̕ύX���J�n���ꂽ�Ƃ��̏���
	virtual void EndChangeSelect(void) = 0;		//�I���̕ύX���I�������Ƃ��̏���
	void SetMenuBG(CTexture::TEXTURE_TYPE typeTex, D3DXVECTOR3 pos, float fWidth, float fHeight);	//���j���[�̔w�i�̐ݒ�

	void SetSelectType(SELECT_TYPE type) { m_typeSelect = type; }	//���͂̎�ނ�ݒ�
	int GetNumSelect(void) { return m_nNumSelect; }	//�I�����̐����擾
	int GetIdxCurSelect(void) { return m_nIdxCurSelect; }	//���݂̑I���̔ԍ����擾
	void SetLockChangeSelect(bool bLock) { m_bLockChangeSelect = bLock; }	//�I���̕ύX�̐�����Ԃ̐ݒ�
	bool GetLockChangeSelect(void) { return m_bLockChangeSelect; }			//�I���̕ύX�̐�����Ԃ̎擾

private:
	void ChangeSelect(void);	//�I���̕ύX

	const int m_nNumSelect;		//�I�����̐�
	SELECT_TYPE m_typeSelect;	//���͂̎��
	int m_nIdxCurSelect;		//���݂̑I���̔ԍ�
	bool m_bFirstFrame;			//�������čŏ��̂P�t���[�����ǂ���
	bool m_bLockChangeSelect;	//�I���̕ύX�̐������
	CObject2D* m_pMenuBG;		//���j���[�̔w�i�ւ̃|�C���^
};

#endif // !_SELECT_MENU_H_