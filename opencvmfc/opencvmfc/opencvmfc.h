// opencvmfc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
 
#define IMAGE_WIDTH 256
#define IMAGE_HEIGHT 256
#define IMAGE_CHANNELS 3


// CopencvmfcApp:
// See opencvmfc.cpp for the implementation of this class
//

class CopencvmfcApp : public CWinApp
{
public:
	CopencvmfcApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	IplImage* TempImage;
};

extern CopencvmfcApp theApp;