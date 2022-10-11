//=============================================================================
//
// �X�R�A���� [score.h]
// Author : �ߊԏr��
//
//=============================================================================
#ifndef _SCORE_H_
#define _SCORE_H_

#include "object.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************

//*****************************************************************************
//�O���錾
//*****************************************************************************
class CObject2D;

//*****************************************************************************
// �X�R�A�N���X
//*****************************************************************************
class CScore : public CObject
{
public:
	CScore();	//�f�t�H���g�R���X�g���N�^
	CScore(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//�I�[�o�[���[�h���ꂽ�R���X�g���N�^
	~CScore();	//�f�X�g���N�^
	static CScore* Create(int nNumDigit, CTexture::TEXTURE_TYPE type, D3DXVECTOR3 pos, float fSize);	//��������
	HRESULT Init(void);	//����������
	void Uninit(void);	//�I������
	void Update(void);	//�X�V����
	void Draw(void) {}	//�`�揈��
	void SetScore(int nScore);	//�X�R�A�̐ݒ�
	void AddScore(int nScore);	//�X�R�A�̒ǉ�
	int GetScore(void);	//�X�R�A���擾
	void SetDigitMin(int nDigit) { m_nDigitMin = nDigit; }	//�\������錅���̍ŏ�����ݒ�

private:
	std::vector<CObject2D*> m_vNumberPtr;	//�����̃I�u�W�F�N�g�̃|�C���^�z��
	D3DXVECTOR3 m_pos;	//�X�R�A�̉E��̈ʒu
	float m_fSize;		//�X�R�A�̃i���o�[�̃T�C�Y
	int m_nScore;		//�X�R�A�̒l
	int m_nDigitMin;	//�\������錅���̍ŏ���
};

#endif // !_SCORE_H_