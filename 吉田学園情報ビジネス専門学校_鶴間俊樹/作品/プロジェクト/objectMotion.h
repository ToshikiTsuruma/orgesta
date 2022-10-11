//=============================================================================
//
// モーションオブジェクト処理 [objectMotion.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _OBJECT_MOTION_H_
#define _OBJECT_MOTION_H_

#include "main.h"
#include "object.h"
#include "model.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_KEY_MOTION (12)	//モーションごとの最大キー数
//#define MAX_ATTACK_OBJECT (24)	//一度に攻撃できる最大のオブジェクト数
#define DEAD_HEIGHT (-5000.0f)	//死亡する高さ
#define POWER_GRAVITY (0.6f)			//重力
#define POWER_GRAVITY_GROUND (15.0f)	//地上にいる時の重力

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjectList;

//*****************************************************************************
// モーションオブジェクトクラス
//*****************************************************************************
class CObjectMotion : public CObject
{
public:
	//パーツ情報の構造体
	typedef struct {
		CModel::MODELTYPE modelType;	//パーツのモデルのタイプ
		int nIdx;			//パーツのインデックス
		int nIdxParent;		//親パーツのインデックス
		D3DXVECTOR3 offset;	//オフセット
	} PARTS_INFO;

	//キーの構造体
	typedef struct
	{
		D3DXVECTOR3 pos;	//位置
		D3DXVECTOR3 rot;	//角度
	} KEY;
	//キー情報の構造体
	typedef struct {
		int nFrame;	//再生フレーム
		KEY* pKeyArray;	//各パーツのキー要素の動的配列の先頭ポインタ
	} KEY_INFO;
	//モーション情報の構造体
	typedef struct {
		bool bLoop;		//ループするかどうか
		int nNumKey;	//キーの総数
		KEY_INFO aKeyInfo[MAX_KEY_MOTION];	//キー情報
	} MOTION_INFO;

	CObjectMotion();		//デフォルトコンストラクタ
	CObjectMotion(const PARTS_INFO* pPartsInfoArray, int nNumParts, const MOTION_INFO* pMotionInfoArray, int nNumTypeMotion, bool bOutline);		//オーバーロードされたコンストラクタ
	virtual ~CObjectMotion();	//デストラクタ
	static void LoadMotionText(char* pLoadDirectory, PARTS_INFO*& pPartsInfoArray, int& nNumParts, MOTION_INFO* pMotionInfoArray, int nNumMotionType);	//モーション情報をロード
	virtual HRESULT Init(void);	//初期化処理
	virtual void Uninit(void);	//終了処理
	virtual void Update(void);	//更新処理
	virtual void Draw(void);	//描画処理
	void SetMotion(int nType);	//モーションの設定
	int GetMotionType(void);	//モーションの種類の取得
	virtual void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	virtual D3DXVECTOR3 GetPos(void);		//位置座標の取得
	virtual D3DXVECTOR3* GetPtrPos(void);	//位置座標のポインタの取得
	virtual void SetRot(D3DXVECTOR3 rot);	//角度の設定
	virtual D3DXVECTOR3 GetRot(void);		//角度の取得
	virtual void SetMove(D3DXVECTOR3 move) {}	//移動量の設定
	void UpdateMtxWorldAll(void);			//オブジェクトの全モデルのワールドマトリックスの更新
	void SetColorOutlineAll(D3DXCOLOR col);	//オブジェクトの全モデルの輪郭の色の指定

	void InitObjAttacked(void);	//攻撃済みリストの初期化
	void Attack(int nObjtype, D3DXVECTOR3 pos, float fRadius, int nDamage, CObject::DAMAGE_TYPE typeDamage, int* pNumKill);	//攻撃
	void GetPosCollision(D3DXVECTOR3* const pPosCollision, D3DXVECTOR3 posOffset, int nIdxParts);	//当たり判定の位置を取得
	D3DXMATRIX* GetPtrMtxWorld(void);	//ワールドマトリックスのポインタを取得
	CModel* GetPtrModel(int nIdxParts);	//モデルのポインタを取得
	bool GetTransMotion(void);	//モーションの遷移判定
	bool GetEndMotion(void);	//モーションの終了判定
	int GetCurKey(void);		//現在のモーションのキーの取得
	int GetCurMotionCnt(void);	//現在のモーションのカウントの取得
	void SetDiffuseModelAll(D3DXCOLOR col, int nIdx);	//モデルのディフューズ色を設定する
	void SetSpecularModelAll(D3DXCOLOR col, int nIdx);	//モデルのスペキュラー色を設定する
	void StartChangeDiffuseAll(int nIdxMat, D3DXCOLOR colDest, int nTimeFin);	//モデルのマテリアルの色の変更の開始
	void SetColorGlowAll(D3DXCOLOR col);	//モデルの輪郭の発光色を設定する
	bool FadeModelAll(float fDestAlpha, float fSpeedFade);	//モデルの色をフェードさせる
	void SetDrawOutlineAll(bool bDraw);	//モデルの輪郭の表示設定

private:
	//モーション更新に使う情報
	typedef struct {
		D3DXVECTOR3 deltaRot;	//モーションの角度の差分
		D3DXVECTOR3 deltaPos;	//モーションの位置の差分
		D3DXVECTOR3 curRot;		//現在（キー遷移時）の角度
		D3DXVECTOR3 curPos;		//現在（キー遷移時）の位置
		D3DXVECTOR3 offsetPos;		//オブジェクトから各パーツへのオフセット
	} UPDATE_MOTION_INFO;

	const int m_nNumParts;		//モデルのパーツの数
	CModel** m_ppModelArray;	//"モデルのポインタ"の動的配列の先頭ポインタ
	UPDATE_MOTION_INFO* m_pUpdateMotionInfoArray;	//モーション更新情報の配列のポインタ

	const int m_nNumTypeMotion;			//モーションのタイプ数
	MOTION_INFO* m_pMotionInfoArray;	//モーション情報の動的配列のポインタ

	D3DXVECTOR3	m_pos;		//位置座標
	D3DXVECTOR3 m_rot;		//角度
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	bool m_bEndMotion;		//モーションが終了している状態かどうか
	CObjectList* m_pListAttacked;	//攻撃済みのオブジェクトのリスト

	int m_nTypeMotion;		//モーションのタイプ
	bool m_bLoopMotion;		//ループするかどうか
	int m_nKey;				//キーNo.
	int m_nCntMotion;		//モーションカウンター
	bool m_bTransMotion;	//モーションの補間
	bool m_bSetCurMotion;	//現在（キー遷移時）を設定したかどうか
};

#endif // !_OBJECT_MOTION_H_