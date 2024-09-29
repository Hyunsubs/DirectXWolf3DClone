#include "pch.h"

#include "CRenderMgr.h"
#include "CTaskMgr.h"

#include "CGameObject.h"
#include "func.h"

void CreateObject(CGameObject* _NewObject, int _LayerIndex)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_0 = _LayerIndex;
	task.Param_1 = (DWORD_PTR)_NewObject;

	CTaskMgr::GetInst()->AddTask(task);
}

void DeleteObject(CGameObject* _DeleteObject)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_0 = (DWORD_PTR)_DeleteObject;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_0 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}


void ChangeLevel(CLevel* _Level, LEVEL_STATE _NextLevelState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;

	task.Param_0 = (DWORD_PTR)_Level;
	task.Param_1 = (DWORD_PTR)_NextLevelState;

	CTaskMgr::GetInst()->AddTask(task);
}

bool IsValid(CGameObject*& _Object)
{
	if (nullptr == _Object)
		return false;

	if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}	

	return true;
}

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.vPos = _Pos;
	Info.vScale = _Scale;
	Info.vRot = _Rot;
	Info.LifeTime = _Life;
	Matrix matRot = XMMatrixRotationX(Info.vRot.x)
		* XMMatrixRotationY(Info.vRot.y)
		* XMMatrixRotationZ(Info.vRot.z);

	Info.matWorld = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z) 
					* XMMatrixRotationX(_Rot.x) 
					* XMMatrixRotationY(_Rot.y) 
					* XMMatrixRotationZ(_Rot.z) 
					* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);



	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.matWorld = _matWorld;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	Info.LifeTime = _Life;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::CIRCLE;
	Info.vPos = _Pos;
	Info.vScale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	Info.vRot = Vec3(0.f, 0.f, 0.f);
	Info.LifeTime = _Life;

	Info.matWorld = XMMatrixScaling(Info.vScale.x, Info.vScale.y, Info.vScale.z)
				    * XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);

	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugLine(Vec3 _StartPos, Vec3 _Dir, float fDist, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::LINE;
	Info.vPos = _StartPos;
	Info.vScale = Vec3(1.f, fDist, 1.f);
	Info.vRot = _Dir;
	Info.LifeTime = _Life;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	Info.matWorld = XMMatrixScaling(Info.vScale.x, Info.vScale.y, Info.vScale.z)
		* XMMatrixRotationX(_Dir.x)
		* XMMatrixRotationY(_Dir.y)
		* XMMatrixRotationZ(_Dir.z)
		* XMMatrixTranslation(Info.vPos.x, Info.vPos.y, Info.vPos.z);

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}


void SaveWString(const wstring& _String, FILE* _File)
{
	size_t len = _String.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_String.c_str(), sizeof(wchar_t), len, _File);	
}

void LoadWString(wstring& _String, FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);

	_String.resize(len);
	fread((wchar_t*)_String.c_str(), sizeof(wchar_t), len, _File);	
}

float deg2rad(float deg)
{
	return (float)deg / 180.f * PI;
}

float rad2deg(float rad)
{
	return (float)rad / PI * 180;
}

float custom_distance2d(Vec2 x, Vec2 y)
{
	return sqrt(pow(abs(x.x - y.x), 2) + pow(abs(x.y - y.y), 2));
}

float custom_distance2d(Vec3 x, Vec3 y)
{
	return sqrt(pow(abs(x.x - y.x), 2) + pow(abs(x.y - y.y), 2));
}

float distance_path_node(PathNode* _a, PathNode* _b)
{
	return sqrtf(pow(_a->curIndex[0] - _b->curIndex[0], 2) + pow(_a->curIndex[1] - _b->curIndex[1], 2));
}

// a 스타 진행 후 다음에 갈 위치를 리턴해줌
vector<Vec2> FindPath(Vec2 _StartIndex, Vec2 _EndIndex, vector<tTileInfo>& _MapData, Vec2 _ColRow)
{
	const int PathNodeSize = _MapData.size();
	const int map_col = _ColRow.x;
	const int map_row = _ColRow.y;

	vector<PathNode> nodes = {};
	for (int i = 0; i < PathNodeSize; i++)
	{
		nodes.push_back(PathNode{});
	}

	// path 초기화
	for (int x = 0; x < map_col; x++)
	{
		for (int y = 0; y < map_row; y++)
		{
			nodes[y * map_col + x].curIndex.x = x;
			nodes[y * map_col + x].curIndex.y = y;
			nodes[y * map_col + x].bVisited = false;
			nodes[y * map_col + x].fGlobalGoal = FLT_MAX;
			nodes[y * map_col + x].fLocalGoal = FLT_MAX;
			nodes[y * map_col + x].parent = nullptr;
			nodes[y * map_col + x].attribute = _MapData[y * map_col + x].ImgIdx;
		}
	}

	for (int x = 0; x < map_col; x++)
	{
		for (int y = 0; y < map_row; y++)
		{
			// 위 아래 왼쪽 오른쪽 연결
			if (y > 0)
				nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y - 1) * map_col + (x + 0)]);
			if (y < map_row - 1)
				nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y + 1) * map_col + (x + 0)]);
			if (x > 0)
				nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y + 0) * map_col + (x - 1)]);
			if (x < map_col - 1)
				nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y + 0) * map_col + (x + 1)]);

			//// 대각선 연결
			//if (y > 0 && x > 0)
			//	nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y - 1) * map_col + (x - 1)]);
			//if (y < map_row - 1 && x>0)
			//	nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y + 1) * map_col + (x - 1)]);
			//if (y > 0 && x < map_col - 1)
			//	nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y - 1) * map_col + (x + 1)]);
			//if (y < map_col - 1 && x < map_col - 1)
			//	nodes[y * map_col + x].vecNeighbours.push_back(&nodes[(y + 1) * map_col + (x + 1)]);

		}
	}

	// 초기 노드 목표 노드 설정
	PathNode* nodeStart = &nodes[map_col * _StartIndex.y + _StartIndex.x];
	PathNode* nodeEnd = &nodes[map_col * _EndIndex.y + _EndIndex.x];


	nodeStart->fLocalGoal = 0.f;
	nodeStart->fGlobalGoal = distance_path_node(nodeStart, nodeEnd);

	PathNode* CurrentNode = nodeStart;

	list<PathNode*> listNotTested;
	listNotTested.push_front(nodeStart);

	while (!listNotTested.empty() && CurrentNode != nodeEnd)
	{
		// 글로벌 골 값으로 정렬한다. 가장 낮은 값이 첫번째가 된다.
		listNotTested.sort([](const PathNode* lhs, const PathNode* rhs) {return lhs->fGlobalGoal < rhs->fGlobalGoal; });

		// 이미 방문한 노드는 리스트에서 제외한다.
		while (!listNotTested.empty() && listNotTested.front()->bVisited)
			listNotTested.pop_front();

		// 방문한 노드를 제외했을때 유효한 노드가 없을시 루프를 나와야 한다.
		if (listNotTested.empty())
			break;

		CurrentNode = listNotTested.front();
		CurrentNode->bVisited = true; // 노드는 한번만 방문한다.

		// 자신의 인접 노드를 확인한다.
		for (auto nodeNeighbour : CurrentNode->vecNeighbours)
		{
			// 인접 노드가 방문된적 없고 벽 값이 0(지나갈수 있음)이면 테스트 되지 않은 리스트에 넣어준다.
			if (!nodeNeighbour->bVisited && nodeNeighbour->attribute == 0)
				listNotTested.push_back(nodeNeighbour);

			// 선택된 이웃 노드와 예상되는 최소 거리를 구한다.
			float fPossiblyLowerGoal = CurrentNode->fLocalGoal + distance_path_node(CurrentNode, nodeNeighbour);

			// 선택된 노드가 더 낮은 값이면 parent를 선택된 노드로 설정하고
			// 예상 최소 거리를 해당 노드와의 거리로 재설정 한다.
			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal)
			{
				nodeNeighbour->parent = CurrentNode;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;

				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + distance_path_node(nodeNeighbour, nodeEnd);
			}
		}
	}

	// 탐색이 잘 됐을때
	if (CurrentNode == nodeEnd)
	{
		PathNode* pathfinder = nodeEnd;
		list<PathNode*> path = {};
		vector<Vec2> returnPath = {};
		
		// 종료지점부터 시작지점 까지 경로를 모두 넣음
		while (nodeEnd->parent != nullptr)
		{
			path.push_back(nodeEnd);
			nodeEnd = nodeEnd->parent;
		}

		list<PathNode*>::iterator iter = path.begin();
		for (iter; iter != path.end(); iter++)
		{
			returnPath.push_back(Vec2((*iter)->curIndex));
		}

		reverse(returnPath.begin(), returnPath.end());

		return returnPath;
	}

	// 안됐을때 시작 위치에 계속 있게 됨
	else
	{
		Vec2 _StartIndex;
	}

}



#include "commdlg.h"
const wstring FileLoadSystem(wstring _Filter, wstring _InitialDir)
{
	// common dialog box structure, setting all fields to 0 is important
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = CEngine::GetInst()->GetMainWnd();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _Filter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = _InitialDir.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&ofn) == TRUE)
	{
		return szFile;
	}
	else
	{
		return L"";
	}
}

const wstring FileSaveSystem(wstring _Filter, wstring _InitialDir)
{
	// common dialog box structure, setting all fields to 0 is important
	OPENFILENAME ofn = { 0 };
	TCHAR szFile[260] = { 0 };
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = CEngine::GetInst()->GetMainWnd();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = _Filter.c_str();
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = _InitialDir.c_str();
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetSaveFileName(&ofn) == TRUE)
	{
		return szFile;
	}
	else
	{
		return L"";
	}
}
