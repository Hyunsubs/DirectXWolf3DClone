#pragma once

#include <vector>
#include <string>

enum SCRIPT_TYPE
{
	ALPHANOISESCRIPT,
	BULLETSCRIPT,
	CAMERAMOVESCRIPT,
	CAMERASIDEWALKSCRIPT,
	DELAYEDINSTANTIATOR,
	FADESCRIPT,
	FLASHFONT,
	FONTPRINTSCRIPT,
	INTERMISSIONSCRIPT,
	LEVELCHANGERSCRIPT,
	MONSTERSCRIPT,
	PICKUPSCRIPT,
	PLAYERHANDSCRIPT,
	PLAYERSCRIPT,
	PLAYERSELECTWEAPON,
	PLAYERSTATUSSCRIPT,
	RAYCASTRENDERERSCRIPT,
	SPRITESCRIPT,
	TIMERSCRIPT,
	UIANIMSCRIPT,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static CScript * GetScript(UINT _iScriptType);
	static const wchar_t * GetScriptName(CScript * _pScript);
};
