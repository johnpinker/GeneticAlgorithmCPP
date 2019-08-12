/***********************************************************************
Author:		John Pinkerton
File:		BassFitnessDlg.h
Purpose:	Header file for the Windows dialog that sets parameters for the
			BassInstrument fitness function.
***********************************************************************/

#pragma once
#include "afxcmn.h"


// BassFitnessDlg dialog

class BassFitnessDlg : public CDialog
{
	DECLARE_DYNAMIC(BassFitnessDlg)

public:
	BassFitnessDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~BassFitnessDlg();
	BOOL OnInitDialog();

// Dialog Data
	enum { IDD = IDD_BASSFITNESSDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl numNotesSlider;
	CSliderCtrl disonanceSlider;
	CSliderCtrl consonanceSlider;
	BOOL isMajor;
	CSliderCtrl evenAccentSlider;
	CSliderCtrl oddAccentSlider;
};
