//=============================================================================
//
// ステージ処理 [stage.cpp]
// Author : 鶴間俊樹
//
//=============================================================================
#include "stage.h"
#include "objectList.h"
#include "terrain.h"
#include "objectModel.h"

//=============================================================================
// マクロ定義
//=============================================================================
#define MAX_LOAD_TEXT (128)		//一度にファイルから読み込むテキストのバイト数
#define GLOW_COLOR_STAGE_OBJECT (D3DXCOLOR(0.0f, 0.8f, 1.0f, 1.0f))

//=============================================================================
// デフォルトコンストラクタ
//=============================================================================
CStage::CStage()
{
	//オブジェクトリストの生成
	m_pStageObjectList = new CObjectList;
	m_pStageTerrain = nullptr;
}

//=============================================================================
// デストラクタ
//=============================================================================
CStage::~CStage()
{
	if (m_pStageObjectList != nullptr) {
		//ステージのオブジェクトをすべて破棄する
		m_pStageObjectList->DeleteAllObject();
		//オブジェクトリストの破棄
		delete m_pStageObjectList;
	}
	//地形の破棄
	if (m_pStageTerrain != nullptr) m_pStageTerrain->Uninit();
}

//=============================================================================
// 初期化処理
//=============================================================================
void CStage::Init(void) {

}

//=============================================================================
// ファイルを読み込んでステージを生成
//=============================================================================
void CStage::CreateStage(char* strFileName) {
	//生成前に現在のステージを破棄
	DestroyStage();

	//読み込むファイル名がない場合終了
	if (strFileName == nullptr) return;

	FILE* pFile = nullptr;	//ファイルへのポインタ

	//ファイルを開く
	pFile = fopen(strFileName, "r");

	//ファイルがnullの場合終了
	if (pFile == nullptr) return;

	char sLoadText[MAX_LOAD_TEXT];		//一行ずつ読み込むファイルのテキスト
	char *pSplitText;	//ファイルのテキストを分割した文字列

	//------------------------------
	//読み込み開始位置まで行を飛ばす
	//------------------------------
	while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //一行ごとに文字列を取得
	{
		pSplitText = strtok(sLoadText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
		//分割したテキストのポインタがnullの場合ループを飛ばす
		if (pSplitText == nullptr) continue;

		//読み込み開始
		if (strcmp(pSplitText, "SCRIPT") == 0) {
			break;
		}
	}

	//------------------------------
	//ステージの生成
	//------------------------------
	bool bSetTerrain = false;	//地形設定中かどうか
	bool bSetObject = false;	//オブジェクト設定中かどうか
	TerrainData createTerrainData;	//生成する地形のデータ
	ObjectData createObjData;		//生成するオブジェクトのデータ

	while (fgets(sLoadText, MAX_LOAD_TEXT, pFile) != nullptr) //一行ごとに文字列を取得
	{
		pSplitText = strtok(sLoadText, " =\t\n");	//文字列の分割（空白 タブ 改行 ＝）
		//分割したテキストのポインタがnullの場合ループを飛ばす
		if (pSplitText == nullptr) continue;

		//コメント
		if (strstr(pSplitText, "//") != nullptr) {
			continue;
		}
		//読み込み終了
		if (strcmp(pSplitText, "END_SCRIPT") == 0) {
			break;
		}

		//------------------------------
		//地形の生成
		//------------------------------
		//地形の読み込み開始
		if (strcmp(pSplitText, "SET_TERRAIN") == 0 && !bSetObject) {
			bSetTerrain = true;

			//生成する地形のデータの初期化
			createTerrainData.modelType = (CModel::MODELTYPE) - 1;
			createTerrainData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			createTerrainData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (bSetTerrain) {
			//地形の読み込み終了
			if (strcmp(pSplitText, "END_SET_TERRAIN") == 0) {
				bSetTerrain = false;

				m_pStageTerrain = CreateLoadTerrain(createTerrainData);
			}

			//地形のデータを読み込む
			LoadTerrainData(pSplitText, createTerrainData);
		}

		//------------------------------
		//オブジェクトの生成
		//------------------------------
		//オブジェクトの読み込み開始
		if (strcmp(pSplitText, "SET_OBJECT") == 0 && !bSetTerrain) {
			bSetObject = true;

			//生成するオブジェクトのデータの初期化
			strcpy(createObjData.sObjName, "[_NO_NAME_]");
			createObjData.modelType = (CModel::MODELTYPE) - 1;
			createObjData.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			createObjData.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		}

		if (bSetObject) {
			//オブジェクトの読み込み終了
			if (strcmp(pSplitText, "END_SET_OBJECT") == 0) {
				bSetObject = false;

				//オブジェクトを生成する
				CObject* pObjectCreate = CreateLoadObject(createObjData);

				//生成できていない場合ループを飛ばす
				if (pObjectCreate == nullptr) continue;

				//オブジェクトの名前を設定
				//pObjectCreate->SetObjName(createObjData.sObjName);
				//オブジェクトリストに追加
				AppendObject(pObjectCreate);
			}

			//オブジェクトのデータを読み込む
			LoadObjData(pSplitText, createObjData);
		}
	}
}

//=============================================================================
// 現在のステージを破棄
//=============================================================================
void CStage::DestroyStage(void) {
	//地形の破棄
	if (m_pStageTerrain != nullptr) {
		m_pStageTerrain->Uninit();
		m_pStageTerrain = nullptr;
	}

	//リストのオブジェクトをすべて破棄
	if (m_pStageObjectList != nullptr) {
		m_pStageObjectList->DeleteAllObject();
	}
}

//=============================================================================
// オブジェクトの追加
//=============================================================================
void CStage::AppendObject(CObject* pObject) {
	//オブジェクトリストがnullの場合終了
	if (m_pStageObjectList == nullptr) return;

	//リストにオブジェクトを追加
	m_pStageObjectList->AppendNode(pObject);
}

//=============================================================================
// 地形のデータを読み込む
//=============================================================================
void CStage::LoadTerrainData(const char* pLoadText, TerrainData& terrainData) {
	//読み込んだテキストがnullの場合終了
	if (pLoadText == nullptr) return;

	//モデルの種類の設定
	if (strcmp(pLoadText, "MODEL") == 0) {
		pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;

		for (int nCntModel = 0; nCntModel < (int)CModel::MODELTYPE::ENUM_MAX; nCntModel++)
		{
			//モデルのパスと文字列を比較する
			if (strcmp(pLoadText, CModel::GetPathName((CModel::MODELTYPE)nCntModel)) == 0) {
				//モデルの種類の番号を設定
				terrainData.modelType = (CModel::MODELTYPE)nCntModel;
				break;
			}
		}
	}
	//位置の設定
	else if (strcmp(pLoadText, "POS") == 0) {
		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		//X座標の設定
		terrainData.pos.x = (float)atof(pLoadText);	

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		terrainData.pos.y = (float)atof(pLoadText);	//Y座標の設定

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		terrainData.pos.z = (float)atof(pLoadText);	//Z座標の設定
	}
	//角度の設定
	else if (strcmp(pLoadText, "ROT") == 0) {
		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		//X座標の設定
		terrainData.rot.x = (float)atof(pLoadText);	

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		terrainData.rot.y = (float)atof(pLoadText);	//Y座標の設定

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		terrainData.rot.z = (float)atof(pLoadText);	//Z座標の設定
	}
}

//=============================================================================
// オブジェクトのデータを読み込む
//=============================================================================
void CStage::LoadObjData(const char* pLoadText, ObjectData& objData) {
	//読み込んだテキストがnullの場合終了
	if (pLoadText == nullptr) return;

	//オブジェクト名の設定
	if (strcmp(pLoadText, "NAME") == 0) {
		pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
		//文字列がnullの場合終了
		if (pLoadText == nullptr || strlen(pLoadText) >= MAX_OBJECT_NAME) return;

		//文字列のコピー
		strcpy(objData.sObjName, pLoadText);
	}
	//モデルの種類の設定
	else if (strcmp(pLoadText, "MODEL") == 0) {
		pLoadText = strtok(nullptr, " =#\t\n");	//文字列の分割
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;

		for (int nCntModel = 0; nCntModel < (int)CModel::MODELTYPE::ENUM_MAX; nCntModel++)
		{
			//モデルのパスと文字列を比較する
			if (strcmp(pLoadText, CModel::GetPathName((CModel::MODELTYPE)nCntModel)) == 0) {
				//モデルの種類の番号を設定
				objData.modelType = (CModel::MODELTYPE)nCntModel;
				break;
			}
		}
	}
	//位置の設定
	else if (strcmp(pLoadText, "POS") == 0) {
		//文字列の分割
		pLoadText = strtok(nullptr, " =");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		//X座標の設定
		objData.pos.x = (float)atof(pLoadText);

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		objData.pos.y = (float)atof(pLoadText);	//Y座標の設定

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		objData.pos.z = (float)atof(pLoadText);	//Z座標の設定
	}
	//角度の設定
	else if (strcmp(pLoadText, "ROT") == 0) {
		//文字列の分割
		pLoadText = strtok(nullptr, " =");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		//X座標の設定
		objData.rot.x = (float)atof(pLoadText);

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		objData.rot.y = (float)atof(pLoadText);	//Y座標の設定

		//文字列の分割
		pLoadText = strtok(nullptr, " ");
		//文字列がnullの場合終了
		if (pLoadText == nullptr) return;
		objData.rot.z = (float)atof(pLoadText);	//Z座標の設定
	}
}

//=============================================================================
// ロードした地形を生成
//=============================================================================
CTerrain* CStage::CreateLoadTerrain(TerrainData& terrainData) {
	//生成するモデルの種類が存在する場合
	if ((int)terrainData.modelType < 0 || (int)terrainData.modelType >= (int)CModel::MODELTYPE::ENUM_MAX) return nullptr;
	//地形を生成する
	CTerrain* pTerrain = CTerrain::Create(terrainData.modelType, terrainData.pos, terrainData.rot);
	if (pTerrain == nullptr) return nullptr;

	CModel* pModel = pTerrain->GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(GLOW_COLOR_STAGE_OBJECT);
	}

	return pTerrain;
}

//=============================================================================
// ロードしたオブジェクトを生成
//=============================================================================
CObject* CStage::CreateLoadObject(ObjectData& objData) {
	//生成するモデルの種類が存在しない場合ループを飛ばす
	if ((int)objData.modelType < 0 || (int)objData.modelType >= (int)CModel::MODELTYPE::ENUM_MAX) return nullptr;

	//モデルオブジェクトを生成する
	CObjectModel* pCreateObj = CObjectModel::Create(objData.modelType, objData.pos, objData.rot, false);
	if (pCreateObj == nullptr) return nullptr;

	CModel* pModel = pCreateObj->GetPtrModel();
	if (pModel != nullptr) {
		pModel->SetColorGlow(GLOW_COLOR_STAGE_OBJECT);
	}

	return pCreateObj;
}

//=============================================================================
// オブジェクトリストの取得
//=============================================================================
CObjectList* CStage::GetObjList(void) {
	return m_pStageObjectList;
}