// MusicGeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MusicGenerator.h"
#include "MusicGeneratorDlg.h"
#include ".\musicgeneratordlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMusicGeneratorDlg dialog


// Constructor
CMusicGeneratorDlg::CMusicGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMusicGeneratorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	this->ga = NULL;
}


void CMusicGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_POPSIZE, mPopSizeEdit);
	DDX_Control(pDX, IDC_CROSSRATE, mCrossoverEdit);
	DDX_Control(pDX, IDC_MUTRATE, mMutationEdit);
	DDX_Control(pDX, IDC_BRESTART, mStartButton);
	DDX_Control(pDX, IDC_LOGWIN, mLogWin);
	DDX_Control(pDX, IDC_TAB1, mTabCtrl);
	DDX_Control(pDX, IDC_EDIT1, mResolutionText);
	DDX_Control(pDX, IDC_DIFFERENCE, m_difference);
}

BEGIN_MESSAGE_MAP(CMusicGeneratorDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BRESTART, OnBnClickedBrestart)
	ON_EN_KILLFOCUS(IDC_POPSIZE, OnEnKillfocusPopsize)
	ON_EN_KILLFOCUS(IDC_CROSSRATE, OnEnKillfocusCrossrate)
	ON_EN_KILLFOCUS(IDC_MUTRATE, OnEnKillfocusMutrate)
	ON_BN_CLICKED(IDC_BRUN, OnBnClickedBrun)
	ON_BN_CLICKED(IDC_BEXIT, OnBnClickedBexit)
	ON_BN_CLICKED(IDC_BEXPORT, OnBnClickedBexport)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnTcnSelchangeTab1)
	ON_EN_KILLFOCUS(IDC_RESOLUTION, OnEnKillfocusResolution)
END_MESSAGE_MAP()


// CMusicGeneratorDlg message handlers

BOOL CMusicGeneratorDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	this->mPopSizeEdit.SetWindowText("100");
	this->mCrossoverEdit.SetWindowText("0.7");
	this->mMutationEdit.SetWindowText("0.05");
	this->mResolutionText.SetWindowText("4");
	this->mPopSizeEdit.SetFocus();
	TC_ITEM tci;
	tci.mask=TCIF_TEXT;
	tci.iImage=-1;
	tci.pszText="Drum Fitness";
	this->mTabCtrl.InsertItem(0, &tci);
	tci.pszText="Bass Fitness";
	this->drumFitDlg.Create(IDD_DRUMFITNESSDLG, &this->mTabCtrl);
	this->bassFitDlg.Create(IDD_BASSFITNESSDIALOG, &this->mTabCtrl);
	this->mTabCtrl.InsertItem(1, &tci);
	this->ShowFitnessDlg(0);
	return FALSE;  // return TRUE  unless you set the focus to a control
}

// show the specified fitness dialog (0=drum fitness, 1=bass fitness)
void CMusicGeneratorDlg::ShowFitnessDlg(int dlg)
{
	RECT itemRect;
	if (dlg == 0)
	{
		this->mTabCtrl.GetItemRect(0, &itemRect);
		this->drumFitDlg.SetWindowPos(0, itemRect.left+2, itemRect.bottom+2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
		if (this->bassFitDlg.IsWindowVisible() == TRUE)
		{
			this->bassFitDlg.EnableWindow(false);
			this->bassFitDlg.ShowWindow(SW_HIDE);
		}
		this->drumFitDlg.EnableWindow(true);
		this->drumFitDlg.ShowWindow(SW_SHOW);
	}
	else if (dlg ==1)
	{
		this->mTabCtrl.GetItemRect(0, &itemRect);
		this->bassFitDlg.SetWindowPos(0, itemRect.left+2, itemRect.bottom+2, 0, 0, SWP_NOSIZE|SWP_NOZORDER );
		if (this->drumFitDlg.IsWindowVisible() == TRUE)
		{
			this->drumFitDlg.EnableWindow(false);
			this->drumFitDlg.ShowWindow(SW_HIDE);
		}
		this->bassFitDlg.EnableWindow(true);
		this->bassFitDlg.ShowWindow(SW_SHOW);
	}
}

void CMusicGeneratorDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMusicGeneratorDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMusicGeneratorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// start button clicked
void CMusicGeneratorDlg::OnBnClickedBrestart()
{
	char tmp[25];
	if (this->isRunning == true)
	{
		delete this->ga;
		this->ga = NULL;
		this->mStartButton.SetWindowText("Start GA");
		this->mResolutionText.SetReadOnly(false);
		this->mPopSizeEdit.SetReadOnly(false);
		this->isRunning = false;
		return;
	}
	if (this->ga == NULL) {
		this->ga = new GA();
		this->UpdateGAParameters();
		this->ga->SetInstrument(this->BuildInstrument());
		this->ga->InitGA();
		this->ga->Start();
		this->mStartButton.SetWindowText("Stop GA");
		this->mResolutionText.SetReadOnly(true);
		this->mPopSizeEdit.SetReadOnly(true);
		this->mLogWin.SetWindowText("Generation\tBest Fitness\tAverage Fitness\tVariance\r\n");
		this->isRunning = true;
	}
}


void CMusicGeneratorDlg::OnEnKillfocusPopsize()
{
	char tmp[25];
	this->mPopSizeEdit.GetLine(0, tmp, 25);
	int val = atoi(tmp);
	if (val <= 0 || val > 2000) {
		AfxMessageBox("You must enter a number between 1 and 2000.");
		this->mPopSizeEdit.SetWindowText("");
		this->mPopSizeEdit.SetFocus();
	}
}
void CMusicGeneratorDlg::OnEnKillfocusCrossrate()
{
	char tmp[25];
	this->mCrossoverEdit.GetLine(0, tmp, 25);
	float val = (float)atof(tmp);
	if (val <= 0.0 || val > 1.0) {
		AfxMessageBox("You must enter a number between 0 and 1.");
		this->mCrossoverEdit.SetWindowText("");
		this->mCrossoverEdit.SetFocus();
	}
}
void CMusicGeneratorDlg::OnEnKillfocusMutrate()
{
	char tmp[25];
	this->mMutationEdit.GetLine(0, tmp, 25);
	float val = (float)atof(tmp);
	if (val <= 0.0 || val > 1.0) {
		AfxMessageBox("You must enter a number between 0 and 1.");
		this->mMutationEdit.SetWindowText("");
		this->mMutationEdit.SetFocus();
	}
}

// Run the GA one time
void CMusicGeneratorDlg::RunGA() {
	char* logLine;
	logLine = this->ga->RunOnce();
	CString logText;
	CString newLogLine(logLine);
	this->mLogWin.GetWindowText(logText);
	logText += newLogLine;
	this->mLogWin.SetWindowText(logText);
}

// run button clicked
void CMusicGeneratorDlg::OnBnClickedBrun()
{
	this->UpdateGAParameters();
	this->UpdateInstrumentParams();
	for (int i=0; i<10; i++)
	{
		this->RunGA();
	}
	CString logText;
	this->mLogWin.GetWindowText(logText);
	logText += this->ga->PrintBest();
	this->mLogWin.SetWindowText(logText);
}

// exit button clicked
void CMusicGeneratorDlg::OnBnClickedBexit()
{
	delete this->ga;
	this->OnOK();
}

// export button clicked
void CMusicGeneratorDlg::OnBnClickedBexport()
{
	CFileDialog fileDlg(FALSE, "mid", "out.mid", OFN_EXPLORER | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"Midi Files (*.mid)", this, sizeof(OPENFILENAME));
	if (fileDlg.DoModal() == IDOK) {
		this->ga->WriteMidiFile(fileDlg.GetPathName());
	}
}

// Tab changed in fitness tab control
void CMusicGeneratorDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if (this->isRunning == true)
	{
		AfxMessageBox("You must stop the GA before you can change instruments.");
		return;
	}
	this->ShowFitnessDlg(this->mTabCtrl.GetCurSel());
}

// update the ga with the operational parameters
void CMusicGeneratorDlg::UpdateGAParameters(void)
{
	char tmp[25];
	this->mPopSizeEdit.GetLine(0, tmp, 25);
	this->ga->PopulationSize(atoi(tmp));
	this->mCrossoverEdit.GetLine(0, tmp, 25);
	this->ga->CrossoverRate((float)atof(tmp));
	this->mMutationEdit.GetLine(0, tmp, 25);
	this->ga->MutationRate((float)atof(tmp));
	this->ga->includeDifference = this->m_difference.GetCheck() == BST_CHECKED ? true : false;
}

// build a new kit
KitInstrument* CMusicGeneratorDlg::BuildKit(void)
{
	KitInstrument* kit = new KitInstrument();
	char buffer[100];
	this->mResolutionText.GetLine(0, buffer, 100);
	kit->SetResolution(atoi(buffer));
	if (this->mTabCtrl.GetCurSel() == 0) {
		kit->SetKickOnBeatWeight(this->drumFitDlg.kickOnBeatSlider.GetPos());
		kit->SetSnareOnBeatWeight(this->drumFitDlg.snareOnBeatSlider.GetPos());
		kit->SetNumCymbNotesWeight(this->drumFitDlg.numCymbNoteSlider.GetPos());
		kit->SetNumPercNotesWeight(this->drumFitDlg.numPercNoteSlider.GetPos());
	}
	return kit;
}

// update the instrument fitness parameters
void CMusicGeneratorDlg::UpdateInstrumentParams(void)
{
	if (this->mTabCtrl.GetCurSel() == 0) {
		KitInstrument* kit = (KitInstrument*)this->ga->GetInstrument();
		kit->SetKickOnBeatWeight(this->drumFitDlg.kickOnBeatSlider.GetPos());
		kit->SetSnareOnBeatWeight(this->drumFitDlg.snareOnBeatSlider.GetPos());
		kit->SetNumCymbNotesWeight(this->drumFitDlg.numCymbNoteSlider.GetPos());
		kit->SetNumPercNotesWeight(this->drumFitDlg.numPercNoteSlider.GetPos());
		kit->SetAccentSnareWeight(this->drumFitDlg.accSnareCnt.GetPos());
		kit->SetAccentKickWeight(this->drumFitDlg.accentKickCnt.GetPos());
		kit->SetAccentClosedHHWeight(this->drumFitDlg.accentClosedHHCnt.GetPos());
	}
	else if (this->mTabCtrl.GetCurSel() == 1) {
		BassInstrument* bass = (BassInstrument*)this->ga->GetInstrument();
		if (this->bassFitDlg.isMajor == true)
			bass->key = 0;
		else
			bass->key = 1;
		bass->numNotesWeight = this->bassFitDlg.numNotesSlider.GetPos();
		bass->consonanceWeight = this->bassFitDlg.consonanceSlider.GetPos();
		bass->disonanceWeight = this->bassFitDlg.disonanceSlider.GetPos();
		bass->evenAccentWeight = this->bassFitDlg.evenAccentSlider.GetPos();
		bass->oddAccentWeight = this->bassFitDlg.oddAccentSlider.GetPos();
	}
}

void CMusicGeneratorDlg::OnEnKillfocusResolution()
{
	CString resStr;
	this->mResolutionText.GetWindowText(resStr);
	int res = atoi(resStr.GetBuffer());
	if (res%4 != 0 || res >16 || res<0) {
		AfxMessageBox("Resolution must be one of 4, 8 or 16.");
		mResolutionText.SetWindowText("");
		mResolutionText.SetFocus();
	}
}

// build a new BassInstrument
BassInstrument* CMusicGeneratorDlg::BuildBass(void)
{
	BassInstrument* inst = new BassInstrument();
	char buffer[100];
	this->mResolutionText.GetLine(0, buffer, 100);
	inst->SetResolution(atoi(buffer));
	if (this->mTabCtrl.GetCurSel() == 1) {
		if (this->bassFitDlg.isMajor == true)
			inst->key = 0;
		else
			inst->key = 1;
		inst->numNotesWeight = this->bassFitDlg.numNotesSlider.GetPos();
		inst->consonanceWeight = this->bassFitDlg.consonanceSlider.GetPos();
		inst->disonanceWeight = this->bassFitDlg.disonanceSlider.GetPos();
		inst->evenAccentWeight = this->bassFitDlg.evenAccentSlider.GetPos();
		inst->oddAccentWeight = this->bassFitDlg.oddAccentSlider.GetPos();
	}
	return inst;
}

// build and return a new instrument tha matches the type 
// selected in the fitness tab control
InstrumentType* CMusicGeneratorDlg::BuildInstrument(void)
{
	if (this->mTabCtrl.GetCurSel() == 0)
		return this->BuildKit();
	else if (this->mTabCtrl.GetCurSel() ==1)
		return this->BuildBass();
	else
		AfxMessageBox("Undefined tab selected");
}

