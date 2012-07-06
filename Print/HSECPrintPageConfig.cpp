
#include "stdafx.h" 

#include "HSECPrintPageConfig.h"



CHSECPrintPageConfig::CHSECPrintPageConfig()
{
	Init();
}

CHSECPrintPageConfig::~CHSECPrintPageConfig()
{

}

void CHSECPrintPageConfig::Init()
{
	CString strPath,PathName;
	DWORD dwSize=MAX_PATH;
	::GetModuleFileName(NULL,strPath.GetBuffer(dwSize),dwSize); 
	strPath.ReleaseBuffer(dwSize);
	PathName = strPath.Left(strPath.ReverseFind(_T('\\'))+1);	
	m_sPage1stIconPath = PathName + _T("001.jpg");

	//page 1

	m_sTestDepartmentName = _T("广东产品质量监督检验研究院");	
	m_sTestDepartmentAddress = _T("广州市新港东路");				
	m_sTestDepartmentTelephone = _T("020-8923 2280");				
	m_sTestDepartmentFax = _T("020-89232696");				
	m_sTestDepartmentEmail = _T("xxxxx@126.com");		

	m_sTestReportTitle = _T("测试报告");
	m_sWorkShop = _T("广东省XX电缆生产厂");	
	m_sProductorID = _T("HDMI 1.4线缆");		
	m_sStandardName = _T("HDMI 1.4测试标准");	
	m_sTestData = _T("");
	m_sResult = _T("2012/3/19");	
	m_sRemark = _T("合格");	

	m_sSoftwareNameEn = _T("CTS-HSEC");
	m_sSoftwareVersion = _T("V6.1");
	m_sSoftwareNameCn = _T("高速电子线测试系统（c）上海电缆研究所");


	//page 2
	m_sRFNAInfo_Name = _T("网络分析仪");
	m_saRFNATestItemConfigDescribe.Add(_T("AGL-E5071C-4PORT 300kHz-20GHz"));
	m_saRFNATestItemConfigDescribe.Add(_T("00233test"));
	m_sTDRInfo_Name = _T("时域反射计");
	m_saTDRTestItemConfigDescribe.Add(_T("TEK-8300-80E04 80E03"));


	m_sOperator = _T("xx0025");
	m_sCableLengthID = _T("001");
	m_sTemperatureInCentigrade = _T("20.1℃");
	m_sCableLengthInm = _T("25");
	m_sHumidity = _T("14.25");
	m_sProductPlateID = _T("ID0009");
	m_sRecordFilename = _T("xxxooegfj.ccc");
	m_sProductBatchID = _T("20120205XXX");

	//page 3

	CString str[] = {_T("衰减"),_T("串音"),_T("HEAC衰减"),_T("差分阻抗"),_T("近端连接器阻抗"),
		_T("远端连接器阻抗"),_T("差分延时"),_T("对内延时差"),_T("对间延时差"),_T("HEAC差分阻抗"),
		_T("HEAC近端连接器阻抗"),_T("HEAC远端连接器阻抗")};
	for (int i= 0; i<_countof(str); i++)
		m_saTestResultList.Add(str[i]);


}

bool CHSECPrintPageConfig::SetPrintPageConfig(EPrintPageConfig e, CString str)
{
	if(PPC_NONE >= e || PPC_END <= e)
		return false;

	switch (e)
	{
	case PPC_PAGE1ST_ICON_PATH:
		m_sPage1stIconPath = str;
		break;
	case PPC_TEST_DEPARTMENT_NAME:
		m_sTestDepartmentName = str;
		break;
	case PPC_TEST_DEPARTMENT_ADDRESS:
		m_sTestDepartmentAddress = str;
		break;
	case PPC_TEST_DEPARTMENT_TELEPHONE:
		m_sTestDepartmentTelephone = str;
		break;
	case PPC_TEST_DEPARTMENT_FAX:
		m_sTestDepartmentFax = str;
		break;
	case PPC_TEST_DEPARTMENT_EMAIL:
		m_sTestDepartmentEmail = str;
		break;
	}
	return true;
}

bool CHSECPrintPageConfig::GetPrintPageConfig(EPrintPageConfig e, CString& str)
{
	if(PPC_NONE >= e || PPC_END <= e)
		return false;

	switch (e)
	{
	case PPC_PAGE1ST_ICON_PATH:
		str = m_sPage1stIconPath;
		break;
	case PPC_TEST_DEPARTMENT_NAME:
		str = m_sTestDepartmentName;
		break;
	case PPC_TEST_DEPARTMENT_ADDRESS:
		str = m_sTestDepartmentAddress;
		break;
	case PPC_TEST_DEPARTMENT_TELEPHONE:
		str = m_sTestDepartmentTelephone;
		break;
	case PPC_TEST_DEPARTMENT_FAX:
		str = m_sTestDepartmentFax ;
		break;
	case PPC_TEST_DEPARTMENT_EMAIL:
		str = m_sTestDepartmentEmail;
		break;
	}
	return true;
}

