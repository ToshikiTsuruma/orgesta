//=============================================================================
//
// 2D�I�����j���[���� [selectMenu2D.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SELECT_MENU_2D_H_
#define _SELECT_MENU_2D_H_

#include "selectMenu.h"
#include "object2D.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
// �O���錾
//*****************************************************************************

//*****************************************************************************
// 2D�I�����j���[�N���X
//*****************************************************************************
class CSelectMenu2D : public CSelectMenu
{
public:

	CSelectMenu2D();		//�f�t�H���g�R���X�g���N�^
	CSelectMenu2D(int nNumSelect, bool bUseBG);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	virtual ~CSelectMenu2D();		//�f�X�g���N�^
	static CSelectMenu2D* Create(int nNumSelect, bool bUseBG);	//��������
	virtual HRESULT Init(void);	//����������
	virtual void Uninit(void);	//�I������
	virtual void Update(void);	//�X�V����
	virtual void Draw(void) {}	//�`�揈��

	virtual void BeginChangeSelect(void);	//�I���̕ύX���J�n���ꂽ�Ƃ��̏���
	virtual void EndChangeSelect(void) {}	//�I���̕ύX���I�������Ƃ��̏���

	void SetSelectUI(int nIdx, D3DXVECTOR3 pos, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex);	//�I����UI�̐ݒ�
	void CreateSelectIcon(D3DXVECTOR3 posOffset, float fWidth, float fHeight, CTexture::TEXTURE_TYPE typeTex);	//�I�𒆃A�C�R���̐���
	void SetIconPosOffset(int nIdx, D3DXVECTOR3 posOffset);	//�I���A�C�R���̈ʒu�̃I�t�Z�b�g��ݒ�

private:
	void CreateSelectUI(void);	//UI�𐶐�
	void UpdateSelectIconPos(void);	//�I���A�C�R���̈ʒu���X�V

	CObject2D** m_ppSelectUIArray;	//�I������UI�̓��I�z��

	CObject2D* m_pSelectIcon;	//�I�𒆂̑I�����ׂ̗ɐ��������A�C�R��
	D3DXVECTOR3* m_pPosSelectIconOffsetArray;	//�I�𒆃A�C�R���̑I��������̃I�t�Z�b�g�ʒu�̓��I�z��̃|�C���^
};

#endif // !_SELECT_MENU_3D_H_