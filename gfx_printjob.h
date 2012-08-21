
// gfx_printjob.h

#ifndef _GFX_PRINTJOB_H_
#define _GFX_PRINTJOB_H_


#include "gfx_printunit.h"

#define DELETE_IF_NOT_NULL(p) if(p != NULL){delete p; p = NULL;}

class Printing::GPrintJob;
class Printing::GPrintIndexTree;


// Print() return values
#define PRINTJOB_READY            0
#define PRINTJOB_DIALOGCANCEL     1
#define PRINTJOB_DIALOGNOINIT     2
#define PRINTJOB_NOPRINTTOFILE    3 



// print job flags
#define PJF_ALLOCATEDPD      0x00000001
#define PJF_ENDPAGEPENDING   0x00000002  // start page called, end page wasn't


// selects an index tree as the active tree, use as follows...
//
// INDEXITEM ii;
// ii.pChildren = NULL;
//
// GSELECT_PJINDEXTREE(&ii.pChildren);
//
//...this won't work!!!!....
//
// GPrintIndexTree m_tree;
// GSELECT_PJINDEXTREE(&m_tree);  // error - will compile, but crash at runtime
//
//...use instead....
//
// GPrintIndexTree *m_pTree = new GPrintIndexTree();
// GSELECT_PJINDEXTREE(&m_pTree);
//
#define GSELECT_PJINDEXTREE(it) \
	Printing::GSelectPrintJobIndexTree xx_indextree_xx(this, it);

namespace Printing
{
	// instatiating this class swaps in/out an index tree
	class GSelectPrintJobIndexTree 
	{
	public:
		GSelectPrintJobIndexTree(GPrintJob *pJob, GPrintIndexTree **pTree);
		GSelectPrintJobIndexTree(GPrintUnit *pUnit, GPrintIndexTree **pTree);

	protected:
		GPrintIndexTree **m_pOldTree;
		GPrintJob *m_pJob;

	public:
		virtual ~GSelectPrintJobIndexTree();
	};

	//
	// when printing to a printer, the device names will have names that will
	// look something like...

	//   lpszFileName = "winspool"
	//   lpszDevice = "HP LaserJet 5"
	//   lpszOutput = "LPT1"

	// when printing to a file, the device names will have names that will look like..

	//   lpszFileName = "winspool"
	//   lpszDevice = "HP LaserJet 5"
	//   lpszOutput = "FILE"

	typedef class GDevNames
	{
	public:
		CString strFileName;
		CString strDevice;
		CString strOutput;

	} GDEVNAMES, *LPGDEVNAMES;


	class GPrintJob : public CObject
	{
		DECLARE_DYNAMIC(GPrintJob)

	public:
		GPrintJob();

		// call to print this job..returns one of the "PRINTJOB_..." codes
		int PrintFollowingPrintDialog();
		// will initialize a structure with pointers to all device names
		// these structure values are temporary, and must be stored as 
		// CStrings if you intend to use them later
		void GetDeviceNames(LPGDEVNAMES pDevNames);

		// insert print unit task into the job
		void InsertTask( GPrintUnit* task );
		// evaluate the total pages of the job or the specified unit
		int EvaluateAllUnitPages(CDC* pPreviewDC, int from = 1, int to = 65535);
		int EvaluateOneUnitPages(CDC* pPreviewDC, int unitIndex, int from = 1, int to = 65535);

	public:    
		// return a pointer to the dialog specific to this print job...if you don't override
		// this to create your own, a default will be provided for you.  If you don't want
		// a dialog to appear, override it, call UseDefaults() and return NULL.
		virtual CDialog *CreatePrintDialog();
		// called to give you a chance to initialize the print dialog options before it
		// is displayed, returns a TRUE if initialized ok, FALSE if not
		virtual BOOL InitPrintDialog();
		// called to display the print dialog..will call DoModal() for custom dialogs
		// or CWinApp::DoPrintDialog() for the default dialog...returns the return code
		// of the dialog (IDOK etc.)
		virtual int DoPrintDialog();
		// called to display the "print to file option" if the user checked the print to
		// file option..returns FALSE if the user hit cancel.  'strFileName' is the name
		// of the file to print to
		virtual BOOL DoPrintToFileDialog(CString& strFileName);
		// returns a pointer to the device context to use for printing
		virtual CDC *CreatePrintDC();
		// called when the print job is ready to begin
		virtual void OnBeginPrinting();
		// called when the print job has ended.  'bError' is TRUE if the print ended
		// in error
		virtual void OnEndPrinting(BOOL bError=FALSE);

		// called to ready the printer for the document...'docInfo' contains document
		// information...returns TRUE if we started printing
		virtual BOOL StartDocPrinting(DOCINFO& docInfo);
		// called to end printing the document...'bError' is TRUE if the print ended
		// in error
		virtual void EndDocPrinting(BOOL bError=FALSE);

		// called when the printer dc should be initialized
		virtual void InitPrintDC();
		// called when the CPrintInfo member should be initialize
		virtual void InitPrintInfo();
		// called so derived classes can take user specified margins into account
		virtual BOOL GetPageSetupMargins(CRect& rectMargins);
		// called to initialize the 'docInfo' structure
		virtual void InitDocInfo(DOCINFO& docInfo);

		// index item helpers
		void AddIndexItem(INDEXITEM *pII);

		// call to initialize the job with the default print options...needed if you
		// aren't using the defualt print dialog
		virtual void UseDefaults();

		int Preview(CDC * pPreviewDC, CPrintInfo* info, int from = 1, int to = 65535 );

	protected:
		// returns TRUE if this job is using the default print dialog
		BOOL IsUsingDefaultPrintDialog();
		// return the old CDC
		void SetPreviewPrintDC(CDC* dc, BOOL needPreprocessing = FALSE);
		// do the printing job by using the specified CDC 	
		// return -1 if sth wrong
		// the function will also get the total pages that will print within this job or some units
		// if the unitIndex is -1, means to preview all the units
		// e.g. Preview(pDC, 0, 2,3) means using pDC to print unit 0's page 2 to page 3
		virtual int PreviewOneUnit(CDC * pOriginDC, int unitIndex = 0, BOOL bGetPageOnly = FALSE, int from = 1, int to = 65535 );
		virtual int PreviewAll(CDC * pPreviewDC, int from = 1, int to = 65535 );


	public:
		// print device context
		CDC *m_pDC;
		// print dialog...may be CPrintDialog or not
		CDialog *m_pDialog;
		// print information
		CPrintInfo *m_pInfo;
		// print settings information
		LPPRINTDLG m_pPD;
		// client area (not including margins, headers, and footers) of print dc rect
		CRect m_rectClient;
		// size of the device page, not including margins
		CRect m_rectDevice;
		// point at which we're currently printing
		CPoint m_ptCursor;
		// the size of the original page, including margins, headers, and footers
		CRect m_rectPage;

		// special print job flags
		DWORD m_dwFlags;
		// current index tree
		GPrintIndexTree **m_pActiveTree;

		// name of the port we're printing to
		CString m_strPortName;
		// name of the file we're printing to
		CString m_strPrintToFileName;

		// attributes
		void SetEndPagePending(BOOL bPending=TRUE);
		BOOL IsEndPagePending() const;

	private:
		// remain a list of all the print unit task
		vector<GPrintUnit*> m_vecPrintUnitTasks;
		// the vector contains need pages of each print unit 
		vector<int> m_vecUnitPages;

	public:
		virtual ~GPrintJob();
	};
}

#endif

