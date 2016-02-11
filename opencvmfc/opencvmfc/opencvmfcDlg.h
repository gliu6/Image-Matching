// opencvmfcDlg.h : header file
//

#pragma once
#include "atltypes.h"
#include "highgui.h"
#include "cxtypes.h"
#include "afxwin.h"


// CopencvmfcDlg dialog
class CopencvmfcDlg : public CDialog
{
// Construction
public:
	CopencvmfcDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_OPENCVMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	IplImage* TheImage;						//Դͼ��1
	afx_msg void OnBnClickedShowimg();		//��ʾԴͼ��1
	void ShowImage(IplImage* img , UINT ID);
	void ResizeImage(IplImage* img,IplImage* W_img);
	afx_msg void OnBnClickedRead();			//��ͼ��
	afx_msg void OnBnClickedRead1();
	IplImage* TheImage1;					//Դͼ��2
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);		//������������Ӧ

	CPoint point1;							//��sampleʱ��Ҫ����������㣬ͨ����굥�����ͷŻ�������������
	CPoint point2;

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);		//����ͷ���Ӧ

	CString str;												//ͼ�񱣴��·��
	CString str1;

	IplImage* gray;							//���ڽ�����ͼ��ת��Ϊ�Ҷ�ͼ
	IplImage* gray1;

	CvvImage cvimg;
	afx_msg void OnBnClickedSample();			//ȡ������

	int SampleTag;
	IplImage* subimg;
	afx_msg void OnBnClickedMatch();			//ƥ�亯��

	CvRect sam_rect;
	CComboBox m_ctrlComboPort;					//combo box ����ɫ��Ӧ����
	CComboBox m_ctrlComboPort1;
};
