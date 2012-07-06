// HSEPDoc.h : interface of the CHSEPDoc class
//


#pragma once


class CHSEPDoc : public CDocument
{
protected: // create from serialization only
	CHSEPDoc();
	DECLARE_DYNCREATE(CHSEPDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CHSEPDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


