//=============================================================================
//
// ステージ処理 [stage.h]
// Author : 鶴間俊樹
//
//=============================================================================
#ifndef _STAGE_H_
#define _STAGE_H_

#include "main.h"
#include "model.h"
#include "object.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define TEXT_FILE_NAME_STAGE_GAME "data/STAGE/stage001.txt"

//*****************************************************************************
// 前方宣言
//*****************************************************************************
class CObjectList;
class CTerrain;

//*****************************************************************************
// ステージクラス
//*****************************************************************************
class CStage
{
public:
	CStage();	//デフォルトコンストラクタ
	~CStage();	//デストラクタ
	void Init(void);	//初期化処理
	void CreateStage(char* strFileName);	//ファイルを読み込んでステージを生成
	void DestroyStage(void);	//現在のステージを破棄
	void AppendObject(CObject* pObject);	//オブジェクトの追加
	CObjectList* GetObjList(void);	//オブジェクトリストの取得

private:
	//地形の生成時の情報
	struct TerrainData {
		CModel::MODELTYPE modelType;	//地形のモデルの種類
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//角度
	};

	//オブジェクトの生成時の情報
	struct ObjectData {
		char sObjName[MAX_OBJECT_NAME];				//オブジェクトの名前
		CModel::MODELTYPE modelType;	//オブジェクトのモデルの種類
		D3DXVECTOR3 pos;		//位置
		D3DXVECTOR3 rot;		//角度
	};

	void LoadTerrainData(const char* pLoadText, TerrainData& terrainData);	//地形のデータをロード
	void LoadObjData(const char* pLoadText, ObjectData& objData);			//オブジェクトのデータをロード
	CTerrain* CreateLoadTerrain(TerrainData& terrainData);	//ロードした地形を生成
	CObject* CreateLoadObject(ObjectData& objData);	//ロードしたオブジェクトを生成

	//ツールのみ

	CTerrain* m_pStageTerrain;	//ステージの地形
	CObjectList* m_pStageObjectList;	//ステージのオブジェクトのリスト
};

#endif // !_STAGE_H_