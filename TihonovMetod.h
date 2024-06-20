
// TihonovMetod.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CTihonovMetodApp:
// Сведения о реализации этого класса: TihonovMetod.cpp
//

class CTihonovMetodApp : public CWinApp
{
public:
	CTihonovMetodApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CTihonovMetodApp theApp;
