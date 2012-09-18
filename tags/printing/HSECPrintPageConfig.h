
#include "afx.h"

#pragma once


enum EPrintPageConfig
{
	PPC_NONE = 0,

	PPC_PAGE1ST_ICON_PATH,
	PPC_TEST_DEPARTMENT_NAME,
	PPC_TEST_DEPARTMENT_ADDRESS,
	PPC_TEST_DEPARTMENT_TELEPHONE,
	PPC_TEST_DEPARTMENT_FAX,
	PPC_TEST_DEPARTMENT_EMAIL,

	PPC_END,
};

enum EPrintPageFont
{
	PPF_NONE = 0,
	PPF_TEST_DEPARTMENT_NAME,
	PPF_TEST_DEPARTMENT_ADDRESS,
	PPF_TEST_REPORT_TITLE,
	PPT_PAGE1BODY_MID,
	PPF_PAGE1BODY_DOWN,
	PPF_FOOT,
	PPF_PAGE2_TABCAPTION,
	PPF_PAGE2_TABCONTENT,
	PPF_HEAD_CAPTION,
	PPF_HEAD_ITEM,
	PPF_PAGE3_TITLE,
	PPF_PAGE4_TABCAPTION,
	PPF_PAGE4_TABCONTENT,

	PPF_END,
};

enum EPrintPageLayout
{
	PPL_NONE = 0,
	PPL_PAGE_NUMBER,
	PPL_PAGE1_HEAD_TD,				// 距上间距
	PPL_PAGE1_HEAD_LD,
	PPL_PAGE1_ICON_HEIGHT,
	PPL_PAGE1_ICON_WIDTH,
	PPL_TEST_REPORT_TITLE_TD,
	PPL_TEST_REPORT_TITLE_BD,
	PPL_PAGE1_BODYMID_LD,
	PPL_PAGE1_BODYDOWN_BD,
	PPL_FOOT_HEIGHT,
	PPL_PAGE2_HEAD_HEIGHT,
	PPL_PAGE2_LD,
	PPL_HEAD_HEIGHT,
	PPL_PAGE3_HEAD_LD,
	PPL_PAGE3_ICON_HEIGHT,
	PPL_PAGE3_ICON_WIDTH,
	PPL_SPC_TAB_HD,


	PPL_END,
};

class CHSECPrintPageConfig : public CObject
{
public:
	CHSECPrintPageConfig(void);
	~CHSECPrintPageConfig(void);

protected:
	CString	m_sPage1stIconPath;						//打印报告首页页眉图标路径

	CString m_sTestDepartmentName;					//测试部门名称
	CString m_sTestDepartmentAddress;				//测试部门地址
	CString m_sTestDepartmentTelephone;				//测试部门电话
	CString m_sTestDepartmentFax;					//测试部门传真
	CString m_sTestDepartmentEmail;					//测试部门邮箱

	CString m_sTestReportTitle;						//测试报告标题

	CString m_sWorkShop;							//产品生产厂家
	CString m_sProductorID;							//产品标示
	CString m_sStandardName;						//测试标准名称
	CString m_sTestData;
	CString m_sResult;								//测试结果
	CString m_sRemark;								//备注

	CString m_sSoftwareNameEn;
	CString m_sSoftwareVersion;
	CString m_sSoftwareNameCn;

	//网络分析仪信息
	CString m_sRFNAInfo_Name;						//RFNA名称
	CStringArray m_saRFNATestItemConfigDescribe;	//校准类型,校准件类型,扫描类型
	//TDR信息
	CString m_sTDRInfo_Name;						//TDR名称
	CStringArray m_saTDRTestItemConfigDescribe;		//校准类型,校准件类型,扫描类型

	//////////////////////////////////////////////////////////////////////////
	CString m_sOperator;							//测试人员
	CString m_sCableLengthID;				//电缆标识
	CString m_sTemperatureInCentigrade;				//测试环境温度（摄氏度）
	CString m_sCableLengthInm;						//测试电缆长度（m）
	CString m_sHumidity;							//测试环境湿度
	CString m_sProductPlateID;				//产品盘号
	CString m_sRecordFilename;						//测试记录文件名称
	CString m_sProductBatchID;				//产品批号
	//////////////////////////////////////////////////////////////////////////
	//扩展信息
	CStringArray m_saExtendInfo;					//以\n分割

	//项目信息
	CStringArray m_saTestResultList;				//以\n分割




public:
	void Init();


	bool SetPrintPageConfig(EPrintPageConfig e, CString str);
	bool GetPrintPageConfig(EPrintPageConfig e, CString& str);

	virtual bool SetPrintFont(EPrintPageConfig e, LOGFONT lf) = 0;
	virtual bool GetPrintFont(EPrintPageConfig e, LOGFONT& lf) = 0;
	virtual bool GetPrintFont(EPrintPageLayout e, int val) = 0;
	virtual bool GetPrintFont(EPrintPageLayout e, int& val) = 0;


};

