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
	IplImage* TheImage;						//源图像1
	afx_msg void OnBnClickedShowimg();		//显示源图像1
	void ShowImage(IplImage* img , UINT ID);
	void ResizeImage(IplImage* img,IplImage* W_img);
	afx_msg void OnBnClickedRead();			//读图像
	afx_msg void OnBnClickedRead1();
	IplImage* TheImage1;					//源图像2
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);		//鼠标左键单击相应

	CPoint point1;							//话sample时需要的两个坐标点，通过鼠标单击和释放获得区域的两个点
	CPoint point2;

	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);		//鼠标释放相应

	CString str;												//图像保存的路径
	CString str1;

	IplImage* gray;							//用于将读入图像转化为灰度图
	IplImage* gray1;

	CvvImage cvimg;
	afx_msg void OnBnClickedSample();			//取样函数

	int SampleTag;
	IplImage* subimg;
	afx_msg void OnBnClickedMatch();			//匹配函数

	CvRect sam_rect;
	CComboBox m_ctrlComboPort;					//combo box 的颜色对应变量
	CComboBox m_ctrlComboPort1;
};
