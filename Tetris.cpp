//=========================================================
// Project	Win32 俄罗斯方块程序
// File		Tetris.cpp 主程序
// Update	2009.04
// Author	沉淪 http://www.EasyIDE.cn
// Download 源码爱好者：http://www.NewXing.com
// Tools	Visual C++ 6.0 & Editplus 3
//=========================================================

#include "Tetris.h"

//Windows 入口函数
int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR szCmdLine,int iCmdShow)
{
	static TCHAR	szClsName[] = TEXT("Tetris");
	HWND		hwnd;
	MSG		msg;
	WNDCLASSEX	wndclass;
	
	InitCommonControls(); //初始化通用控件类库

	//程序启动时初始化数据
	hInst = hInstance;
	RandomBlock(&ii,&jj,&cc);
	//x = 4; y = -1;
	iLevel = 1; iSorce = 0;
	iLayer = 0; iSorceMax = TetrisIni(0);
	hDcAlert = CreateCompatibleDC(NULL);

	//注册窗口类
	wndclass.hInstance	= hInstance;
	wndclass.lpszClassName	= szClsName;
	wndclass.lpfnWndProc	= WndProc;
	wndclass.cbClsExtra	= 0;
	wndclass.cbWndExtra	= 0;
	wndclass.cbSize		= sizeof(WNDCLASSEX);
	wndclass.style		= CS_HREDRAW|CS_VREDRAW;
	wndclass.hIcon		= LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
	wndclass.hIconSm	= NULL;
	wndclass.hCursor	= LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground	= (HBRUSH)(COLOR_BTNFACE+1);
	wndclass.lpszMenuName	= MAKEINTRESOURCE(IDR_MENU1);
	if ( !RegisterClassEx(&wndclass) )
	{
		MessageBox(
			NULL,
			TEXT("This program requires Windows NT!"),
			TEXT("RegisterClass ERROR"),
			MB_ICONERROR);
		return 0;

	}

	//创建主窗口
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		szClsName,
		TEXT("俄罗斯方块Win32版"),
		WS_MINIMIZEBOX|WS_SYSMENU,
		GetSystemMetrics(SM_CXFULLSCREEN)/2-370/2,
		GetSystemMetrics(SM_CYFULLSCREEN)/2-520/2,
		370,
		520,
		NULL,
		NULL,
		hInstance,
		NULL);
	
	//显示窗口并刷新客户区
	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	//构建消息循环
	while ( GetMessage(&msg,NULL,0,0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//主窗口过程
LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	HDC		hdc;
	PAINTSTRUCT	ps;

	switch ( message )
	{
	case WM_CREATE:
		GameUI(hwnd); //创建界面控件
		GameStart(hwnd); //数据初始化
		//背景音乐初始化
		bMusicOn = TRUE;
		iMusicBG = IDR_WAVE1;
		hMenu = GetMenu(hwnd);
		MusicItem(IDM_MUSIC1);
		return 0;
	case WM_PAINT:
		hdc = BeginPaint(hwnd,&ps);
		DrawMatrix(hdc); //绘制游戏区
		DrawTetris(hdc,0,0,0); //绘制方块
		DrawNext(hdc); //绘制预览区
		BitBlt(hdc,40,200,160,50,hDcAlert,0,0,SRCCOPY);//绘制提示语
		EndPaint(hwnd,&ps);
		return 0;
	case WM_TIMER:
		DropTetris(hwnd);
		return 0;
	case WM_DRAWITEM:
		DrawButton(lParam); //绘制方向按钮
		return TRUE;
	case WM_KEYDOWN:
		switch ( (int)wParam )
		{
		case VK_LEFT:
			if ( !bStart ) break;
			if ( CheckBlock(x-1,y) )
			{
				x --;
				hdc = GetDC(hwnd);
				DrawTetris(hdc,-1,0,0);
				ReleaseDC(hwnd,hdc);
			}
			
			break;
		case VK_RIGHT:
			if ( !bStart ) break;
			if ( CheckBlock(x+1,y) )
			{
				x ++;
				hdc = GetDC(hwnd);
				DrawTetris(hdc,1,0,0);
				ReleaseDC(hwnd,hdc);
			}
			
			break;
		case VK_UP:
			if ( !bStart || y<0 ) break;
			RankTetris(hwnd);
			break;
		case VK_DOWN:
			if ( !bStart ) break;
			DropTetris(hwnd);
			break;
		case VK_SPACE:
			GamePause(hwnd);
			break;
		}
		return 0;
	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case IDM_STOP:
			if ( y == -1 ) break;
			GameOver(hwnd);
			break;
		case IDM_NEW:
			GameStart(hwnd);
			GamePause(hwnd);
			break;
		case IDM_PAUSE:
			GamePause(hwnd);
			break;
		case IDM_EXIT:
			SendMessage(hwnd,WM_DESTROY,NULL,NULL);
			break;
		case IDM_ABOUT:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG1),hwnd,AboutDlgProc);
			break;
		case IDM_HELP:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG2),hwnd,HelpDlgProc);
			break;
		case IDM_MUSIC:
			DialogBox(hInst,MAKEINTRESOURCE(IDD_DIALOG3),hwnd,MusicDlgProc);
			break;
		case IDM_MUSIC1:
			MusicItem(IDM_MUSIC1);
			break;
		case IDM_MUSIC2:
			MusicItem(IDM_MUSIC2);
			break;
		case IDM_MUSICOFF:
			MusicItem(IDM_MUSICOFF);
			break;
		case IDM_LEFT:
			SendMessage(hwnd,WM_KEYDOWN,VK_LEFT,0);
			SetFocus(hwnd);
			break;
		case IDM_RIGHT:
			SendMessage(hwnd,WM_KEYDOWN,VK_RIGHT,0);
			SetFocus(hwnd);
			break;
		case IDM_UP:
			SendMessage(hwnd,WM_KEYDOWN,VK_UP,0);
			SetFocus(hwnd);
			break;
		case IDM_DOWN:
			SendMessage(hwnd,WM_KEYDOWN,VK_DOWN,0);
			SetFocus(hwnd);
			break;
		}
		return 0;
	case WM_CLOSE:
		if ( iSorce > iSorceMax ) iSorceMax = iSorce;
		TetrisIni(iSorceMax);
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd,message,wParam,lParam);
}

//关于对话框窗口过程
BOOL CALLBACK AboutDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch ( message )
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case IDOK:
			EndDialog(hDlg,0);
			break;
		}
		break;
	}
	return FALSE;
}

//帮助对话框窗口过程
BOOL CALLBACK HelpDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch ( message )
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch ( LOWORD(wParam) )
		{
		case IDOK:
			EndDialog(hDlg,0);
			break;
		}
		break;
	}
	return FALSE;
}

//背景音乐对话框窗口过程
BOOL CALLBACK MusicDlgProc(HWND hDlg,UINT message,WPARAM wParam,LPARAM lParam)
{
	switch ( message )
	{
	case WM_INITDIALOG:
		if( iMusicBG == IDR_WAVE1 ) CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO3,IDC_RADIO1);
		if( iMusicBG == IDR_WAVE2 ) CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO3,IDC_RADIO2);
		if( !bMusicOn ) CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO3,IDC_RADIO3);
		return TRUE;
	case WM_COMMAND :
		switch ( LOWORD(wParam) )
		{
		case IDC_RADIO1:
			CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO3,IDC_RADIO1);
			break;
		case IDC_RADIO2:
			CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO3,IDC_RADIO2);
			break;
		case IDC_RADIO3:
			CheckRadioButton(hDlg,IDC_RADIO1,IDC_RADIO3,IDC_RADIO3);
			break;
		case IDOK:
			if ( IsDlgButtonChecked(hDlg,IDC_RADIO1) == BST_CHECKED )
			{
				bMusicOn = TRUE;
				iMusicBG = IDR_WAVE1;
				MusicItem(IDM_MUSIC1);
				if ( bStart ) TetrisMusic(TRUE);
			}
			if ( IsDlgButtonChecked(hDlg,IDC_RADIO2) == BST_CHECKED )
			{
				bMusicOn = TRUE;
				iMusicBG = IDR_WAVE2;
				MusicItem(IDM_MUSIC2);
				if ( bStart ) TetrisMusic(TRUE);
			}
			if ( IsDlgButtonChecked(hDlg,IDC_RADIO3) == BST_CHECKED )
			{
				bMusicOn = FALSE;
				MusicItem(IDM_MUSICOFF);
				TetrisMusic(FALSE);
			}
			EndDialog(hDlg,0);
			break;
		case IDCANCEL:
			EndDialog(hDlg,0);
			break;
		}
		break;
	}
	return FALSE;
}

//音乐菜单处理
void MusicItem(int iCheckID)
{
	CheckMenuRadioItem(hMenu,IDM_MUSIC1,IDM_MUSICOFF,iCheckID,MF_BYCOMMAND);

	if ( iCheckID == IDM_MUSIC1 )
	{
		bMusicOn = TRUE;
		iMusicBG = IDR_WAVE1;
		if ( bStart ) TetrisMusic(TRUE);
	}else if ( iCheckID == IDM_MUSIC2 )
	{
		bMusicOn = TRUE;
		iMusicBG = IDR_WAVE2;
		if ( bStart ) TetrisMusic(TRUE);
	}
	else
	{
		bMusicOn = FALSE;
		TetrisMusic(FALSE);
	}

	return;
}

//背景音乐
void TetrisMusic(BOOL start)
{
	if ( start && bMusicOn && !bGOver )
		PlaySound(MAKEINTRESOURCE(iMusicBG),NULL,SND_RESOURCE|SND_LOOP|SND_ASYNC);
	else
		PlaySound(NULL,NULL,NULL);
	return;
}

//初始化游戏数据
void GameStart(HWND hwnd)
{
	int	m, n;
	HDC	hdc;
	hDcBitmap = CreateCompatibleDC(NULL);

	//初始化Matrix矩阵
	for ( m=0; m<=18; m++ )
	{
		for ( n=0; n<=9; n++ )
		{
			Matrix[m][n] = -1; //Matrix[m][n]处初始颜色值为-1
		}
	}

	//初始化方块位置
	x = 4; y = -1;
	
	//初始化标志变量
	bStart = FALSE;
	bGOver = FALSE;
	
	//初始化游戏区
	hdc = GetDC(hwnd);
	DrawMatrix(hdc);
	ReleaseDC(hwnd,hdc);

	//初始化计分器
	iLevel = 1;
	wsprintf(szBuffer,TEXT("%i"),iLevel);
	SetWindowText(hLevel,szBuffer); //级别

	iLayer = 0;
	wsprintf(szBuffer,TEXT("%i"),iLayer);
	SetWindowText(hLayer,szBuffer); //满行

	if ( iSorce > iSorceMax ) iSorceMax = iSorce;
	wsprintf(szBuffer,TEXT("%i"),iSorceMax);
	SetWindowText(hSorceMax,szBuffer); //最高分

	iSorce = 0;
	wsprintf(szBuffer,TEXT("%i"),iSorce);
	SetWindowText(hSorce,szBuffer); //得分

	//状态栏提示
	SendMessage(hStatus,SB_SETTEXT,0,(LPARAM)TEXT("空格键开始/暂停"));
	SendMessage(hStatus,SB_SETTEXT,1,(LPARAM)TEXT("方向键上键控制变形，左、右、下键控制移动"));

	//清除提示DC在的位图
	hDcAlert = CreateCompatibleDC(NULL);

	return;
}

//暂停/继续游戏
void GamePause(HWND hwnd)
{
	if( bGOver ) return;
	if ( bStart )
	{
		KillTimer(hwnd, 1);
		SendMessage(hStatus,SB_SETTEXT,0,(LPARAM)TEXT("游戏暂停"));
		TetrisMusic(FALSE);
	}
	else
	{
		SetTimer(hwnd, 1, (550-55*iLevel), NULL);
		SendMessage(hStatus,SB_SETTEXT,0,(LPARAM)TEXT("游戏中 ^-^"));
		TetrisMusic(TRUE);
	}
	bStart = !bStart;
	return;
}

//游戏结束
void GameOver(HWND hwnd)
{
	HDC	hdc;

	KillTimer(hwnd, 1);
	SelectObject(hDcAlert,LoadBitmap(hInst,MAKEINTRESOURCE(IDB_BITMAP9)));
	hdc = GetDC(hwnd);
	BitBlt(hdc,40,200,160,50,hDcAlert,0,0,SRCCOPY);//绘制提示语
	ReleaseDC(hwnd,hdc);
	SendMessage(hStatus,SB_SETTEXT,0,(LPARAM)TEXT("游戏结束"));

	bGOver = TRUE;
	x = 4; y = -1;

	TetrisMusic(FALSE);
	return;
}

//从INI文件中读/写最高分
int TetrisIni(int value)
{
	int iSorceMax = 0;
	TCHAR szBuffer[10];
	TCHAR szIniFile[100];

	SHGetSpecialFolderPath(0,szIniFile,CSIDL_APPDATA,0);
	lstrcat(szIniFile,TEXT("\\Win32Tetris"));
	CreateDirectory(szIniFile,NULL);
	lstrcat(szIniFile,TEXT("\\Config.ini"));

	if ( value == 0 )
	{//读
		iSorceMax = GetPrivateProfileInt(TEXT("Win32Tetris"),TEXT("SorceMax"),0,szIniFile);
	}
	else
	{//写
		wsprintf(szBuffer,TEXT("%i"),value);
		WritePrivateProfileString(TEXT("Win32Tetris"),TEXT("Contact"),TEXT("www.EasyIDE.cn"),szIniFile);
		WritePrivateProfileString(TEXT("Win32Tetris"),TEXT("SorceMax"),szBuffer,szIniFile);
	}
	
	return iSorceMax;
}

//用Matrix中的数据绘制游戏区
void DrawMatrix(HDC hdc)
{
	int	m, n;
	//绘制Matrix矩阵
	for ( m=0; m<=18; m++ )
	{
		for ( n=0; n<=9; n++ )
		{
			if ( Matrix[m][n] == -1 )
			{
				PatBlt(hdc,20+n*20,40+m*20,20,20,BLACKNESS);
			}
			else
			{
				SelectObject(hDcBitmap,LoadBitmap(hInst,MAKEINTRESOURCE(120+Matrix[m][n])));
				BitBlt(hdc,20+n*20,40+m*20,20,20,hDcBitmap,0,0,SRCCOPY);
			}
		}
	}
	return;
}

//绘制方块
void DrawTetris(HDC hdc, int iOffsetX, int iOffsetY, int iRank)
{
	int iX, iY, iJ;

	if ( y == -1 ) return;

	//如果方块平移过则擦除原来的方块位置
	if ( iOffsetX != 0 || iOffsetY != 0 )
	{
		iX = x + Tetris[i][j].a.x - iOffsetX;
		iY = y + Tetris[i][j].a.y - iOffsetY;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);

		iX = x + Tetris[i][j].b.x - iOffsetX;
		iY = y + Tetris[i][j].b.y - iOffsetY;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);

		iX = x + Tetris[i][j].c.x - iOffsetX;
		iY = y + Tetris[i][j].c.y - iOffsetY;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);

		iX = x + Tetris[i][j].d.x - iOffsetX;
		iY = y + Tetris[i][j].d.y - iOffsetY;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);
	}

	//如果方块旋转过则擦除原来的方块位置
	if ( iRank != 0 )
	{
		iJ = j==0 ? 3 : j-1;

		iX = x + Tetris[i][iJ].a.x;
		iY = y + Tetris[i][iJ].a.y;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);

		iX = x + Tetris[i][iJ].b.x;
		iY = y + Tetris[i][iJ].b.y;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);

		iX = x + Tetris[i][iJ].c.x;
		iY = y + Tetris[i][iJ].c.y;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);

		iX = x + Tetris[i][iJ].d.x;
		iY = y + Tetris[i][iJ].d.y;
		PatBlt(hdc,20+iX*20,40+iY*20,20,20,BLACKNESS);
	}

	//绘制新位置方块
	SelectObject(hDcBitmap,LoadBitmap(hInst,MAKEINTRESOURCE(120+c)));

	iX = x + Tetris[i][j].a.x;
	iY = y + Tetris[i][j].a.y;
	BitBlt(hdc,20+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	iX = x + Tetris[i][j].b.x;
	iY = y + Tetris[i][j].b.y;
	BitBlt(hdc,20+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	iX = x + Tetris[i][j].c.x;
	iY = y + Tetris[i][j].c.y;
	BitBlt(hdc,20+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	iX = x + Tetris[i][j].d.x;
	iY = y + Tetris[i][j].d.y;
	BitBlt(hdc,20+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);
	
	return;
}

//绘制预览区
void DrawNext(HDC hdc)
{
	RECT	rc;
	int	iX, iY;

	//预览区填充黑色
	rc.left = 240; rc.top = 40;
	rc.right = 340; rc.bottom = 140;
	FillRect(hdc,&rc,CreateSolidBrush(RGB(0,0,0)));

	//绘制预览方块
	SelectObject(hDcBitmap,LoadBitmap(hInst,MAKEINTRESOURCE(120+cc)));

	iX = 1 + Tetris[ii][jj].a.x; iY = 1 + Tetris[ii][jj].a.y;
	BitBlt(hdc,240+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	iX = 1 + Tetris[ii][jj].b.x; iY = 1 + Tetris[ii][jj].b.y;
	BitBlt(hdc,240+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	iX = 1 + Tetris[ii][jj].c.x; iY = 1 + Tetris[ii][jj].c.y;
	BitBlt(hdc,240+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	iX = 1 + Tetris[ii][jj].d.x; iY = 1 + Tetris[ii][jj].d.y;
	BitBlt(hdc,240+iX*20,40+iY*20,20,20,hDcBitmap,0,0,SRCCOPY);

	return;
}

//随机产生新的方块数据
void RandomBlock(int * l, int * m, int * n)
{
	srand((unsigned)time(NULL));

	* l = rand() % 7;
	* m = rand() % 4;
	* n = rand() % 7;

	return;
}

//方块越界检测
BOOL CheckBlock(int x, int y)
{
	int	m, n;

	//初始位置不允许左右移动
	if( y == -1 ) return false;

	//游戏区越界检测
	if ( Tetris[i][j].a.x+x<0 || Tetris[i][j].a.x+x>9 || Tetris[i][j].a.y+y>18 ) return FALSE;
	if ( Tetris[i][j].b.x+x<0 || Tetris[i][j].b.x+x>9 || Tetris[i][j].b.y+y>18 ) return FALSE;
	if ( Tetris[i][j].c.x+x<0 || Tetris[i][j].c.x+x>9 || Tetris[i][j].c.y+y>18 ) return FALSE;
	if ( Tetris[i][j].d.x+x<0 || Tetris[i][j].d.x+x>9 || Tetris[i][j].d.y+y>18 ) return FALSE;
	
	//方块相遇检测
	for ( m=0; m<=18; m++ )
	{
		for ( n=0; n<=9; n++ )
		{
			if ( m==Tetris[i][j].a.y+y && n==Tetris[i][j].a.x+x && Matrix[m][n]!=-1 ) return FALSE;
			if ( m==Tetris[i][j].b.y+y && n==Tetris[i][j].b.x+x && Matrix[m][n]!=-1 ) return FALSE;
			if ( m==Tetris[i][j].c.y+y && n==Tetris[i][j].c.x+x && Matrix[m][n]!=-1 ) return FALSE;
			if ( m==Tetris[i][j].d.y+y && n==Tetris[i][j].d.x+x && Matrix[m][n]!=-1 ) return FALSE;
		}
	}

	return TRUE;
}

//方块下落
void DropTetris(HWND hwnd)
{
	BOOL		bReDraw;
	int		l, m, n;
	HDC		hdc;

	//检测下一个方块位
	if ( CheckBlock(x,y+1) )
	{ //可以下落

		hdc = GetDC(hwnd);
		if ( y == -1 )
		{
			//传递方块并重绘预览区
			i = ii; j = jj; c = cc;
			RandomBlock(&ii,&jj,&cc);
			DrawNext(hdc);

			//绘制新方块
			y ++;
			DrawTetris(hdc,0,0,0);
		}
		else
		{
			//绘制新方块
			y ++;
			DrawTetris(hdc,0,1,0);

		}
		ReleaseDC(hwnd,hdc);
	}
	else
	{ //不能下落
		
		//在初始位置越界则结束游戏
		if ( y == -1 )
		{
			GameOver(hwnd);
			return;
		}
		
		//保存当前方块
		Matrix[ Tetris[i][j].a.y+y ][ Tetris[i][j].a.x+x ] = c;
		Matrix[ Tetris[i][j].b.y+y ][ Tetris[i][j].b.x+x ] = c;
		Matrix[ Tetris[i][j].c.y+y ][ Tetris[i][j].c.x+x ] = c;
		Matrix[ Tetris[i][j].d.y+y ][ Tetris[i][j].d.x+x ] = c;
		
		//逐行扫描 检测满行
		for ( l=18; l>=0; l-- )
		{
			bReDraw = TRUE;
			for ( m=0; m<=9; m++ )
			{
				if ( Matrix[l][m] == -1 )
				{
					bReDraw = FALSE;
					//break;
				}
			}
			if ( bReDraw )//检测到第l行为满行
			{
				//更新计分器
				iLayer += 1;
				wsprintf(szBuffer,TEXT("%i"),iLayer);
				SetWindowText(hLayer,szBuffer); //满行
				iSorce += 10;
				wsprintf(szBuffer,TEXT("%i"),iSorce);
				SetWindowText(hSorce,szBuffer); //得分
				if ( iSorce >= (iLevel*50+50)*iLevel/2 ) //升级
				{
					iLevel ++;
					wsprintf(szBuffer,TEXT("%i"),iLevel);
					SetWindowText(hLevel,szBuffer);
					SendMessage(hStatus,SB_SETTEXT,1,(LPARAM)TEXT("恭喜升级！"));
				}
				else
				{
					wsprintf(szBuffer,TEXT("恭喜加分！距离下次升级还需 %i 分！"),(iLevel*50+50)*iLevel/2-iSorce);
					SendMessage(hStatus,SB_SETTEXT,1,(LPARAM)szBuffer);
				}
				//删除满行数据
				for ( m=0; m<=9; m++ ) Matrix[l][m] = -1;
				//数据整体下移
				for ( n=l; n>0; n-- )
				{
					for ( m=0; m<=9; m++ )
					{
						Matrix[n][m] = Matrix[n-1][m];
						Matrix[n-1][m] = -1;
					}
				}
				//重绘游戏区
				hdc = GetDC(hwnd);
				DrawMatrix(hdc);
				ReleaseDC(hwnd,hdc);
				l ++; //重新检查当前行
			}
		}

		//重置方块位置
		x = 4; y = -1;
	}
}

//方块旋转
void RankTetris(HWND hwnd)
{
	HDC	hdc;
	if ( j == 3 )
	{
		j = 0;
		if ( CheckBlock(x,y) )
		{
			hdc = GetDC(hwnd);
			DrawTetris(hdc,0,0,1);
			ReleaseDC(hwnd,hdc);
		}
		else
		{
			j = 3;
		}
	}
	else
	{
		j++;
		if ( CheckBlock(x,y) )
		{
			hdc = GetDC(hwnd);
			DrawTetris(hdc,0,0,1);
			ReleaseDC(hwnd,hdc);
		}
		else
		{
			j --;
		}
	}
}

//在控件中显示默认设备字体
void SetNormalText(HWND hwnd,BOOL redraw)
{
	static HFONT	hFont;
	LOGFONT		lfBuffer;
	
	GetObject(GetStockObject(DEFAULT_GUI_FONT),sizeof(lfBuffer),&lfBuffer);
	hFont = CreateFontIndirect(&lfBuffer);  
	SendMessage(hwnd,WM_SETFONT,(WPARAM)hFont,(LPARAM)redraw);

	return;
}

//处理自绘消息
void DrawButton(LPARAM lParam)
{
	HDC			hdc;
	LPDRAWITEMSTRUCT	pdis;
	HPEN			hpen;

	pdis = (LPDRAWITEMSTRUCT)lParam; //包含自绘信息的结构体
	hdc = pdis->hDC;
	hpen = CreatePen(PS_SOLID,1,RGB(0,0,128));
	SelectObject(hdc,hpen);
	switch ( pdis->CtlID )
	{
	case IDM_UP:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem,WM_ERASEBKGND,(WPARAM)pdis->hDC,NULL);
		if ( pdis->itemState & ODS_SELECTED )
		{
			MoveToEx(hdc,13,1,NULL);
			LineTo(hdc,1,25);
			LineTo(hdc,25,25);
			LineTo(hdc,13,1);
			MoveToEx(hdc,12,3,NULL);
			LineTo(hdc,2,24);
		}
		else
		{
			MoveToEx(hdc,12,1,NULL);
			LineTo(hdc,1,24);
			LineTo(hdc,24,24);
			LineTo(hdc,12,1);
			MoveToEx(hdc,13,2,NULL);
			LineTo(hdc,25,25);
			LineTo(hdc,2,25);
		}
		break;
	case IDM_DOWN:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem,WM_ERASEBKGND,(WPARAM)pdis->hDC,NULL);
		if ( pdis->itemState & ODS_SELECTED )
		{
			MoveToEx(hdc,1,1,NULL);
			LineTo(hdc,13,25);
			LineTo(hdc,25,1);
			LineTo(hdc,1,1);
			MoveToEx(hdc,1,2,NULL);
			LineTo(hdc,24,2);
		}
		else
		{
			MoveToEx(hdc,1,1,NULL);
			LineTo(hdc,13,24);
			LineTo(hdc,24,1);
			LineTo(hdc,1,1);
			MoveToEx(hdc,25,2,NULL);
			LineTo(hdc,13,25);
			LineTo(hdc,1,2);
		}
		break;
	case IDM_LEFT:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem,WM_ERASEBKGND,(WPARAM)pdis->hDC,NULL);
		if ( pdis->itemState & ODS_SELECTED )
		{
			MoveToEx(hdc,1,13,NULL);
			LineTo(hdc,25,25);
			LineTo(hdc,25,1);
			LineTo(hdc,1,13);
			MoveToEx(hdc,2,13,NULL);
			LineTo(hdc,24,2);
		}
		else
		{
			MoveToEx(hdc,1,12,NULL);
			LineTo(hdc,24,23);
			LineTo(hdc,24,1);
			LineTo(hdc,1,12);
			MoveToEx(hdc,1,13,NULL);
			LineTo(hdc,25,25);
			LineTo(hdc,25,1);
		}
		break;
	case IDM_RIGHT:
		//FillRect(pdis->hDC,&pdis->rcItem,(HBRUSH)(COLOR_BTNFACE+1));
		SendMessage(pdis->hwndItem,WM_ERASEBKGND,(WPARAM)pdis->hDC,NULL);
		if ( pdis->itemState & ODS_SELECTED )
		{
			MoveToEx(hdc,1,1,NULL);
			LineTo(hdc,1,25);
			LineTo(hdc,25,12);
			LineTo(hdc,1,1);
			MoveToEx(hdc,2,24,NULL);
			LineTo(hdc,2,2);
			LineTo(hdc,24,13);
		}
		else
		{
			MoveToEx(hdc,1,1,NULL);
			LineTo(hdc,1,24);
			LineTo(hdc,24,12);
			LineTo(hdc,1,1);
			MoveToEx(hdc,1,25,NULL);
			LineTo(hdc,25,13);	
		}
		break;
	}
	DeleteObject(hpen);

	return;
}

//游戏界面控件
void GameUI(HWND hwnd)
{
	int	stWidth[] = {100,-1};
	HWND	hTemp;

	//工具栏
	CreateToolbarEx(
		hwnd,  //hwnd 父窗口句柄
		WS_VISIBLE|WS_CHILD|TBSTYLE_FLAT|TBSTYLE_TOOLTIPS|CCS_NODIVIDER,  //ws 工具栏风格
		IDR_TOOLBAR1,  //wID 工具栏的子窗口ID
		6,  //nBitmaps 位图中包含多少个按钮小图标
		hInst,  //hBMInst 包含位图资源的模块句柄
		IDB_BITMAP1,  //wBMID 位图资源ID
		tbButtons,  //lpButtons
		8,  //iNumButtons 按钮总数
		16,  //dxButton 单个按钮宽度
		16,  //dyButton 单个按钮高度
		16,  //dxBitmap 单个按钮小图标的宽度
		16,  //dyBitmap 单个按钮小图标的高度
		sizeof(TBBUTTON));  //uStructSize

	//状态栏
	hStatus = CreateStatusWindow(
		WS_CHILD|WS_VISIBLE,
		NULL,
		hwnd,
		IDS_STATUSBAR);
	SendMessage(hStatus,SB_SETPARTS,2,(LPARAM)(stWidth));

	//工具栏下的黑线
	CreateWindow(
		TEXT("static"),  //lpClassName 类名
		NULL,  //lpWindowName 标题
		WS_CHILD|WS_VISIBLE|SS_BLACKFRAME,
		1,24,
		358,1,
		hwnd,  //hWndParent
		NULL,  //hMenu
		hInst,
		NULL);  //lpParam  额外的参数

	//游戏区的外框
	CreateWindow(
		TEXT("static"),
		NULL,
		WS_CHILD|WS_VISIBLE|SS_GRAYFRAME,
		17,37,
		206,386,
		hwnd,
		NULL,
		hInst,
		NULL);

	//预览区的外框
	CreateWindow(
		TEXT("static"),
		NULL,
		WS_CHILD|WS_VISIBLE|SS_GRAYFRAME,
		237,37,
		106,106,
		hwnd,
		NULL,
		hInst,
		NULL);

	//级别
	hTemp = CreateWindow(
		TEXT("static"),
		TEXT("级别 :"),
		WS_CHILD|WS_VISIBLE|SS_LEFT,
		240,190,
		48,12,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp,FALSE);
	hLevel = CreateWindowEx(
		WS_EX_STATICEDGE,
		TEXT("static"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_GROUP|SS_RIGHT|SS_CENTERIMAGE,
		280,186,
		60,18,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hLevel,FALSE);

	//满行
	hTemp = CreateWindow(
		TEXT("static"),
		TEXT("满行 :"),
		WS_CHILD|WS_VISIBLE|SS_LEFT,
		240,220,
		36,12,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp,FALSE);
	hLayer = CreateWindowEx(
		WS_EX_STATICEDGE,
		TEXT("static"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_GROUP|SS_RIGHT|SS_CENTERIMAGE,
		280,216,
		60,18,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hLayer,FALSE);

	//得分
	hTemp = CreateWindow(
		TEXT("static"),
		TEXT("得分 :"),
		WS_CHILD|WS_VISIBLE|SS_LEFT,
		240,250,
		36,12,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp,FALSE);
	hSorce = CreateWindowEx(
		WS_EX_STATICEDGE,
		TEXT("static"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_GROUP|SS_RIGHT|SS_CENTERIMAGE,
		280,246,
		60,18,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hSorce,FALSE);

	//最高分
	hTemp = CreateWindow(
		TEXT("static"),
		TEXT("最高分"),
		WS_CHILD|WS_VISIBLE|SS_LEFT,
		240,280,
		36,12,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp,FALSE);
	hSorceMax = CreateWindowEx(
		WS_EX_STATICEDGE,
		TEXT("static"),
		NULL,
		WS_CHILD|WS_VISIBLE|WS_GROUP|SS_RIGHT|SS_CENTERIMAGE,
		280,276,
		60,18,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hSorceMax,FALSE);

	//方向键 上
	CreateWindow(
		TEXT("button"),
		NULL,
		WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
		280,340,
		26,26,
		hwnd,
		(HMENU)IDM_UP,
		hInst,
		NULL);

	//方向键 下
	CreateWindow(
		TEXT("button"),
		NULL,
		WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
		280,370,
		26,26,
		hwnd,
		(HMENU)IDM_DOWN,
		hInst,
		NULL);

	//方向键 左
	CreateWindow(
		TEXT("button"),
		NULL,
		WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
		253,355,
		26,26,
		hwnd,
		(HMENU)IDM_LEFT,
		hInst,
		NULL);

	//方向键 右
	CreateWindow(
		TEXT("button"),
		NULL,
		WS_CHILD|WS_VISIBLE|BS_OWNERDRAW,
		307,355,
		26,26,
		hwnd,
		(HMENU)IDM_RIGHT,
		hInst,
		NULL);
	
	hTemp = CreateWindow(
		TEXT("static"),
		TEXT("下载本程序源代码请访问作者主页：http://www.EasyIDE.cn"),
		WS_CHILD|WS_VISIBLE|SS_LEFT|WS_DISABLED,
		18,433,
		500,12,
		hwnd,
		NULL,
		hInst,
		NULL);
	SetNormalText(hTemp,FALSE);


	return;
}
