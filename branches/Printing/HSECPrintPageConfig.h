
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
	PPL_PAGE1_HEAD_TD,				// ���ϼ��
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
	CString	m_sPage1stIconPath;						//��ӡ������ҳҳüͼ��·��

	CString m_sTestDepartmentName;					//���Բ�������
	CString m_sTestDepartmentAddress;				//���Բ��ŵ�ַ
	CString m_sTestDepartmentTelephone;				//���Բ��ŵ绰
	CString m_sTestDepartmentFax;					//���Բ��Ŵ���
	CString m_sTestDepartmentEmail;					//���Բ�������

	CString m_sTestReportTitle;						//���Ա������

	CString m_sWorkShop;							//��Ʒ��������
	CString m_sProductorID;							//��Ʒ��ʾ
	CString m_sStandardName;						//���Ա�׼����
	CString m_sTestData;
	CString m_sResult;								//���Խ��
	CString m_sRemark;								//��ע

	CString m_sSoftwareNameEn;
	CString m_sSoftwareVersion;
	CString m_sSoftwareNameCn;

	//�����������Ϣ
	CString m_sRFNAInfo_Name;						//RFNA����
	CStringArray m_saRFNATestItemConfigDescribe;	//У׼����,У׼������,ɨ������
	//TDR��Ϣ
	CString m_sTDRInfo_Name;						//TDR����
	CStringArray m_saTDRTestItemConfigDescribe;		//У׼����,У׼������,ɨ������

	//////////////////////////////////////////////////////////////////////////
	CString m_sOperator;							//������Ա
	CString m_sCableLengthID;				//���±�ʶ
	CString m_sTemperatureInCentigrade;				//���Ի����¶ȣ����϶ȣ�
	CString m_sCableLengthInm;						//���Ե��³��ȣ�m��
	CString m_sHumidity;							//���Ի���ʪ��
	CString m_sProductPlateID;				//��Ʒ�̺�
	CString m_sRecordFilename;						//���Լ�¼�ļ�����
	CString m_sProductBatchID;				//��Ʒ����
	//////////////////////////////////////////////////////////////////////////
	//��չ��Ϣ
	CStringArray m_saExtendInfo;					//��\n�ָ�

	//��Ŀ��Ϣ
	CStringArray m_saTestResultList;				//��\n�ָ�




public:
	void Init();


	bool SetPrintPageConfig(EPrintPageConfig e, CString str);
	bool GetPrintPageConfig(EPrintPageConfig e, CString& str);

	virtual bool SetPrintFont(EPrintPageConfig e, LOGFONT lf) = 0;
	virtual bool GetPrintFont(EPrintPageConfig e, LOGFONT& lf) = 0;
	virtual bool GetPrintFont(EPrintPageLayout e, int val) = 0;
	virtual bool GetPrintFont(EPrintPageLayout e, int& val) = 0;


};

