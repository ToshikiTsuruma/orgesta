//=============================================================================
//
// モデル処理 [model.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _MODEL_H_
#define _MODEL_H_

#include "main.h"
#include "texture.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define MAX_MODEL_FILE_PATH (128)	//モデルのファイルのパスの長さの最大
#define MAX_MATERIAL (16)			//モデルごとのマテリアルの最大数

//*****************************************************************************
// モデルクラス
//*****************************************************************************
class CModel
{
public:
	//モデルの種類
	enum class MODELTYPE{
		//地形
		TERRAIN = 0,	//地形

		//オブジェクト
		OBJ_APPLE,
		OBJ_APPLE_TREE,
		OBJ_APPLE_BULLET,
		OBJ_MENUAPPLE,
		OBJ_TREE,
		OBJ_SCAPEGOAT,
		OBJ_SEEDLING,
		OBJ_FOUNTAIN,
		OBJ_ROCK,

		//プレイヤー
		pl_0, pl_1, pl_2, pl_3, pl_4, pl_5, pl_6, pl_7, pl_8, 

		//敵
		eNo_0, eNo_1, eNo_2, eNo_3, eNo_4,	//通常敵
		eHu_0, eHu_1, eHu_2, eHu_3, eHu_4, eHu_5, eHu_6, eHu_7, eHu_8, eHu_9,	//通常敵
		eSo_0, eSo_1, eSo_2, eSo_3, eSo_4,	//魔術師敵

		ENUM_MAX	//最大数
	};

	//モデルデータ
	typedef struct
	{
		LPD3DXMESH pMesh;	//メッシュ（頂点情報）へのポインタ
		LPD3DXBUFFER pBuffMat;	//マテリアルへのポインタ
		DWORD nNumMat;	//マテリアルの数
	} ModelData;

	//色の変更
	typedef struct {
		D3DXCOLOR colAdd;	//変更中はフレーム毎に加算される色
		int nTimeFinish;	//色の変更にかかる時間
		int nCnt;			//カウンター
	} ChangeDiffuse;

	CModel();	//デフォルトコンストラクタ
	~CModel();	//デストラクタ
	static CModel* Create(MODELTYPE type, D3DXVECTOR3 pos, D3DXVECTOR3 rot, CModel* pParent, bool bOutLine);	//生成処理
	static HRESULT Load(void);	//モデルデータの読み込み
	static void Unload(void);	//モデルデータの解放
	static ModelData GetModelData(MODELTYPE type);	//モデルデータの取得
	static char* GetPathName(MODELTYPE type);		//テクスチャのパスの文字列の取得
	static D3DXCOLOR GetDefaultColor(MODELTYPE type, int nIdx);	//モデルのデフォルトの色を取得

	HRESULT Init(void);	//初期化処理
	void Uninit(void);	//終了処理
	void Update(void);	//更新処理
	void Draw(void);	//描画処理
	void SetPos(D3DXVECTOR3 pos);	//位置座標の設定
	D3DXVECTOR3 GetPos(void);		//位置座標の取得
	void SetRot(D3DXVECTOR3 rot);	//角度の設定
	D3DXVECTOR3 GetRot(void);		//角度の取得
	void UpdateMtxWorld(void);		//ワールドマトリックスの更新
	D3DXMATRIX GetMtxWorld(void);			//ワールドマトリックスの取得
	D3DXMATRIX* GetPtrMtxWorld(void);		//ワールドマトリックスのポインタの取得
	void SetModelType(MODELTYPE type);		//モデルのタイプの設定
	MODELTYPE GetModelType(void);			//モデルのタイプの取得
	void SetMaterialDiffuse(D3DXCOLOR col, int nIdx);	//マテリアルの色の設定
	D3DXCOLOR GetMaterialDiffuse(int nIdx);				//マテリアルの色の取得
	void SetMaterialDiffuseAlpha(float fAlpha, int nIdx);	//マテリアルのアルファ値を設定
	void SetMaterialDiffuseAlphaAll(float fAlpha);			//すべてのマテリアルのアルファ値を設定
	float GetMaterialDiffuseAlpha(int nIdx);				//マテリアルのアルファ値を取得
	void SetMaterialSpecular(D3DXCOLOR col, int nIdx);	//マテリアルのスペキュラー色の設定
	void SetMaterialEmissive(D3DXCOLOR col, int nIdx);	//マテリアルの発光色の設定
	void SetMaterialPower(float fPower, int nIdx);		//マテリアルの反射の質感の設定
	void StartChangeMaterialDiffuse(int nIdxMat, D3DXCOLOR colDest, int nTimeFin);	//マテリアルの色の変更の開始
	void SetColorGlow(D3DXCOLOR col);	//輪郭の発光色の設定

	void CreateCloneMesh(void);		//複製メッシュの生成
	void ExpansionCloneMesh(void);	//複製メッシュを膨張させる
	void SetColorOutline(D3DXCOLOR col);	//輪郭の色の設定
	void SetDrawOutline(bool bDraw);		//輪郭の表示設定

private:
	void UpdateColorChange(void);	//色の変更の更新

	static ModelData m_aModelData[(int)MODELTYPE::ENUM_MAX];	//全モデルの情報
	static char m_asFilePath[(int)MODELTYPE::ENUM_MAX][MAX_MODEL_FILE_PATH];	// モデルのテキストファイルのパス
	static D3DXMATERIAL m_aMatDefault[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//モデルのマテリアルの初期値
	static CTexture::TEXTURE_TYPE m_aTexType[(int)MODELTYPE::ENUM_MAX][MAX_MATERIAL];	//マテリアルごとのテクスチャの種類

	MODELTYPE m_modelType;	//モデルの種類
	D3DXVECTOR3 m_pos;		//位置
	D3DXVECTOR3 m_rot;		//向き
	D3DXMATRIX m_mtxWorld;	//ワールドマトリックス
	CModel* m_pParent;		//親のモデルへのポインタ
	int m_nIdxParent;		//親のインデックス
	D3DXMATERIAL m_aMat[MAX_MATERIAL];	//モデルのマテリアル
	ChangeDiffuse m_aChangeDiffuse[MAX_MATERIAL];	//モデルのマテリアルの色を変更する構造体の配列
	D3DXCOLOR m_colGlow;	//輪郭の発光色の色

	bool m_bOutline;			//輪郭があるかどうか
	D3DXCOLOR m_colOutline;	//輪郭の色
	LPD3DXMESH m_pCloneMesh;	//複製したメッシュのポインタ
};

#endif // !_MODEL_H_