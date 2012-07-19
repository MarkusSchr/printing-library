
#include "stdafx.h" 
#include "HSEPrinter.h"



CHSEPrinter::CHSEPrinter()
{
	InitLayout();
	InitPrintGDI();
}

CHSEPrinter::~CHSEPrinter()
{

}

void CHSEPrinter::InitLayout()
{
	m_nPrintPage = 7;

	m_nPage1stHeadLD = 150;
	m_nPage1stHeadTD = 120;	
	m_nPage1stIconHeight = 350;
	m_nPage1stIconWidth = 350;
	m_nTestReportTitleTD = 250;
	m_nTestReportTitleBD = 450;
	m_nPage1stBodyMidLD = 280;
	m_nPage1stBodyDownBD = 320;
	
	m_nFootHeight = 200;

	m_nPage2ndHeadHeight = 220;
	m_nPage2ndLD = 200;

	m_nHeadHeight = 200;	

	m_nPage3rdHeadLD = 80;
	m_nPage3rdIconHeight = 120;
	m_nPage3rdIconWidth = 150;

	m_nSpcTabHD = 150;

}

void CHSEPrinter::InitPrintGDI()
{
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));        
	//lf.lfHeight = -MulDiv(12, GetDeviceCaps(*pDC, LOGPIXELSY), 72);
	lf.lfHeight = 42;                  
	lf.lfWeight = FW_NORMAL;
	lf.lfCharSet = ANSI_CHARSET; 
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH | FF_SWISS; 
	lstrcpy(lf.lfFaceName,_T("Times New Roman"));
	m_LFntTestDepartmentName = lf;	// 宋体 二号78,加粗,//小五 32
	lf.lfHeight = 37;
	lf.lfWeight = FW_NORMAL;
	m_LFntTestDepartmentAddress = lf;	
	lf.lfHeight = 78;
	lf.lfWeight = FW_BOLD;
	m_LFntTestReportTitle = lf;
	lf.lfHeight = 53;
	lf.lfWeight = FW_NORMAL;
	m_LFntPage1stBodyMid = lf;		
	lf.lfHeight = 56;
	lf.lfWeight = FW_BOLD;
	m_LFntPage1stBodyDown = lf;
	lf.lfHeight = 32;
	lf.lfWeight = FW_NORMAL;
	m_LFntFoot = lf;
	lf.lfHeight = 56;
	lf.lfWeight = FW_NORMAL;
	m_LFntPage2ndTabCaption = lf;
	lf.lfHeight = 37;
	lf.lfWeight = FW_NORMAL;
	//lf.lfEscapement = 2700;
	m_LFntPage2ndTabContent = lf;
	//lf.lfEscapement = 0;
	lf.lfHeight = 37;
	lf.lfWeight = FW_NORMAL;
	m_LFntHeadCaption = lf;
	lf.lfHeight = 32;
	lf.lfWeight = FW_NORMAL;
	m_LFntHeadItems = lf;
	lf.lfHeight = 49;
	lf.lfWeight = FW_NORMAL;
	m_LFntPage3rdTitle = lf;
	lf.lfHeight = 42;
	lf.lfWeight = FW_NORMAL;
	m_LFntPage4thTabCaption = lf;
	lf.lfHeight = 37;
	lf.lfWeight = FW_NORMAL;
	m_LFntPage4thTabContent = lf;
}

void CHSEPrinter::PreparePrinting(CPrintInfo* pInfo)
{
	//CDC dcPrint;
	//dcPrint.Attach(pInfo->m_pPD->m_pd.hDC);
	//PrePareGDI(&dcPrint,pInfo);
	//FormatDataPages(&dcPrint,pInfo);
	//SetMaxPrintPage(pInfo,3+m_vDataPages.size());
	//dcPrint.Detach();

	SetMaxPrintPage(pInfo,m_nPrintPage);
}

void CHSEPrinter::PrePareGDI(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	m_fntTestDepartmentName.CreateFontIndirect(&m_LFntTestDepartmentName);	// 宋体 二号78,加粗,//小五 32
	m_fntTestDepartmentAddress.CreateFontIndirect(&m_LFntTestDepartmentAddress);	
	m_fntTestReportTitle.CreateFontIndirect(&m_LFntTestReportTitle);;
	m_fntPage1stBodyMid.CreateFontIndirect(&m_LFntPage1stBodyMid);		
	m_fntPage1stBodyDown.CreateFontIndirect(&m_LFntPage1stBodyDown);
	m_fntFoot.CreateFontIndirect(&m_LFntFoot);
	m_fntPage2ndTabCaption.CreateFontIndirect(&m_LFntPage2ndTabCaption);
	m_fntPage2ndTabContent.CreateFontIndirect(&m_LFntPage2ndTabContent);
	m_fntHeadCaption.CreateFontIndirect(&m_LFntHeadCaption);;
	m_fntHeadItems.CreateFontIndirect(&m_LFntHeadItems);
	m_fntPage3rdTitle.CreateFontIndirect(&m_LFntPage3rdTitle);
	m_fntPage4thTabCaption.CreateFontIndirect(&m_LFntPage4thTabCaption);
	m_fntPage4thTabContent.CreateFontIndirect(&m_LFntPage4thTabContent);


	m_penHeadFoot.CreatePen(PS_SOLID,4,RGB(0,0,0));
	m_penTabLine.CreatePen(PS_SOLID,1,RGB(0,0,0));
}

void CHSEPrinter::BeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	PrePareGDI(pDC,pInfo);
}

void CHSEPrinter::PrepareDC(CDC* pDC, CPrintInfo* pInfo)
{
	if(NULL == pInfo)
		return;

	pDC->SetMapMode(MM_LOMETRIC);
	pDC->SetBkMode(TRANSPARENT);	
}

void CHSEPrinter::Print(CDC* pDC, CPrintInfo* pInfo)
{
	static int i = 1;

	if(3 <= pInfo->m_nCurPage)
		PrintHead(pDC,pInfo);

	PrintPage1st(pDC,pInfo);

	PrintPageFoot(pDC,pInfo);

	// ATTENTION: we have added a page, so we should add the current page
	pInfo->m_nCurPage++;
}

void CHSEPrinter::EndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	m_fntTestDepartmentName.DeleteObject();
	m_fntTestDepartmentAddress.DeleteObject();
	m_fntTestReportTitle.DeleteObject();
	m_fntPage1stBodyMid.DeleteObject();
	m_fntPage1stBodyDown.DeleteObject();

	m_fntFoot.DeleteObject();
	m_penHeadFoot.DeleteObject();

	m_fntPage2ndTabCaption.DeleteObject();
	m_fntPage2ndTabContent.DeleteObject();
	m_penTabLine.DeleteObject();

	m_fntHeadCaption.DeleteObject();
	m_fntHeadItems.DeleteObject();

	m_fntPage3rdTitle.DeleteObject();

	m_fntPage4thTabCaption.DeleteObject();
	m_fntPage4thTabContent.DeleteObject();
}

void CHSEPrinter::SetMaxPrintPage(CPrintInfo* pInfo, int num )
{
	if(num!=m_nPrintPage)
		m_nPrintPage = num;
	pInfo->SetMaxPage(m_nPrintPage);
}

void CHSEPrinter::PrintIcon(CDC* pDC,CPrintInfo* pInfo, CRect& rtIC)
{
	if (m_sPage1stIconPath.IsEmpty())
		return;
	if(1 == pInfo->m_nCurPage)
	{
		if(!m_imgPage1stIcon.IsNull())
			m_imgPage1stIcon.Detach();
		m_imgPage1stIcon.Load(m_sPage1stIconPath);
	}
	if(m_imgPage1stIcon.IsNull())
		return;
	float nw = (float)rtIC.Height()/m_imgPage1stIcon.GetWidth();
	float nh = (float)rtIC.Width()/m_imgPage1stIcon.GetHeight();
	nw  = (nw<0) ? -nw : nw ;
	nh  = (nh<0) ? -nh : nh ;
	float nk = (nw<nh) ? nw : nh;
	CPoint ptct = rtIC.CenterPoint();
	int nW = int(m_imgPage1stIcon.GetWidth() * nk + 0.5f);
	int nH = int(m_imgPage1stIcon.GetHeight() * nk + 0.5f);
	//rtIC.left = ptct.x - nW/2;
	rtIC.right = rtIC.left + nW;
	//rtIC.top = ptct.y + nH/2;
	rtIC.bottom = rtIC.top - nH;

	pDC->LPtoDP(&rtIC);
	rtIC.NormalizeRect();
	int nOldMapMode = pDC->SetMapMode(MM_TEXT);
	m_imgPage1stIcon.Draw(pDC->GetSafeHdc(),rtIC.left,rtIC.top,rtIC.Width(),rtIC.Height(),0,0,m_imgPage1stIcon.GetWidth(),m_imgPage1stIcon.GetHeight());
	pDC->SetMapMode(nOldMapMode);
	pDC->DPtoLP(&rtIC);
}

void DrawSingleLineText(CDC* pDC,CFont* fnt,CString const s,CRect& rect,bool center = true,float hscl=1.3f)
{
	CFont* pOldFont = 0;
	if( fnt )
		pDC->SelectObject(fnt);
	TEXTMETRIC ttmc;
	pDC->GetTextMetrics(&ttmc);

	int dirY = 1;
	if(MM_TEXT  == pDC->GetMapMode())
		dirY = -1;
	rect.bottom = rect.top - dirY*int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*hscl +0.5f);
	CRect rectT = rect;
	
	rectT.DeflateRect(ttmc.tmHeight/2,0);
	
	DWORD dwFmt = DT_SINGLELINE|DT_VCENTER;
	if(center)
		dwFmt |= DT_CENTER;
	else
		dwFmt |= DT_LEFT;

	pDC->DrawText(s,rectT,dwFmt);
	if( fnt )
		pDC->SelectObject(pOldFont);
}

void CHSEPrinter::PrintPage1st(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcPage = pInfo->m_rectDraw;
	CRect rect,rect1;
	rect.left = rcPage.left + m_nPage1stHeadLD;
	rect.right = rcPage.right - m_nPage1stHeadLD;
	rect.top = rcPage.top - m_nPage1stHeadTD;
	rect.bottom = rect.top - m_nPage1stIconHeight;
	//pDC->SetMapMode(MM_LOMETRIC);
	//pDC->DPtoLP(&rcPage);
	//int nHeadLeft = rcPage.left + m_nPage1stHeadLD;

	{
		CRect rtIC = rect;
		rtIC.bottom = rtIC.top - m_nPage1stIconWidth;
		rtIC.right = rtIC.left + m_nPage1stIconHeight;
		PrintIcon(pDC,pInfo,rtIC);
	}

	//if (!m_sPage1stIconPath.IsEmpty())
	//{
	//	if(!m_imgPage1stIcon.IsNull())
	//		m_imgPage1stIcon.Detach();
	//	m_imgPage1stIcon.Load(m_sPage1stIconPath);
	//	float nw = (float)m_nPage1stIconWidth/m_imgPage1stIcon.GetWidth();
	//	float nh = (float)m_nPage1stIconHeight/m_imgPage1stIcon.GetHeight();
	//	float nk = (nw<nh) ? nw : nh;
	//	rect1 = rect;
	//	rect1.right = rect1.left + int(m_imgPage1stIcon.GetWidth() * nk + 0.5f);
	//	rect1.bottom = rect1.top - int(m_imgPage1stIcon.GetHeight() * nk + 0.5f);
	//	pDC->LPtoDP(&rect1);
	//	rect1.NormalizeRect();
	//	int nOldMapMode = pDC->SetMapMode(MM_TEXT);
	//	m_imgPage1stIcon.Draw(pDC->GetSafeHdc(),rect1.left,rect1.top,rect1.Width(),rect1.Height(),0,0,m_imgPage1stIcon.GetWidth(),m_imgPage1stIcon.GetHeight());
	//	pDC->SetMapMode(nOldMapMode);		
	//}

	int nPage1stHeadTD0 = 20;
	rect1 = rect;
	rect1.top = rect1.top  ;
	rect1.left = rect.left + m_nPage1stIconWidth;
	DrawSingleLineText(pDC,&m_fntTestDepartmentName,m_sTestDepartmentName,rect1,false,1.5f);

	{
		CString sname[] ={_T("电话： "),_T("传真： "),_T("Email： "),_T("地址： ")} ;
		CString str[] = { m_sTestDepartmentTelephone,m_sTestDepartmentFax,m_sTestDepartmentEmail,m_sTestDepartmentAddress};
		for (int i=0;i<_countof(str); i++)
		{
			rect1.top = rect1.bottom;
			DrawSingleLineText(pDC,&m_fntTestDepartmentAddress,sname[i]+str[i],rect1,false,1.3f);
		}
	}
	
	rect1 = rect;
	rect1.top = rect1.bottom - m_nTestReportTitleTD;
	DrawSingleLineText(pDC,&m_fntTestReportTitle,m_sTestReportTitle,rect1,true,1.3f);
	
	rect1.bottom -=  m_nTestReportTitleBD;
	{
		CString sname[] ={_T("送样单位： "),_T("样品名称： "),_T("执行标准： "),_T("测试日期： "),_T("测试结果： "),_T("备    注： ")} ;
		CString str[] = { m_sWorkShop,m_sProductorID,m_sStandardName,m_sTestData,m_sResult,m_sRemark};
		rect1.left = rcPage.left + m_nPage1stBodyMidLD;
		rect1.right = rcPage.right - m_nPage1stBodyMidLD;
		for (int i=0;i<_countof(str); i++)
		{
			rect1.top = rect1.bottom;
			DrawSingleLineText(pDC,&m_fntPage1stBodyMid,sname[i]+str[i],rect1,false,1.8f);
		}
	}

	{
		CFont* pOldFont = NULL;
		TEXTMETRIC ttmc;

		CString sname[] = {_T("测试： "), _T("审核： "),_T("批准： ")};
		CString str = _T("_");
		pOldFont = pDC->SelectObject(&m_fntPage1stBodyDown);	pDC->GetTextMetrics(&ttmc);
		CSize sz = pDC->GetTextExtent(sname[0]);
		CSize sz0 = pDC->GetTextExtent(str);
		int nw = int((rcPage.Width()- 2*m_nPage1stBodyMidLD - sz.cx*_countof(sname))/(float)_countof(sname))/sz0.cx;
		ASSERT(0 < nw);	
		rect1.top = rcPage.bottom + m_nFootHeight + m_nPage1stBodyDownBD;
		rect1.bottom = rect1.top - int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*1.5f +0.5f);
		CString str0 = _T("");
		for (int i=0;i<_countof(sname); i++)
		{
			str0 += sname[i] ;
			for (int j=0; j<nw-1; j++)
				str0 += str ;
			str0 += _T(" ");
		}
		pDC->DrawText(str0,rect1,DT_SINGLELINE|DT_LEFT|DT_TOP);
		//DrawSingleLineText(pDC,&m_fntPage1stBodyDown,str0,rect1,false,1.5f);
		pDC->SelectObject(pOldFont);
	}
}

void CHSEPrinter::PrintPageFoot(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcPage = pInfo->m_rectDraw;
	CRect rect1 = rcPage;
	if(3> pInfo->m_nCurPage)
	{
		rect1.left = rcPage.left + m_nPage1stHeadLD;
		rect1.right = rcPage.right - m_nPage1stHeadLD;
	}
	else
	{
		rect1.left = rcPage.left + m_nPage3rdHeadLD;
		rect1.right = rcPage.right - m_nPage3rdHeadLD;
	}
	rect1.top = rcPage.bottom + m_nFootHeight - 20;

	CFont* pOldFont = NULL;
	TEXTMETRIC ttmc;
	
	CString str = m_sSoftwareNameEn + _T(" ") + m_sSoftwareVersion + _T(" ") + m_sSoftwareNameCn;
	pOldFont = pDC->SelectObject(&m_fntFoot);	pDC->GetTextMetrics(&ttmc);
	pDC->DrawText(str,rect1,DT_SINGLELINE|DT_LEFT|DT_TOP);
	str.Format(_T("第 %d 页 共 %d 页"),pInfo->m_nCurPage,pInfo->GetMaxPage());
	pDC->DrawText(str,rect1,DT_SINGLELINE|DT_RIGHT|DT_TOP);
	pDC->SelectObject(pOldFont);

	CPen *pOldPen = pDC->SelectObject(&m_penHeadFoot);
	pDC->MoveTo(rect1.left,rcPage.bottom + m_nFootHeight - 5);
	pDC->LineTo(rect1.right,rcPage.bottom + m_nFootHeight - 5);
	pDC->SelectObject(pOldPen);

}

void CHSEPrinter::PrintPage2nd(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcPage = pInfo->m_rectDraw;

	CString sDefault = _T(" ");

	SPrintTable sTable;
	vector<CString> vstr;
	{
		for (int i=0; i<m_saRFNATestItemConfigDescribe.GetCount();i++)
		{
			if(0 == i)
				vstr.push_back(m_sRFNAInfo_Name);
			else
				vstr.push_back(_T(""));
			vstr.push_back(m_saRFNATestItemConfigDescribe.GetAt(i));
		}
		for (int i=0; i<m_saTDRTestItemConfigDescribe.GetCount();i++)
		{
			if(0 == i)
				vstr.push_back(m_sTDRInfo_Name);
			else
				vstr.push_back(_T(""));
			vstr.push_back(m_saTDRTestItemConfigDescribe.GetAt(i));
		}

		sTable.title = _T("测试设备信息：");
		sTable.vhead.push_back(_T("设备型号"));
		sTable.vhead.push_back(_T("配置"));
		sTable.vbody = vstr;

		int wdth = rcPage.Width() - 2*m_nPage2ndLD;
		int wdth01 = int(wdth*0.2f+0.5f);
		sTable.vhdcellwd.push_back(wdth01);
		sTable.vhdcellwd.push_back(wdth-wdth01);
		sTable.tableft = rcPage.left + m_nPage2ndLD;
		sTable.tabwidth = rcPage.Width() - 2*m_nPage2ndLD;
		sTable.tabtop = rcPage.top - m_nPage2ndHeadHeight;

		sTable.title_fnt = &m_fntPage2ndTabCaption;
		sTable.content_fnt = &m_fntPage2ndTabContent;
		sTable.tabline_pen = &m_penTabLine;
		//sTable.isContentCenter = false;
		//sTable.isFrstLinePrt = true;
		//sTable.titleHeightScl = 1.5f;
		//sTable.contentHeightScl = 1.5f;
	}
	int nY;
	PrintSingleTable(pDC,pInfo,sTable,nY);	

	int ndispace = rcPage.top - rcPage.bottom - m_nPage2ndHeadHeight- m_nFootHeight-3*90 -(m_saTestResultList.GetCount()+10)*57;		//估计Page2表格间距离
	ndispace /= 3;
	ndispace = (ndispace >250) ? 250 :ndispace;
	ndispace = (ndispace < 0) ? 2 :ndispace;
	{
		sTable.title = _T("测试信息：");
		sTable.vhead.clear();
		sTable.vbody.clear();

		sTable.vhdcellwd.clear();
		CString str0[]={_T("测试人员："),_T("电缆标识："),_T("环境温度："),_T("电缆长度："),
			_T("环境湿度："),_T("产品盘号："),_T("记录文件："),_T("产品批号：")};
		CString str[] = {m_sOperator, m_sCableLengthID,m_sTemperatureInCentigrade,m_sCableLengthInm,
			m_sHumidity, m_sProductPlateID,m_sRecordFilename,m_sProductBatchID};
		for (int i= 0; i<_countof(str); i++)
			sTable.vbody.push_back(str0[i]+str[i]);

		sTable.tabtop = nY - ndispace ;
	}
	PrintSingleTable(pDC,pInfo,sTable,nY);

	{
		sTable.title = _T("项目信息：");
		sTable.vhead.clear();
		CString str0[]={_T("序号"),_T("项目名称"),_T("单位"),_T("测试结果")};
		for (int i= 0; i<_countof(str0); i++)
			sTable.vhead.push_back(str0[i]);

		sTable.vbody.clear();
		CString strtmp;
		for (int i = 0; i<m_saTestResultList.GetCount();i++)
		{
			for (size_t j = 0; j<sTable.vhead.size(); j++)
			{
				if(0 == j)
					strtmp.Format(_T("%d"),i+1);
				else if(1 == j)
					strtmp = m_saTestResultList.GetAt(i);
				else if(2 == j)
					strtmp = _T("");
				else if(3 == j)
					strtmp = _T("合格");
				sTable.vbody.push_back(strtmp);
			}
		}

		sTable.vhdcellwd.clear();
		float wd01[]={0.1f, 0.5f, 0.28f,0.12f};
		for (int i= 0; i<_countof(wd01); i++)
			sTable.vhdcellwd.push_back(wd01[i]*sTable.tabwidth);

		sTable.tabtop = nY - ndispace ;
	}
	PrintSingleTable(pDC,pInfo,sTable,nY);
}

bool CHSEPrinter::GetTabHeight(CDC* pDC/*,CPrintInfo* pInfo*/,SPrintTable sTab,int& nH,int topheadline)
{
	CFont* pOldFont = 0;
	TEXTMETRIC ttmc;

	int nh = 0;
	if(!sTab.title.IsEmpty())
	{
		pDC->SelectObject(sTab.title_fnt);
		pDC->GetTextMetrics(&ttmc);
		pDC->SelectObject(pOldFont);
		nh += int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*sTab.titleHeightScl+0.5f);
	}

	if(sTab.vhead.size()>0)
	{
		pDC->SelectObject(sTab.content_fnt);
		pDC->GetTextMetrics(&ttmc);
		pDC->SelectObject(pOldFont);

		//if(topheadline>0)
		//nH0 = nh+int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*sTab.contentHeightScl+0.5f);

		int nht0 = int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*sTab.contentHeightScl+0.5f);
		int crow = sTab.vbody.size() + sTab.vhead.size();
		if(crow>0)
			nh += (crow/sTab.vhead.size() + topheadline) * nht0;
	}

	nH = nh;
	//if(sTab.tabtop-nh < pInfo->m_rectDraw.bottom+m_nFootHeight)
	//	return false;

	return true;
}

void CHSEPrinter::GetTRowHeight(CDC* pDC,SPrintTable sTab,int& nH0,int& nH1)
{
	CFont* pOldFont = 0;
	TEXTMETRIC ttmc;

	pDC->SelectObject(sTab.title_fnt);
	pDC->GetTextMetrics(&ttmc);
	pDC->SelectObject(pOldFont);
	nH0 = int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*sTab.titleHeightScl+0.5f);

	pDC->SelectObject(sTab.content_fnt);
	pDC->GetTextMetrics(&ttmc);
	pDC->SelectObject(pOldFont);
	nH1 = int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*sTab.titleHeightScl+0.5f);
}

bool CHSEPrinter::PrintSingleTable(CDC* pDC, CPrintInfo* pInfo, SPrintTable sTab,int& nY )
{
	//CFont* pOldFont = NULL;
	CPen* pOldPen = NULL;
	//TEXTMETRIC ttmc;

	int dirY = 1;
	if(MM_TEXT  == pDC->GetMapMode())
		dirY = -1;

	CRect rect,rect1,rect2;
	rect.left = sTab.tableft;
	rect.right = rect.left + sTab.tabwidth;
	rect.top = rect.bottom =  sTab.tabtop;
	if(!sTab.title.IsEmpty())
		DrawSingleLineText(pDC,sTab.title_fnt,sTab.title,rect,false,sTab.titleHeightScl);

	pOldPen = pDC->SelectObject(sTab.tabline_pen);
	rect.OffsetRect(0,rect.Height());	
	rect2 = rect;
	int ntop = rect.top;	
	
	vector<CString> vstr ;
	vstr = sTab.vhead;
	for (size_t i= 0; i<sTab.vbody.size();i++)
		vstr.push_back(sTab.vbody[i]);
	int ncol = (int)sTab.vhead.size();
	if(0 == ncol)
	{
		ncol = 2;
		sTab.vhdcellwd.push_back(sTab.tabwidth/2);
		sTab.vhdcellwd.push_back(sTab.tabwidth-sTab.tabwidth/2);
		if(0 != vstr.size()%ncol)
			for (int i=0; i<(int)vstr.size()%ncol;i++)
				vstr.push_back(_T(""));
	}
	int nline = (int)vstr.size()/ncol;

	float scl = 0;
	for (int i = 0; i<nline; i++)
	{
		//////////////////////////////////////////////////////////////////////////
		pDC->MoveTo(rect2.TopLeft());	pDC->LineTo(rect2.right,rect2.top);
		rect1 = rect2;
		for (int j = 0; j<ncol; j++)
		{
			if(0 != j)
				rect1.left = rect1.right;
			rect1.right = rect1.left + sTab.vhdcellwd[j] ;
			//pDC->DrawText(vstr[ncol*i+j],rect1,DT_SINGLELINE|DT_LEFT|DT_VCENTER);/*DT_LEFT|DT_WORDBREAK|DT_EDITCONTROL|DT_VCENTER*/
			DrawSingleLineText(pDC,sTab.content_fnt,vstr[ncol*i+j],rect1,sTab.isContentCenter,sTab.contentHeightScl);
			if(0==i&&0==j)
				rect2.bottom = rect1.bottom;
		}
		rect2.OffsetRect(0,rect2.Height());	
	}
	pDC->MoveTo(rect2.TopLeft());	pDC->LineTo(rect2.right,rect2.top);	
	int nb = 0,nl = rect.left;
	for (int i = 0; i<ncol; i++)
	{
		nb = (0==i) ? 0 : sTab.vhdcellwd[i-1];
		nl += nb;
		pDC->MoveTo(nl, ntop);	
		pDC->LineTo(nl, rect2.top);
	}
	pDC->MoveTo(rect.right, ntop);	
	pDC->LineTo(rect.right, rect2.top);

	pDC->SelectObject(pOldPen);	
	nY = rect2.top;
	return true;
}

void CHSEPrinter::PrintHead(CDC* pDC,CPrintInfo* pInfo)
{
	CRect rcPage = pInfo->m_rectDraw;
	CRect rect,rect1;
	rect.left = rcPage.left + m_nPage3rdHeadLD;
	rect.right = rcPage.right - m_nPage3rdHeadLD;
	rect.top = rcPage.top - (m_nHeadHeight-m_nPage3rdIconHeight);
	rect.bottom = rect.top - m_nPage3rdIconHeight;

	CPen *pOldPen = pDC->SelectObject(&m_penHeadFoot);
	pDC->MoveTo(rect.left,rect.bottom);
	pDC->LineTo(rect.right,rect.bottom);
	pDC->SelectObject(pOldPen);


	CRect rtIC = rect;
	rtIC.bottom = rtIC.top - m_nPage3rdIconHeight;
	rtIC.right = rtIC.left + m_nPage3rdIconWidth;
	PrintIcon(pDC,pInfo,rtIC);

	CFont* pOldFont = NULL;
	TEXTMETRIC ttmc;
	{
		int nleft = 100, ndispace;
		ndispace = (rtIC.top- rtIC.bottom- 62-50)/2;
		pOldFont = pDC->SelectObject(&m_fntHeadCaption);	pDC->GetTextMetrics(&ttmc);
		rect1 = rect;
		rect1.top = rect1.top - ndispace ;
		rect1.bottom = rect1.top - int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*1.5f +0.5f);
		rect1.left = rtIC.right + nleft;
		rect1.right = rect.right - nleft;
		pDC->DrawText(m_sTestDepartmentName,rect1,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		pDC->SelectObject(pOldFont);
	}
	{	
		CString str[] ={_T("测试标准："),_T("记录文件：")};
		pOldFont = pDC->SelectObject(&m_fntHeadItems);	pDC->GetTextMetrics(&ttmc);
		rect1.top = rect1.bottom;
		rect1.bottom = rect1.top - int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*1.3f +0.5f);
		rect1.right = rect1.left + rect1.Width()/2;
		pDC->DrawText(str[0]+m_sStandardName,rect1,DT_SINGLELINE|DT_CENTER|DT_VCENTER);

		rect1.OffsetRect(rect1.Width(),0);
		pDC->DrawText(str[1]+m_sRecordFilename,rect1,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
		pDC->SelectObject(pOldFont);
	}
}

void CHSEPrinter::PrintPageTitle(CDC* pDC,CString str,CRect& rect)
{
	CFont* pOldFont = NULL;
	TEXTMETRIC ttmc;

	pOldFont = pDC->SelectObject(&m_fntPage3rdTitle);	pDC->GetTextMetrics(&ttmc);
	rect.bottom = rect.top - int((ttmc.tmHeight + ttmc.tmExternalLeading * 2)*1.8f +0.5f);
	pDC->DrawText(str,rect,DT_SINGLELINE|DT_CENTER|DT_VCENTER);
	pDC->SelectObject(pOldFont);
}

void CHSEPrinter::PrintPage3rd(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcPage = pInfo->m_rectDraw;
	CRect rect,rect1;
	rect.left = rcPage.left + m_nPage3rdHeadLD;
	rect.right = rcPage.right - m_nPage3rdHeadLD;
	rect.top = rcPage.top - m_nHeadHeight;
	rect.bottom = rect.top - 100;

	PrintPageTitle(pDC,_T("汇总报告(图形)"),rect);

	rect.top = rect.bottom;
	rect.bottom = rcPage.bottom + m_nFootHeight;
	rect1 = rect;

	int nCnt = 6, nrow, ncol;
	if(4 == nCnt)
	{
		nrow = 2;
		ncol = 2;
	}
	else if(6 == nCnt)
	{
		nrow = 3;
		ncol = 2;
	}
	else if(9 == nCnt)
	{
		nrow = 3;
		ncol = 3;
	}
	int nH = (rect1.Height()/nrow)>0 ? (rect1.Height()/nrow) :-(rect1.Height()/nrow);
	rect1.right = rect1.left + rect1.Width()/ncol;
	rect1.bottom = rect1.top - nH;

	CPen* pOldPen = NULL;
	pOldPen = pDC->SelectObject(&m_penTabLine);
	for (int i=0; i<nrow; i++)
	{
		for (int j=0; j<ncol; j++)
		{
			pDC->Rectangle(rect1);;
			if(ncol -1 != j)
				rect1.OffsetRect(rect1.Width(),0);
			else
				rect1.OffsetRect(-rect1.Width()*(ncol-1),-nH);			
		}
	}
	pDC->SelectObject(pOldPen);
}

void CHSEPrinter::FormatPointPairTableString(int nBpt,int nCnt, SPrintTable& sTable)
{
	CString str;
	int nrow = 5;

	sTable.vhead.clear();
	sTable.vbody.clear();

	sTable.vhead.push_back(_T("频率[MHz]"));
	for (int i=nBpt; i<nBpt+nCnt; i++)
	{
		str.Format(_T("%g"),(i-1)*50+3.0f*(rand()/(float)RAND_MAX));
		sTable.vhead.push_back(str);
	}
	sTable.vhead.push_back(_T("结果"));

	sTable.vbody.push_back(_T("标准数据"));
	for (int i=nBpt; i<nBpt+nCnt; i++)
	{
		str.Format(_T("≤%g"),(i+1)*3.5+1.0f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);
	}
	sTable.vbody.push_back(_T("--"));
	for (int i=0 ;i<nrow; i++)
	{
		str.Format(_T("线对%d"),(i+1));
		sTable.vbody.push_back(str);
		for (int j=nBpt;j<nBpt+nCnt; j++)
		{
			str.Format(_T("%g"),(i+1)*2.3+0.8f*(rand()/(float)RAND_MAX));
			sTable.vbody.push_back(str);
		}
		sTable.vbody.push_back(_T("合格"));
	}
}

void CHSEPrinter::FormatPointSingleTableString(int nBpt,int nCnt, SPrintTable& sTable)
{
	CString str;
	int nrow = 5;

	sTable.vhead.clear();
	sTable.vbody.clear();
	CString vstr[]={_T("线对"),_T("标准数据"),_T("测试数据"),_T("结果")};
	for (int i=0; i<_countof(vstr); i++)
		sTable.vhead.push_back(vstr[i]);

	for (int i=nBpt; i<nBpt+nCnt; i++)
	{
		str.Format(_T("线对 %d"),(i+1));
		sTable.vbody.push_back(str);
		
		str.Format(_T("≤%g"),(i+1)*3.5+1.0f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);

		str.Format(_T("≤%g"),(i+1)*1.5+1.3f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);

		sTable.vbody.push_back(_T("合格"));
	}
}

void CHSEPrinter::FormatPointSingleTableMMString(int nBpt,int nCnt, SPrintTable& sTable)
{
	CString str;
	int nrow = 5;

	sTable.vhead.clear();
	sTable.vbody.clear();
	CString vstr[]={_T("线对"),_T("标准数据"),_T("最大值"),_T("位置"),_T("最小值"),_T("位置"),_T("平均值"),_T("结果")};
	for (int i=0; i<_countof(vstr); i++)
		sTable.vhead.push_back(vstr[i]);

	for (int i=nBpt; i<nBpt+nCnt; i++)
	{
		str.Format(_T("线对 %d"),(i+1));
		sTable.vbody.push_back(str);

		str.Format(_T("%f-%f"),i+1.2,(i+1)*3.5+1.0f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);

		str.Format(_T("%g"),(i+3.5)*1.6+1.3f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);
		sTable.vbody.push_back(_T(""));

		str.Format(_T("%g"),(i+1.3)*1.6+1.3f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);
		sTable.vbody.push_back(_T(""));
	
		str.Format(_T("%g"),(i+2.5)*1.6+1.3f*(rand()/(float)RAND_MAX));
		sTable.vbody.push_back(str);

		sTable.vbody.push_back(_T("合格"));
	}
}

void CHSEPrinter::FormatTableWidth_Col(CDC* pDC,CString mcont,int allcol,int& maxcol,SPrintTable& sTable,int cellcnt)
{
	sTable.vhdcellwd.clear();
	int rlwdith;
	CSize  sz0,sz1,sz;

	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(sTable.content_fnt);
	TEXTMETRIC ttmc;
	pDC->GetTextMetrics(&ttmc);

	sz0 = pDC->GetTextExtent(_T("不合格"));
	sz1 = pDC->GetTextExtent(_T("频率[MHz]"));
	sz = pDC->GetTextExtent(mcont);

	pDC->SelectObject(pOldFont);

	maxcol = (sTable.tabwidth-(sz1.cx+ttmc.tmHeight)-(sz0.cx+ttmc.tmHeight))/(sz.cx+ttmc.tmHeight);
	maxcol = maxcol/cellcnt*cellcnt;

	rlwdith = (sz.cx+ttmc.tmHeight)*maxcol + (sz1.cx+ttmc.tmHeight)+(sz0.cx+ttmc.tmHeight);
	
	float extspc = 0.0f;
	if(maxcol <= allcol)
		extspc = (sTable.tabwidth-rlwdith)/(maxcol+2.0f);
	else
		sTable.tabwidth = rlwdith;	

	sTable.vhdcellwd.push_back(sz1.cx+ttmc.tmHeight+extspc);
	for (int i=0; i<maxcol; i++)
		sTable.vhdcellwd.push_back((sz.cx+ttmc.tmHeight+extspc));

	sTable.vhdcellwd.push_back((sz0.cx+ttmc.tmHeight+extspc));
}

void CHSEPrinter::FormatTableWidth_Col(int curcol,int & tabwidth,vector<int>& vhw)
{
	vector<int> tmp = vhw;
	vhw.clear();

	int owdth = tabwidth;
	tabwidth = (tmp[0] + curcol*tmp[1]+tmp[tmp.size()-1]);

	vhw.push_back(tmp[0]);
	for (int i=0; i<curcol; i++)
		vhw.push_back(tmp[1]);
	vhw.push_back(tmp[tmp.size()-1]);

	//for (size_t i=0;i<vhw.size();i++)
	//	vhw[i] *=((float)owdth/tabwidth);
}

void CHSEPrinter::CombineTableCellWidth_Col(int cellcnt,vector<int>& vhw)
{
	vector<int> tmp = vhw;
	vhw.clear();

	vhw.push_back(tmp[0]);
	for (int i=0; i<(tmp.size()-2)/cellcnt; i++)
		vhw.push_back(tmp[1]*cellcnt);

	vhw.push_back(tmp[tmp.size()-1]);
}

void CHSEPrinter::FormatTableWidth_Row(CDC* pDC,CString mcont,int allcol,SPrintTable& sTable)
{
	sTable.vhdcellwd.clear();
	int rlwdith;
	CSize  sz0,sz1,sz;

	CFont* pOldFont = NULL;
	pOldFont = pDC->SelectObject(sTable.content_fnt);
	TEXTMETRIC ttmc;
	pDC->GetTextMetrics(&ttmc);

	sz0 = pDC->GetTextExtent(_T("不合格"));
	sz1 = pDC->GetTextExtent(_T("线对 100"));
	sz = pDC->GetTextExtent(mcont);
	pDC->SelectObject(pOldFont);

	sTable.vhdcellwd.push_back(sz1.cx+ttmc.tmHeight);
	for (int i=0; i<allcol; i++)
		sTable.vhdcellwd.push_back((sz.cx+ttmc.tmHeight));

	sTable.vhdcellwd.push_back((sz0.cx+ttmc.tmHeight));

	int ncurwd = sTable.vhdcellwd[0] + allcol*sTable.vhdcellwd[1]+(sz0.cx+ttmc.tmHeight);

	float scl = (ncurwd-sTable.tabwidth)/(float)sTable.tabwidth;
	if(scl>0)
	{
//////////////////////////////////////////////////////////////////////////
	}
	else if( scl <-0.5f)
	{
		sTable.tabwidth = ncurwd;
		return;
	}
	int nwd = 0 ; 
	for (int i=0; i<allcol+1;i++)
	{
		sTable.vhdcellwd[i] *=((float)sTable.tabwidth/ncurwd);
		nwd += sTable.vhdcellwd[i];
	}
	sTable.vhdcellwd[allcol+1] = sTable.tabwidth-nwd;
}

void CHSEPrinter::FormatPairTableFirstHeadString(int cbcellcnt,SPrintTable& sTable)
{
	CString str;
	sTable.vhead.clear();
	sTable.vbody.clear();

	sTable.vhead.push_back(_T("频率[MHz]"));
	for (int i=0; i<cbcellcnt; i++)
	{
		str.Format(_T("%d-%g"),i*2,(i-1)*50+3.0f*(rand()/(float)RAND_MAX));
		sTable.vhead.push_back(str);
	}
	sTable.vhead.push_back(_T("结果"));
}

void CHSEPrinter::FomatPairdataTableString(int cbcellcnt,SPrintTable& sTable)
{
	CString str;
	sTable.vhead.clear();
	sTable.vbody.clear();

	CString vstr0[]={_T("标准数据"),_T("最差值"),_T("位置")};
	sTable.vhead.push_back(_T("线对"));
	for (int i=0; i<cbcellcnt; i++)
	{
		for (int j=0; j<_countof(vstr0);j++)
			sTable.vhead.push_back(vstr0[j]);
	}
	sTable.vhead.push_back(_T("结果"));

	CString vstr1[]={_T("线对1-2"),_T("线对1-3"),_T("线对1-4")};
	for (int i=0 ;i<_countof(vstr1); i++)
	{
		sTable.vbody.push_back(vstr1[i]);
		for (int j=0;j<sTable.vhead.size()-2; j++)
		{
			str.Format(_T("%.2g"),(i+1)*20+3.8f*(rand()/(float)RAND_MAX));
			sTable.vbody.push_back(str);
		}
		sTable.vbody.push_back(_T("合格"));
	}
}

void CHSEPrinter::FormatDataPages(CDC* pDC, CPrintInfo* pInfo)
{
	CRect rcPage = pInfo->m_rectDraw;
	CRect rect,rect1;
	rect.left = rcPage.left + m_nPage3rdHeadLD;
	rect.right = rcPage.right - m_nPage3rdHeadLD;
	rect.top = rcPage.top - m_nHeadHeight;
	rect.bottom = rect.top - 100;

	//PrintPageTitle(pDC,_T("汇总报告(数据)"),rect);

	SPrintTable sTable;
	SDataPage dataPage;
	//int nY;
	{
		sTable.title = _T("衰减（dB/m）不合格");
		int ncol_all = 52, maxcol = 0;	//数据项，不包括前后

		sTable.title_fnt = &m_fntPage4thTabCaption;
		sTable.content_fnt = &m_fntPage4thTabContent;
		sTable.tabline_pen = &m_penTabLine;
		sTable.isContentCenter = true;
		//sTable.isFrstLinePrt = true;
		//sTable.titleHeightScl = 1.5f;
		//sTable.contentHeightScl = 1.5f;
		sTable.tableft = rect.left;
		sTable.tabtop = rect.bottom;

		sTable.tabwidth = rect.Width();
		FormatTableWidth_Col(pDC,_T("X+999.999"),ncol_all,maxcol,sTable);

		bool isChangePage = false;
		int nCntmp = 0, nH,nH0,nH1;
		for (int i=0; i<ncol_all/maxcol+1*(0!=ncol_all%maxcol); i++)
		{
			nCntmp = ((i+1)*maxcol > ncol_all) ? ncol_all-i*maxcol : maxcol;
			FormatPointPairTableString(i*maxcol,nCntmp,sTable);

			if(0 < i )
				sTable.title = _T("");

			if(maxcol > nCntmp)
				FormatTableWidth_Col(nCntmp,sTable.tabwidth,sTable.vhdcellwd);

			sTable.isFrstLinePrt = !(maxcol>nCntmp);
			//if(!GetTabHeight(pDC,pInfo,sTable,nH))
			//{
			//	m_vDataPages.push_back(dataPage);
			//	dataPage.vTabs.clear();
			//	sTable.tabtop = rect.bottom;
			//	sTable.title = _T("衰减（dB/m）不合格");
			//	
			//	//////////////////////////////////////////////////////////////////////////
			//	GetTabHeight(pDC,pInfo,sTable,nH);
			//}			

			GetTabHeight(pDC/*,pInfo*/,sTable,nH);
			if(0==i)
				nH0 = nH;
			else if(1==i)
				nH1 = nH;
			
			if(sTable.tabtop-nH < pInfo->m_rectDraw.bottom+m_nFootHeight)
			{			
				m_vDataPages.push_back(dataPage);
				dataPage.vTabs.clear();
				sTable.tabtop = rect.bottom;
				sTable.title = _T("衰减（dB/m）不合格");
			}

			if(!sTable.title.IsEmpty())
				nH = nH0;
			else
				nH = nH1;
			dataPage.vTabs.push_back(sTable);
			sTable.tabtop -= nH;	
			//PrintSingleTable(pDC,pInfo,sTable,nY);
		}
	}

	int nSpcTabHD = m_nSpcTabHD;
	{
		int ncol_all = 3*10, maxcol = 3*3;	//数据项，不包括前后
		int nInCellCnt = 3;

		sTable.title = _T("近端串音（dB）合格");
		sTable.tabtop -=  nSpcTabHD;
		sTable.tabwidth = rect.Width();
		FormatTableWidth_Col(pDC,_T("标准数据"),ncol_all,maxcol,sTable,nInCellCnt);

		vector<int> tmp = sTable.vhdcellwd;
		int  nH,nH0,nH1,nH2,nH3;
		{
			FomatPairdataTableString(maxcol/nInCellCnt,sTable);
			GetTabHeight(pDC/*,pInfo*/,sTable,nH,1);
		}

		int nCntmp = 0;
		for (int i=0; i<ncol_all/maxcol+1*(0!=ncol_all%maxcol); i++)
		{
			if(sTable.tabtop-nH < pInfo->m_rectDraw.bottom+m_nFootHeight)
			{
				m_vDataPages.push_back(dataPage);
				dataPage.vTabs.clear();
				sTable.tabtop = rect.bottom;
				sTable.title = _T("近端串音（dB）合格");
			}
		
			nCntmp = ((i+1)*maxcol > ncol_all) ? ncol_all-i*maxcol : maxcol;
			if(maxcol > nCntmp)
				FormatTableWidth_Col(nCntmp,sTable.tabwidth,tmp);

			sTable.vhdcellwd = tmp;
			CombineTableCellWidth_Col(nInCellCnt,sTable.vhdcellwd);
			FormatPairTableFirstHeadString(nCntmp/nInCellCnt,sTable);
			sTable.isFrstLinePrt = !(maxcol>nCntmp);
			dataPage.vTabs.push_back(sTable);
			//PrintSingleTable(pDC,pInfo,sTable,nY);
				
			GetTabHeight(pDC/*,pInfo*/,sTable,nH2);
			if(0==i)
				nH0 = nH2;
			else if(1==i)
				nH1 = nH2;
			if(!sTable.title.IsEmpty())
				nH2 = nH0;
			else
				nH2 = nH1;
			sTable.tabtop -= nH2 ;

			sTable.title = _T("");
			sTable.vhdcellwd = tmp;
			FomatPairdataTableString(nCntmp/nInCellCnt,sTable);		
			sTable.isFrstLinePrt = false;
			dataPage.vTabs.push_back(sTable);
			//PrintSingleTable(pDC,pInfo,sTable,nY);

			if(0 == i)
				GetTabHeight(pDC/*,pInfo*/,sTable,nH3);
			sTable.tabtop -= nH3 ;
		}
	}

	{
		sTable.title = _T("对间延时差（ps/m）Risetime：200ps（20%-80%） 不合格");
		sTable.tabtop -=  nSpcTabHD;
		sTable.tabwidth = rect.Width();
		FormatTableWidth_Row(pDC,_T("标准数据"),2,sTable);
		
		int nLine = 30, nH0,nH1;
		GetTRowHeight(pDC,sTable,nH0,nH1);

		int nrowmax = 0, nold = 0;
		for (int i = 0; i<nLine; i +=nrowmax)
		{
			nrowmax = (sTable.tabtop -(pInfo->m_rectDraw.bottom+m_nFootHeight)-nH0)/nH1-1;
			if(0>=nrowmax)
			{
				m_vDataPages.push_back(dataPage);
				dataPage.vTabs.clear();
				sTable.tabtop = rect.bottom;
				nrowmax = 0;
				continue;
			}
			if(nLine - nold <= nrowmax)
				nrowmax = nLine- nold ;
			FormatPointSingleTableString(i,nrowmax,sTable);
			dataPage.vTabs.push_back(sTable);
			sTable.tabtop -= (nH0 + nH1*nrowmax);
			nold += nrowmax;
		}
	}

	{
		sTable.title = _T("差分阻抗（ohm）Risetime：200ps（20%-80%） 合格");
		sTable.tabtop -=  nSpcTabHD;
		sTable.tabwidth = rect.Width();
		FormatTableWidth_Row(pDC,_T(" 标准数据 "),6,sTable);

		int nLine = 25, nH0,nH1;
		GetTRowHeight(pDC,sTable,nH0,nH1);

		int nrowmax = 0, nold = 0;
		for (int i = 0; i<nLine; i +=nrowmax)
		{
			nrowmax = (sTable.tabtop -(pInfo->m_rectDraw.bottom+m_nFootHeight)-nH0)/nH1-1;
			if(0>=nrowmax)
			{
				m_vDataPages.push_back(dataPage);
				dataPage.vTabs.clear();
				sTable.tabtop = rect.bottom;
				nrowmax = 0;
				continue;
			}
			if(nLine - nold <= nrowmax)
				nrowmax = nLine- nold ;
			FormatPointSingleTableMMString(i,nrowmax,sTable);
			dataPage.vTabs.push_back(sTable);
			sTable.tabtop -= (nH0 + nH1*nrowmax);
			nold += nrowmax;
		}
	}

	m_vDataPages.push_back(dataPage);
	dataPage.vTabs.clear();

}

void CHSEPrinter::PrintDataPages(CDC* pDC, CPrintInfo* pInfo, int nCurPage)
{
	if(0>nCurPage-4 || m_vDataPages.size()-1<nCurPage-4)
		return;
	CRect rcPage = pInfo->m_rectDraw;
	CRect rect,rect1;
	rect.left = rcPage.left + m_nPage3rdHeadLD;
	rect.right = rcPage.right - m_nPage3rdHeadLD;
	rect.top = rcPage.top - m_nHeadHeight;
	rect.bottom = rect.top - 100;
	PrintPageTitle(pDC,_T("汇总报告(数据)"),rect);

	SDataPage curDataPage = m_vDataPages[nCurPage-4];
	
	//SPrintTable sTable;
	int nY = 0;
	for (size_t i = 0; i<curDataPage.vTabs.size();i++)
		PrintSingleTable(pDC,pInfo,curDataPage.vTabs[i],nY);

}


bool CHSEPrinter::SetPrintFont(EPrintPageConfig e, LOGFONT lf)
{
	if(PPF_NONE >= e || PPF_END <= e)
		return false;

	switch (e)
	{
	case PPF_TEST_DEPARTMENT_NAME:
		m_LFntTestDepartmentName = lf;
		break;
	case PPF_TEST_DEPARTMENT_ADDRESS:
		m_LFntTestDepartmentAddress = lf;
		break;
	case PPF_TEST_REPORT_TITLE:
		m_LFntTestReportTitle = lf;
		break;
	case PPT_PAGE1BODY_MID:
		m_LFntPage1stBodyMid = lf;
		break;
	case PPF_PAGE1BODY_DOWN:
		m_LFntPage1stBodyDown = lf;
		break;
	case PPF_FOOT:
		m_LFntFoot = lf;
		break;
	case PPF_PAGE2_TABCAPTION:
		m_LFntPage2ndTabCaption = lf;
		break;
	case PPF_PAGE2_TABCONTENT:
		m_LFntPage2ndTabContent = lf;
		break;
	case PPF_HEAD_CAPTION:
		m_LFntHeadCaption = lf;
		break;
	case PPF_HEAD_ITEM:
		m_LFntHeadItems = lf;
		break;
	case PPF_PAGE3_TITLE:
		m_LFntPage3rdTitle = lf;
		break;
	case PPF_PAGE4_TABCAPTION:
		m_LFntPage4thTabCaption = lf;
		break;
	case PPF_PAGE4_TABCONTENT:
		m_LFntPage4thTabContent = lf;
		break;
	}
	return true;
}

bool CHSEPrinter::GetPrintFont(EPrintPageConfig e, LOGFONT& lf)
{
	if(PPF_NONE >= e || PPF_END <= e)
		return false;

	switch (e)
	{
	case PPF_TEST_DEPARTMENT_NAME:
		lf = m_LFntTestDepartmentName;
		break;
	case PPF_TEST_DEPARTMENT_ADDRESS:
		lf = m_LFntTestDepartmentAddress ;
		break;
	case PPF_TEST_REPORT_TITLE:
		lf = m_LFntTestReportTitle;
		break;
	case PPT_PAGE1BODY_MID:
		lf = m_LFntPage1stBodyMid ;
		break;
	case PPF_PAGE1BODY_DOWN:
		lf = m_LFntPage1stBodyDown ;
		break;
	case PPF_FOOT:
		lf = m_LFntFoot ;
		break;
	case PPF_PAGE2_TABCAPTION:
		lf = m_LFntPage2ndTabCaption;
		break;
	case PPF_PAGE2_TABCONTENT:
		lf = m_LFntPage2ndTabContent ;
		break;
	case PPF_HEAD_CAPTION:
		lf = m_LFntHeadCaption ;
		break;
	case PPF_HEAD_ITEM:
		lf = m_LFntHeadItems;
		break;
	case PPF_PAGE3_TITLE:
		lf = m_LFntPage3rdTitle ;
		break;
	case PPF_PAGE4_TABCAPTION:
		lf = m_LFntPage4thTabCaption ;
		break;
	case PPF_PAGE4_TABCONTENT:
		lf = m_LFntPage4thTabContent ;
		break;
	}
	return true;
}

bool CHSEPrinter::GetPrintFont(EPrintPageLayout e, int val)
{
	if(PPL_NONE >= e || PPL_END <= e)
		return false;

	switch (e)
	{
	case PPL_PAGE_NUMBER:
		m_nPrintPage = val;
		break;
	case PPL_PAGE1_HEAD_TD:	
		m_nPage1stHeadTD = val;
		break;
	case PPL_PAGE1_HEAD_LD:			
		m_nPage1stHeadLD = val;
		break;
	case PPL_PAGE1_ICON_HEIGHT:			
		m_nPage1stIconHeight = val;
		break;
	case PPL_PAGE1_ICON_WIDTH:			
		m_nPage1stIconWidth = val;
		break;
	case PPL_TEST_REPORT_TITLE_TD:			
		m_nTestReportTitleTD = val;
		break;
	case PPL_TEST_REPORT_TITLE_BD:			
		m_nTestReportTitleBD = val;
		break;
	case PPL_PAGE1_BODYMID_LD:			
		m_nPage1stBodyMidLD = val;
		break;
	case PPL_PAGE1_BODYDOWN_BD:			
		m_nPage1stBodyDownBD = val;
		break;
	case PPL_FOOT_HEIGHT:			
		m_nFootHeight = val;
		break;
	case PPL_PAGE2_HEAD_HEIGHT:			
		m_nPage2ndHeadHeight = val;
		break;
	case PPL_PAGE2_LD:			
		m_nPage2ndLD = val;
		break;
	case PPL_HEAD_HEIGHT:			
		m_nHeadHeight = val;
		break;
	case PPL_PAGE3_HEAD_LD:			
		m_nPage3rdHeadLD = val;
		break;
	case PPL_PAGE3_ICON_HEIGHT:			
		m_nPage3rdIconHeight = val;
		break;
	case PPL_PAGE3_ICON_WIDTH:			
		m_nPage3rdIconWidth = val;
		break;
	case PPL_SPC_TAB_HD:			
		m_nSpcTabHD = val;
		break;
	}
	return true;
}

bool CHSEPrinter::GetPrintFont(EPrintPageLayout e, int& val)
{
	if(PPL_NONE >= e || PPL_END <= e)
		return false;

	switch (e)
	{
	case PPL_PAGE_NUMBER:
		val = m_nPrintPage;
		break;
	case PPL_PAGE1_HEAD_TD:	
		val = m_nPage1stHeadTD;
		break;
	case PPL_PAGE1_HEAD_LD:			
		val = m_nPage1stHeadLD;
		break;
	case PPL_PAGE1_ICON_HEIGHT:			
		val = m_nPage1stIconHeight;
		break;
	case PPL_PAGE1_ICON_WIDTH:			
		val = m_nPage1stIconWidth;
		break;
	case PPL_TEST_REPORT_TITLE_TD:			
		val = m_nTestReportTitleTD ;
		break;
	case PPL_TEST_REPORT_TITLE_BD:			
		val = m_nTestReportTitleBD;
		break;
	case PPL_PAGE1_BODYMID_LD:			
		val = m_nPage1stBodyMidLD ;
		break;
	case PPL_PAGE1_BODYDOWN_BD:			
		val = m_nPage1stBodyDownBD ;
		break;
	case PPL_FOOT_HEIGHT:			
		val = m_nFootHeight ;
		break;
	case PPL_PAGE2_HEAD_HEIGHT:			
		val = m_nPage2ndHeadHeight ;
		break;
	case PPL_PAGE2_LD:			
		val = m_nPage2ndLD ;
		break;
	case PPL_HEAD_HEIGHT:			
		val = m_nHeadHeight;
		break;
	case PPL_PAGE3_HEAD_LD:			
		val = m_nPage3rdHeadLD ;
		break;
	case PPL_PAGE3_ICON_HEIGHT:			
		val = m_nPage3rdIconHeight ;
		break;
	case PPL_PAGE3_ICON_WIDTH:			
		val = m_nPage3rdIconWidth;
		break;
	case PPL_SPC_TAB_HD:			
		val = m_nSpcTabHD;
		break;
	}
	return true;
}
