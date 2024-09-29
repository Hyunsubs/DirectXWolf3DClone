#pragma once

#include "singleton.h"

#include <FontEngine/FW1FontWrapper.h>
#include <FontEngine/FW1CompileSettings.h>

#ifdef _DEBUG
#pragma comment(lib, "FontEngine/FW1FontWrapper_debug.lib")
#else
#pragma comment(lib, "FontEngine/FW1FontWrapper.lib"))
#endif



class CFontMgr :
    public CSingleton<CFontMgr>
{
    SINGLE(CFontMgr)
private:
    IFW1Factory* m_FW1Factory;
    IFW1FontWrapper* m_FontWrapper;

    vector<FontData*> m_FontList;

public:
    void Init();
    void AddFont(FontData* item) { m_FontList.push_back(item); }
    void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color);
    void Render();
    void Clear();
};

