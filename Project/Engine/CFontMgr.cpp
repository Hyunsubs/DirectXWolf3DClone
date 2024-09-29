#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr()
	: m_FW1Factory(nullptr)
	, m_FontWrapper(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (nullptr != m_FW1Factory)
		m_FW1Factory->Release();

	if (nullptr != m_FontWrapper)
		m_FontWrapper->Release();
}

void CFontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Wolfenstein", &m_FontWrapper)))
	{
		if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Arial", &m_FontWrapper)))
		{
			assert(nullptr);
		}
	}
}

void CFontMgr::DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_FontWrapper->DrawString(
		CONTEXT,
		_pStr,				// String
		_fFontSize,			// Font size
		_fPosX,				// X position
		_fPosY,				// Y position
		_Color,				// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}



void CFontMgr::Render()
{
	for (int i = 0; i < m_FontList.size(); i++)
	{
		DrawFont(m_FontList[i]->_pStr, m_FontList[i]->_fPosX, m_FontList[i]->_fPosY, m_FontList[i]->_fFontSize, m_FontList[i]->_Color);
	}
}

void CFontMgr::Clear()
{
	m_FontList.clear();
}


