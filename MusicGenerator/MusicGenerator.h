/***********************************************************************
Author:		John Pinkerton
File:		MusicGenerator.h
Purpose:	Header file main Windows application.
***********************************************************************/

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CMusicGeneratorApp:
// See MusicGenerator.cpp for the implementation of this class
//

class CMusicGeneratorApp : public CWinApp
{
public:
	CMusicGeneratorApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMusicGeneratorApp theApp;