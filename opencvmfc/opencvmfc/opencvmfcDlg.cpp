// opencvmfcDlg.cpp : implementation file
//

#include "stdafx.h"
#include "opencvmfc.h"
#include "opencvmfcDlg.h"

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


// CopencvmfcDlg dialog




CopencvmfcDlg::CopencvmfcDlg(CWnd* pParent /*=NULL*/)
: CDialog(CopencvmfcDlg::IDD, pParent)
, TheImage(NULL)
, TheImage1(NULL)
, point1(0)
, point2(0)
, str(_T(""))
, str1(_T(""))
, gray(NULL)
, gray1(NULL)
, SampleTag(0)
, subimg(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CopencvmfcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COLOR, m_ctrlComboPort);
	DDX_Control(pDX, IDC_COLOR1, m_ctrlComboPort1);
}

BEGIN_MESSAGE_MAP(CopencvmfcDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_READ, &CopencvmfcDlg::OnBnClickedRead)
	ON_BN_CLICKED(IDC_READ1, &CopencvmfcDlg::OnBnClickedRead1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_SAMPLE, &CopencvmfcDlg::OnBnClickedSample)
	ON_BN_CLICKED(IDC_MATCH, &CopencvmfcDlg::OnBnClickedMatch)
END_MESSAGE_MAP()


// CopencvmfcDlg message handlers

BOOL CopencvmfcDlg::OnInitDialog()
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

	// TODO: Add extra initialization here
	CvSize ImgSize;
	ImgSize.height = IMAGE_HEIGHT;
	ImgSize.width = IMAGE_WIDTH;
	TheImage = cvCreateImage( ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );
	TheImage1 = cvCreateImage( ImgSize, IPL_DEPTH_8U, IMAGE_CHANNELS );
	gray = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1 );
	gray1 = cvCreateImage( cvSize(IMAGE_WIDTH, IMAGE_HEIGHT), IPL_DEPTH_8U, 1 );
	subimg = NULL;
	SampleTag=0;

	//设置Combo box的颜色初始化
	m_ctrlComboPort.ResetContent();
	m_ctrlComboPort.AddString(_T("White"));
	m_ctrlComboPort.AddString(_T("Black"));
	m_ctrlComboPort.SetCurSel(1); //选择颜色1

	m_ctrlComboPort1.ResetContent();
	m_ctrlComboPort1.AddString(_T("White"));
	m_ctrlComboPort1.AddString(_T("Black"));
	m_ctrlComboPort1.SetCurSel(1); //选择颜色1



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CopencvmfcDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CopencvmfcDlg::OnPaint()
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

		//重绘
		ShowImage(gray, IDC_SHOWIMG );
		ShowImage(gray1, IDC_SHOWIMG1 );
		CDialog::UpdateWindow();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CopencvmfcDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CopencvmfcDlg::ShowImage(IplImage* img , UINT ID)
{
	//获得控制IDC
	CDC* pDC = GetDlgItem( ID ) ->GetDC();
	//定义HDC，用于绘图		
	HDC hDC = pDC ->GetSafeHdc();				

	CRect rect;
	GetDlgItem(ID) ->GetClientRect( &rect );
	// 求控制的长高
	int rw = rect.right - rect.left;			
	int rh = rect.bottom - rect.top;
	// 图片长和高
	int iw = img->width;	
	int ih = img->height;
	// 使图片位于picture control的正中
	int tx = (int)(rw - iw)/2;	
	int ty = (int)(rh - ih)/2;
	SetRect( rect, tx, ty, tx+iw, ty+ih );
	// 复制图片
	CvvImage cimg;
	cimg.CopyOf( img );
	// 绘图	
	cimg.DrawToHDC( hDC, &rect );	

	ReleaseDC( pDC );

}



void CopencvmfcDlg::ResizeImage(IplImage* img,IplImage* W_img)
{
	// 读取图片宽和高
	int w = img->width;
	int h = img->height;

	// 取宽和高较大值
	int max = (w > h)? w: h;

	float scale = (float) ( (float) max / 256.0f );

	// 缩放图片宽和高
	int nw = (int)( w/scale );
	int nh = (int)( h/scale );

	// 计算正中位置
	int tlx = (nw > nh)? 0: (int)(256-nw)/2;
	int tly = (nw > nh)? (int)(256-nh)/2: 0;

	// 设置IMAGE的ROI
	cvSetImageROI( W_img, cvRect( tlx, tly, nw, nh) );

	// 缩放
	cvResize( img, W_img );

	//重置
	cvResetImageROI( W_img );

}




void CopencvmfcDlg::OnBnClickedRead()
{
	// TODO: Add your control notification handler code here
	// 选择图片对话框
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T(" All Files (*.*) |*.*||"), NULL
		);
	dlg.m_ofn.lpstrTitle = _T("Open Image");
	if( dlg.DoModal() != IDOK )	
		return;
	// 获得图片路径
	str = dlg.GetPathName();			

	int n = str.GetLength();     // n = 14, len = 18

	//unicode 转换
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pFileName = new char[len+1];   //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);

	pFileName[len+1] = '\0';   //多字节字符以'\0'结束



	IplImage* ipl = cvLoadImage( pFileName , 1 );
	if( !ipl )			
		return;
	if( TheImage )	
		cvZero( TheImage );

	ResizeImage( ipl ,TheImage);	

	//变为灰度图像灰度
	cvCvtColor( TheImage, gray, CV_BGR2GRAY );

	ShowImage( gray, IDC_SHOWIMG );
	//释放资源
	cvReleaseImage( &ipl );


}

void CopencvmfcDlg::OnBnClickedRead1()
{
	// TODO: Add your control notification handler code here
	// 选择图片对话框
	CFileDialog dlg(
		TRUE, _T("*.bmp"), NULL,
		OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY,
		_T(" All Files (*.*) |*.*||"), NULL
		);
	dlg.m_ofn.lpstrTitle = _T("Open Image");

	if( dlg.DoModal() != IDOK )	
		return;

	str1 = dlg.GetPathName();			

	int n = str1.GetLength();     // n = 14, len = 18

		//unicode 转换
	int len = WideCharToMultiByte(CP_ACP,0,str1,str1.GetLength(),NULL,0,NULL,NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pFileName = new char[len+1];   //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str1,str1.GetLength(),pFileName,len,NULL,NULL);

	pFileName[len+1] = '\0';   //多字节字符以'\0'结束


	IplImage* ipl = cvLoadImage( pFileName , 1 );

	if( !ipl )			
		return;

	if( TheImage1 )	
		cvZero( TheImage1 );
	ResizeImage( ipl ,TheImage1);	

	//变为灰度图像灰度
	cvCvtColor( TheImage1, gray1, CV_BGR2GRAY );

	ShowImage( gray1, IDC_SHOWIMG1 );
	// 释放资源
	cvReleaseImage( &ipl );

}



void CopencvmfcDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(SampleTag==1)
	{
		if(point.x>=56&&point.y>=39&&point.x<=310&&point.y<=292)
		{
			point1 =point;
		}
		else
		{
			SampleTag = 0;
			CWnd::MessageBox(_T("Please get the sample in the original picture."));
		}
	}
	CDialog::OnLButtonDown(nFlags, point);

}

void CopencvmfcDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	if(SampleTag==1)
	{
		point2 =point;
		if(point2.x<310&&point2.y<292&&point2.x>=56&&point2.y>=39)
		{

			//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
			int n = str1.GetLength();     // n = 14, len = 18

			//获取宽字节字符的大小，大小是按字节计算的
			int len = WideCharToMultiByte(CP_ACP,0,str1,str1.GetLength(),NULL,0,NULL,NULL);

			//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
			char * pFileName = new char[len+1];   //以字节为单位

			//宽字节编码转换成多字节编码
			WideCharToMultiByte(CP_ACP,0,str1,str1.GetLength(),pFileName,len,NULL,NULL);

			pFileName[len+1] = '\0';   //多字节字符以'\0'结束

			CRect pic_rect;
			GetDlgItem(IDC_SHOWIMG) ->GetClientRect( &pic_rect );


			CvRect img_rect;
			img_rect = cvGetImageROI(gray);


			sam_rect.x= point1.x - img_rect.x - pic_rect.left -56;
			sam_rect.y= point1.y - img_rect.y - pic_rect.top -39;
			sam_rect.width= point2.x-point1.x;
			sam_rect.height= point2.y-point1.y;


			/* copy ROI to subimg */
			subimg = cvCreateImage( cvSize(sam_rect.width, sam_rect.height), IPL_DEPTH_8U, 1 );
			cvSetImageROI(gray, sam_rect);
			cvCopy(gray, subimg, NULL);
			cvResetImageROI(gray);
			//ShowImage( subimg, IDC_SHOWIMG1 );

			int color = m_ctrlComboPort.GetCurSel();
			if(color == 0)
			{
				cvRectangle(gray,cvPoint(sam_rect.x,sam_rect.y),cvPoint(sam_rect.x+sam_rect.width,sam_rect.y+sam_rect.height),cvScalar(0,0,0));

			}
			if(color == 1)
			{
				cvRectangle(gray,cvPoint(sam_rect.x,sam_rect.y),cvPoint(sam_rect.x+sam_rect.width,sam_rect.y+sam_rect.height),cvScalar(255,255,255));

			}
			Invalidate(true);


			SampleTag = 0;
		}
		else
		{
			SampleTag = 0;
			CWnd::MessageBox(_T("Please get the sample in the original picture."));

		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CopencvmfcDlg::OnBnClickedSample()
{
	// TODO: Add your control notification handler code here
	SampleTag = 1;
	

	//清空sample痕迹，从新读入图片（和度图片内容一样）

	int n = str.GetLength();     // n = 14, len = 18

	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pFileName = new char[len+1];   //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pFileName,len,NULL,NULL);

	pFileName[len+1] = '\0';   //多字节字符以'\0'结束



	IplImage* ipl = cvLoadImage( pFileName , 1 );

	if( !ipl )			
		return;
	if( TheImage )	
		cvZero( TheImage );

	ResizeImage( ipl ,TheImage);	

	//变为灰度图像灰度
	cvCvtColor( TheImage, gray, CV_BGR2GRAY );
	ShowImage( gray, IDC_SHOWIMG );
	cvReleaseImage( &ipl );




	//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
	int n1 = str1.GetLength();     // n = 14, len = 18

	//获取宽字节字符的大小，大小是按字节计算的
	int len1 = WideCharToMultiByte(CP_ACP,0,str1,str1.GetLength(),NULL,0,NULL,NULL);

	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	char * pFileName1 = new char[len1+1];   //以字节为单位

	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str1,str1.GetLength(),pFileName1,len1,NULL,NULL);

	pFileName1[len1+1] = '\0';   //多字节字符以'\0'结束



	IplImage* ipl1 = cvLoadImage( pFileName1 , 1 );

	if( !ipl1 )			
		return;

	if( TheImage1 )	
		cvZero( TheImage1 );

	ResizeImage( ipl1 ,TheImage1);	
	//变为灰度图像灰度
	cvCvtColor( TheImage1, gray1, CV_BGR2GRAY );

	ShowImage( gray1, IDC_SHOWIMG1 );	
	cvReleaseImage( &ipl1 );

}

void CopencvmfcDlg::OnBnClickedMatch()
{
	// TODO: Add your control notification handler code here

	IplImage *ftmp; //ftmp will hold results

	//ALLOCATE OUTPUT IMAGES:
	int iwidth = gray1->width - subimg->width + 1;
	int iheight = gray1->height - subimg->height + 1;

	//设置结果图像，保存匹配结果
	ftmp = cvCreateImage(cvSize(iwidth,iheight),32,1);


	//进行模板匹配并进行归一化处理
	cvMatchTemplate( gray1, subimg, ftmp, 3);
	cvNormalize(ftmp,ftmp,1,0,CV_MINMAX);
	//ShowImage(ftmp,IDC_SHOWIMG1);

	//int max=0;
	//int max_y=0;
	//int max_x=0;
	//for(int y=0; y<ftmp->height;y++)
	//{
	//	uchar* ptr = (uchar*)(ftmp->imageData + y * ftmp->widthStep);
	//	for(int x = 0; x <  ftmp->width; x++)
	//	{
	//		if(ptr[x] >= max)
	//		{
	//			max=ptr[x];
	//			max_x=x;
	//			max_y=y;
	//		}
	//	}
	//}

	//在结果图像中，获得最好匹配的坐标起点
	double MinValue;
	double MaxValue;

	CvPoint MinLocation;
	CvPoint MaxLocation;

	cvSetImageCOI(ftmp,1 );
	cvMinMaxLoc(ftmp,& MinValue,& MaxValue,& MinLocation,& MaxLocation);
	
	//设置边框颜色，并且画出匹配区域
	int color = m_ctrlComboPort1.GetCurSel();
			if(color == 0)
			{
				cvRectangle(gray1,MaxLocation,cvPoint(MaxLocation.x+sam_rect.width,MaxLocation.y+sam_rect.height),cvScalar(0,0,0));
			}
			if(color == 1)
			{
				cvRectangle(gray1,MaxLocation,cvPoint(MaxLocation.x+sam_rect.width,MaxLocation.y+sam_rect.height),cvScalar(255,255,255));
			}
	Invalidate(true);

}
