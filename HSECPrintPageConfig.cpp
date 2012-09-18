
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

	m_sTestDepartmentName = _T("�㶫��Ʒ�����ල�����о�Ժ");	
	m_sTestDepartmentAddress = _T("�������¸۶�·");				
	m_sTestDepartmentTelephone = _T("020-8923 2280");				
	m_sTestDepartmentFax = _T("020-89232696");				
	m_sTestDepartmentEmail = _T("xxxxx@126.com");		

	m_sTestReportTitle = _T("���Ա���");
	m_sWorkShop = _T("�㶫ʡXX����������");	
	m_sProductorID = _T("HDMI 1.4����");		
	m_sStandardName = _T("HDMI 1.4���Ա�׼");	
	m_sTestData = _T("");
	m_sResult = _T("2012/3/19");	
	m_sRemark = _T("�ϸ�");	

	m_sSoftwareNameEn = _T("CTS-HSEC");
	m_sSoftwareVersion = _T("V6.1");
	m_sSoftwareNameCn = _T("���ٵ����߲���ϵͳ��c���Ϻ������о���");


	//page 2
	m_sRFNAInfo_Name = _T("���������");
	m_saRFNATestItemConfigDescribe.Add(_T("AGL-E5071C-4PORT 300kHz-20GHz"));
	m_saRFNATestItemConfigDescribe.Add(_T("00233test"));
	m_sTDRInfo_Name = _T("ʱ�����");
	m_saTDRTestItemConfigDescribe.Add(_T("TEK-8300-80E04 80E03"));


	m_sOperator = _T("xx0025");
	m_sCableLengthID = _T("001");
	m_sTemperatureInCentigrade = _T("20.1��");
	m_sCableLengthInm = _T("25");
	m_sHumidity = _T("14.25");
	m_sProductPlateID = _T("ID0009");
	m_sRecordFilename = _T("xxxooegfj.ccc");
	m_sProductBatchID = _T("20120205XXX");

	//page 3

	CString str[] = {_T("˥��"),_T("����"),_T("HEAC˥��"),_T("����迹"),_T("�����������迹"),
		_T("Զ���������迹"),_T("�����ʱ"),_T("������ʱ��"),_T("�Լ���ʱ��"),_T("HEAC����迹"),
		_T("HEAC�����������迹"),_T("HEACԶ���������迹")};
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

