// dxt0001Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "dxt0001.h"
#include "dxt0001Dlg.h"
#include "commctrl.h"
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <CTIME>
#include <MAP>
#include <MMSYSTEM.h>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////
const int c_id_submit = 2010;
const int c_id_ok = 7015;



class stockdata
{
private:
	struct mydatatype
	{
		CString num;
		float avalue;
	}data[1024];
	int sum;
public:
	stockdata(){sum=0;}
	float getdata(CString num)
	{
		for (int i =0 ;i<sum;i++)
		{
			if (num == data[i].num)
			{
				return data[i].avalue;
			}
		}
		return -1.0;
	}
	void insert(CString num,float avalue)
	{
		data[sum].num = num;
		data[sum].avalue = avalue;
		sum++;
	}
	void setValue(CString num,float avalue)
	{
		for (int i =0 ;i<sum;i++)
		{
			if (num == data[i].num)
			{
				data[i].avalue = avalue;
				return;
			}
		}
	}
	string getnum(int pos)
	{
		if (pos >= sum)
			return NULL;
		return data[pos].num;
	}
	float getavalue(int pos)
	{
		if (pos >= sum)
			return NULL;
		return data[pos].avalue;
	}
	int getsize()
	{
		return sum;
	}


}stockdata;


class cls_plug_base
{
private:
	vector<HWND> v_get;
	HWND h_parent;
	ofstream logstream;
	HWND h_refresh;

	void get_hwnd(HWND h_parent,int id)
	{
		HWND h_get_f = ::GetDlgItem(h_parent,id);
		if (h_get_f != NULL)
			v_get.push_back(h_get_f);
		
		HWND h_first_child = ::GetWindow(h_parent,GW_CHILD);
		
		if (h_first_child == NULL)
			return;
		
		do
		{
			get_hwnd(h_first_child,id);
			h_first_child = ::GetNextWindow(h_first_child,GW_HWNDNEXT);
			
		}while(h_first_child != NULL);
		
		return ;
		
	}


	void get_listctrl_text(HWND hwnd,int n_item,int n_subitem,char* buf,int size)
	{
		unsigned long int dw_pid;
		GetWindowThreadProcessId(hwnd,&dw_pid);
		HANDLE h_p = OpenProcess(PROCESS_ALL_ACCESS,false,dw_pid);
		
		LPVOID lp_text = ::VirtualAllocEx(h_p,NULL,1024,MEM_COMMIT,PAGE_READWRITE);
		LPVOID lp_item= ::VirtualAllocEx(h_p,NULL,sizeof(LVITEM),MEM_COMMIT,PAGE_READWRITE);
		
		LVITEM item;
		item.iItem = n_item;
		item.iSubItem = n_subitem;
		item.mask = LVIF_TEXT;
		item.cchTextMax = size - 2;
		item.pszText = (char*)lp_text;
		
		DWORD dwcnt;
		
		::WriteProcessMemory(h_p,lp_item,(LPVOID)&item,sizeof(LVITEM),&dwcnt);
		::SendMessage(hwnd,LVM_GETITEMTEXT,n_item,(LPARAM)lp_item);
		
		::ReadProcessMemory(h_p,lp_text,buf,1024,&dwcnt);
	}

	void get_hwnd2(HWND h_parent2)
	{
		HWND h_get_f = ::FindWindowEx(h_parent2,NULL,"MHPToolBar","MainViewBar");
		if (h_get_f != NULL)
			h_refresh = h_get_f;
		
		HWND h_first_child = ::GetWindow(h_parent2,GW_CHILD);
		
		if (h_first_child == NULL)
			return;
		
		do
		{
			get_hwnd2(h_first_child);
			h_first_child = ::GetNextWindow(h_first_child,GW_HWNDNEXT);
			
		}while(h_first_child != NULL);
		
		return ;	
	}

public:
	bool click_refreshbutton()
	{
		HWND h_parent3 = ::FindWindow("TdxW_MainFrame_Class",NULL);
		if (h_parent3 == NULL)
		{
			return false;
		}
		get_hwnd2(h_parent3);
		if (h_refresh == NULL)
		{
			return false;
		}
		::PostMessage(h_refresh,WM_LBUTTONDOWN,0,(LPARAM)MAKELONG(30 ,10 ));
		::PostMessage(h_refresh,WM_LBUTTONUP,0,(LPARAM)MAKELONG(30 ,10 ));
		return true;
	}
	void get_real_hwnd(int id)
	{
		v_get.clear();
		get_hwnd(h_parent,id);
	}


	cls_plug_base(bool b_main = true)
	{
		h_parent = ::FindWindow("TdxW_MainFrame_Class",NULL);
	}

	bool get_table_text(int id,int n_item,int n_subitem,char *buf,int size)
	{
		get_real_hwnd(id);
		if (v_get[0] == NULL)
			return false;
		get_listctrl_text(v_get[0],n_item,n_subitem,buf,size);
		return true;
	}

	bool get_text(int id,char *buf,int size)
	{
		get_real_hwnd(id);
		if (v_get[0] == NULL)
			return false;
		::SendMessage(v_get[0],WM_GETTEXT,size-2,(LPARAM)buf);
		return true;
	}

	bool set_edit_text(int id,char *buf,int size, int flag)
	{
		get_real_hwnd(id);
		if (v_get[0] == NULL)
			return false;
		::SendMessage(v_get[flag],WM_SETTEXT,size-2,(LPARAM)buf);
		return true;
	}

	bool set_combox_text(int id,char *buf, int flag)
	{
		get_real_hwnd(id);
		if (v_get[0] == NULL)
			return false;
		::SendMessage(v_get[flag],CB_SELECTSTRING,-1,(LPARAM)buf);
		return true;
	}

	bool ok(int flag)
	{
		HWND tmp_p;
		if (flag == 1)
		{
			tmp_p = ::FindWindow(NULL,"卖出交易确认");
		}
		else
		{
			tmp_p = ::FindWindow(NULL,"买入交易确认");
		}
		if (tmp_p == NULL)
			return false;
		HWND temp = ::GetDlgItem(tmp_p,7015);
		if (temp == NULL)
			return false;
		
		::PostMessage(temp,WM_LBUTTONDOWN,NULL,NULL);
		::PostMessage(temp,WM_LBUTTONUP,NULL,NULL);
		return true;
	}

	bool submit(int flag)
	{
		get_real_hwnd(c_id_submit);
		
		if (v_get[flag] == NULL)
			return false;
		::PostMessage(v_get[flag],WM_LBUTTONDOWN,NULL,NULL);
		::PostMessage(v_get[flag],WM_LBUTTONUP,NULL,NULL);
		return true;
	}


	void sell(char *str_num,float price, int amount ,bool testflag)
	{
		play_sound();
		char str_amount[16];
		char str_price[16];
		sprintf(str_amount,"%d",amount);
		sprintf(str_price,"%2f",price);
		set_edit_text(12005,str_num,1024,1);
		Sleep(100);
		set_edit_text(12006,str_price,1024,1);
		Sleep(100);
		set_edit_text(12007,str_amount,1024,1);
		Sleep(1500);
		submit(1);
		if (testflag == true)
			return;
		Sleep(500);
		ok(1);
		char message[1024];
		sprintf(message,"卖\t%s\t%f\t%d",str_num,price,amount);
		writelog("tradelog.txt",message);
	}

	void buy(char *str_num,float price, int amount ,bool testflag)
	{
		play_sound();
		char str_amount[16];
		char str_price[16];
		sprintf(str_amount,"%d",amount);
		sprintf(str_price,"%2f",price);
		set_edit_text(12005,str_num,1024,0);
		Sleep(100);
		set_edit_text(12006,str_price,1024,0);
		Sleep(100);
		set_edit_text(12007,str_amount,1024,0);
		Sleep(1500);
		submit(0);
		if (testflag == true)
			return;
		Sleep(500);
		ok(0);
		char message[1024];
		sprintf(message,"买\t%s\t%f\t%d",str_num,price,amount);
		writelog("tradelog.txt",message);

	}

	void writelog(char *filename,char *message)
	{
		time_t time_n;
		time(&time_n);
		logstream.open(filename,ofstream::ate|ofstream::app);
		logstream<<ctime(&time_n)<<message<<endl<<endl;
		logstream.close();
	}


	void play_sound()
	{
		HINSTANCE hi = AfxGetResourceHandle();
		CString str;
		str.Format("#%d",IDR_WAVE1);
		HRSRC hr = FindResource(hi,(LPCTSTR)str,"WAVE");
		HGLOBAL lp = LoadResource(hi,hr);
		LPVOID hg = LockResource(hg);
		sndPlaySound((LPCTSTR)lp,SND_MEMORY|SND_ASYNC);
		FreeResource(hg);
	}
};


class cls_windwild_dxt : public cls_plug_base
{
public:
	bool refresh_target_data()
	{
		if (click_refreshbutton())
			return true;
		return false;
	}
	
	
	
protected:
private:
};


///////////////

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxt0001Dlg dialog



CDxt0001Dlg::CDxt0001Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDxt0001Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDxt0001Dlg)
	m_edit01 = _T("");
	m_stocknum = _T("");
	m_avalue = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDxt0001Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDxt0001Dlg)
	DDX_Control(pDX, IDC_LIST2, m_inprice);
	DDX_Control(pDX, IDC_BUTTON4, m_stopbtn);
	DDX_Control(pDX, IDC_BUTTON3, m_startbtn);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Text(pDX, IDC_EDIT1, m_stocknum);
	DDX_Text(pDX, IDC_EDIT2, m_avalue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDxt0001Dlg, CDialog)
	//{{AFX_MSG_MAP(CDxt0001Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, OnItemchangedList2)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDxt0001Dlg message handlers


BOOL CDxt0001Dlg::OnInitDialog()
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

	m_stopbtn.EnableWindow(FALSE);

	//初始化listcontrol控件
	CString item_name[11] = {"证券代码","证券名称","证券数量","可卖数量","今买数量","持仓成本价","买入均价","当前价","最新市值","持仓盈亏","盈亏比例"};
	for (int i = 0; i<11; i++)
	{
		m_list.InsertColumn(i,item_name[i],LVCFMT_LEFT,80);
	}
	//初始化listcontrol控件完成
	
	m_inprice.InsertColumn(0,"代码",LVCFMT_LEFT,50);
	m_inprice.InsertColumn(1,"A value",LVCFMT_LEFT,100);
	m_inprice.InsertColumn(2,"上界",LVCFMT_LEFT,100);
	m_inprice.InsertColumn(3,"下届",LVCFMT_LEFT,100);

	cls_plug_base t;
	t.writelog("log.txt","开启程序，初始化完成");

	OnButton1();
	for (i = 0 ; i < CDxt0001Dlg::m_list.GetItemCount();i++)
	{
		if (atoi(CDxt0001Dlg::m_list.GetItemText(i,3))>0 || atoi(CDxt0001Dlg::m_list.GetItemText(i,4))>0)
		{
			stockdata.insert(m_list.GetItemText(i,0).GetBuffer(0),atof(m_list.GetItemText(i,5)));
		}
	}
	OnButton8();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDxt0001Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDxt0001Dlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDxt0001Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}




void CDxt0001Dlg::OnButton1() 
{
	cls_windwild_dxt t;
	t.refresh_target_data();
	Sleep(1000);
	char buf[1024];
	m_list.DeleteAllItems();
	int i=0;
	int const column = 11;
	while (1)
	{
		t.get_table_text(2064,i/column,i%column,buf,1022);
		if (buf[0] == '\0'&&i%column == 0)
			break;
		if (i%column == 0)
			m_list.InsertItem(i/column,buf,0);
		else
			m_list.SetItemText(i/column,i%column,buf);
		++i;
	}
}



void CDxt0001Dlg::OnButton2() 
{
	OnTimer(1);
}

void CDxt0001Dlg::OnButton3() 
{
	SetTimer(1,10000,NULL);
	m_startbtn.EnableWindow(FALSE);
	m_stopbtn.EnableWindow(TRUE);
	cls_plug_base t;
	t.writelog("log.txt","开始监听");
}

void CDxt0001Dlg::OnTimer(UINT nIDEvent) 
{
	cls_plug_base t;
	int i;
	float percent;
	char buf[256];
	
	bool testflag = false;

	float price_n;

	switch(nIDEvent)
	{
	case 1:
		OnButton1();
		for (i = 0; i < m_list.GetItemCount(); i++)
		{
			int sellable = atoi(m_list.GetItemText(i,3));
			if (sellable == 0)//when sum ==0
				break;
			percent = atof(m_list.GetItemText(i,10));//get percent
			price_n = atof(m_list.GetItemText(i,7));//get price
			CString num  = m_list.GetItemText(i,0);//get stock number
			float avalue = stockdata.getdata(num);//get stock avalue
			if (price_n < avalue * 0.97)//case percent < -3
			{
				OnButton3();
				if (sellable == 0)
					continue;
				if(sellable>=200)
					t.sell(num.GetBuffer(0),price_n,200,testflag);
				else
					t.sell(num.GetBuffer(0),price_n,sellable,testflag);
				//OnButton4();
				stockdata.setValue(num,avalue*0.97);
				sprintf(buf,"卖出%s\t%f\t%f",num,price_n,avalue);
				t.writelog("autotradelog.txt",buf);
				OnButton8();
			}
			if (price_n > avalue * 1.03)//case percent > 3
			{
				OnButton3();
				t.buy(num.GetBuffer(0),price_n,100,testflag);
				//OnButton4();
				stockdata.setValue(num,avalue*1.03);
				sprintf(buf,"买入%s\t%f\t%f",num,price_n,avalue);
				t.writelog("autotradelog.txt",buf);
				OnButton8();
			}
			sprintf(buf,"%s\t%f\t%f",num,price_n,avalue);
			t.writelog("scanlog.txt",buf);
			
		}
		break;
	case 2:
		::MessageBox(NULL,"case2","case2",NULL);
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDxt0001Dlg::OnButton4() 
{
	KillTimer(1);
	KillTimer(2);
	m_startbtn.EnableWindow(TRUE);
	m_stopbtn.EnableWindow(FALSE);
	cls_plug_base t;
	t.writelog("log.txt","停止监听");

}

void CDxt0001Dlg::OnButton5() 
{

}

void CDxt0001Dlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	cls_plug_base t;
	t.buy("600000",1,1,true);
}

void CDxt0001Dlg::OnButton7() 
{
	cls_plug_base t;
	t.sell("600000",1,1,true);
}

void CDxt0001Dlg::OnButton8() 
{
	m_inprice.DeleteAllItems();
	char buf[128];
	for (int i =0 ;i < stockdata.getsize();i++)
	{
		m_inprice.InsertItem(i,stockdata.getnum(i).c_str(),0);
		sprintf(buf,"%2f",stockdata.getavalue(i));
		m_inprice.SetItemText(i,1,buf);
		sprintf(buf,"%2f",stockdata.getavalue(i)*1.03);
		m_inprice.SetItemText(i,2,buf);
		sprintf(buf,"%2f",stockdata.getavalue(i)*0.97);
		m_inprice.SetItemText(i,3,buf);
	}
}



void CDxt0001Dlg::OnItemchangedList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CDxt0001Dlg::OnButton10() 
{
	// TODO: Add your control notification handler code here

	CString stknum = m_stocknum;
	float avalue = atof(m_avalue);
	int i = 0;
	if (stockdata.getdata(stknum) > 0)
	{
		stockdata.setValue(stknum,avalue);
	}
}
