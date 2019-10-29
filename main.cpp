
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <dedkam2@wp.pl> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * ----------------------------------------------------------------------------
 */

#include <tchar.h>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <shlobj.h>
#include <shlwapi.h>
#include <objbase.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <cstring>
#pragma comment (lib, "Wininet.lib");
#include <wininet.h>
#include <vector>
#include <mysql.h>

#define IDM_PAGE_OPEN 101
#define IDM_FILE_QUIT 102
#define PASS_SUBMIT 103
#define IDM_PAGE_REFRESH 104
#define ADD_ID 4001
#define REMOVE_ID 4002
#define EDIT_ID 4003
#define FILE_SEND 4004
#define READ_ID 4005
#define SHOW_HELP 4006
#define CONTACT_ME 4007
#define UPDATE_PAGE 4008
#define NAME_SEARCH 4009
#define LINK_OPEN_FORMATTED 4010

using namespace std;

string VERSIONGUI = "v1.17";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void AddMenus(HWND);
void PassCHECK();
void startup();

HWND list, PASS, PASSBUTTON, hwnd, ID, LINK, NAME, INFOBOX, UPDATE, VERSION, INFO;
bool loginScreen = true;
vector <string> lol, lol2;
char IDS[255];
MYSQL_RES *idZapytania;
MYSQL_ROW  wiersz;

void INTELBOX(int SW)
{
	switch(SW)
	{
		case 1:
				SetWindowText(INFOBOX, "[INFO] Pomyœlnie wczytano program.");
				break;
		case 2:
				SetWindowText(INFOBOX, "[INFO] Pomyœlnie wys³ano plik.");
				break;
		case 3:
				SetWindowText(INFOBOX, "[ERROR] B³¹d podczas wysy³ania pliku!");
				break;
		case 4:
				SetWindowText(INFOBOX, "[ERROR] Brak internetu!");
				break;
		case 5:
				SetWindowText(INFOBOX, "[INFO] Internet dostêpny! Wysy³anie pliku...");
				break;
		case 6:
				SetWindowText(INFOBOX, "[INFO] Pomyœlnie wczytano informacje.");
				break;
		case 7:
				SetWindowText(INFOBOX, "[INFO] Pomyœlnie dodano utwór.");
				break;
		case 8:
				SetWindowText(INFOBOX, "[INFO] Pomyœlnie zmieniono informacje.");
				break;
		case 9:
				SetWindowText(INFOBOX, "[INFO] Pomyœlnie usuniêto utwór/ry.");
				break;
		case 10:
				SetWindowText(INFOBOX, "[ERROR] Podane ID jest za du¿e!");
				break;
	}
}

string intToStr(int n)
{
    string tmp;
    if(n < 0) {
      	tmp = "-";
      	n = -n;
    }
    if(n > 9)
      	tmp += intToStr(n / 10);
    tmp += n % 10 + 48;
    
    return tmp;
}

string FuncCorrect(int x)
{
	string tmp = intToStr(x);
	if(tmp.length() == 1)
    {
    	string tmp2 = "0";
    	tmp2 += tmp;
    	return tmp2;
	} else {
		return tmp;
	}
}

int ftpdwn()
{
	string location = "./ytwindowsdata.txt";
	string url = "http://krenski.cba.pl/else/ytwindowsdata.txt";
	URLDownloadToFileA ( NULL, url.c_str(), location.c_str(), 0, NULL );
	Sleep(500);
}

int filelist()
{
	fstream open;
	string x, xall;
	open.open("ytwindowsdata.txt", ios::in);
	int y = 0;
	while(!open.eof())
	{
		getline ( open, x );
		lol2.push_back(x.c_str());
		xall += x + "\n";
		y++;
		if (y == 7)
		{
			lol.push_back(xall.c_str());
			y = 0;
			xall = "";
		}
	}
	open.close();
	system("DEL /Q ytwindowsdata.txt");
}

int SHOW()
{
	string OUTALL = "";
	int i, ile, save;
	for ( i = 5, ile = 1; i<lol2.size(); i+=7, ile++)
	{
		string PROCESS = lol2[i], PROCESSOUT = "";
		for (int j = 0; j<PROCESS.length(); j++)
		{
			if (PROCESS[j] == '<' && PROCESS[j+1] == '/' && PROCESS[j+2] == 'd' && PROCESS[j+3] == 'i' && PROCESS[j+4] == 'v' && PROCESS[j+5] == '>') save = 0;
			if (save == 1) PROCESSOUT += PROCESS[j];
			if (PROCESS[j] == '>' && PROCESS[j-1] == '"' && PROCESS[j-2] == 'T' && PROCESS[j-3] == 'X' && PROCESS[j-4] == 'T') save = 1;
		}
		OUTALL += FuncCorrect(ile) + " - " + PROCESSOUT + "\r\n";
	}
	SetWindowText(list, OUTALL.c_str());
}

bool CheckTEXT(int POSITION)
{
	GetWindowText(ID, &IDS[0], 255);
	if (POSITION == 1)
	{
		bool check = true;
		for (int i = 0; i<((string)IDS).length(); i++)
		{
			if ((IDS[i] < 48 || IDS[i] > 57) && IDS[i] != 44) check = false;
		}
		return check;
	}
	else if (POSITION == 2)
	{
		bool check = true;
		for (int i = 0; i<((string)IDS).length(); i++)
		{
			if (IDS[i] >= 48 && IDS[i] <= 57);
			else check = false;
		}
		return check;
	}
	else if (POSITION == 3)
	{
		char link[255], nazwa[255];
		GetWindowText(LINK, &link[0], 255);
		GetWindowText(NAME, &nazwa[0], 255);
		if (((string)link).length() == 0 || ((string)nazwa).length() == 0)
		{
			return false;
		} else {
			return true;
		}
	}
	else if (POSITION == 4)
	{
		char link[255];
		GetWindowText(LINK, &link[0], 255);
		if (((string)link).length() == 0)
		{
			return false;
		} else {
			return true;
		}
	}
	else if (POSITION == 5)
	{
		char name[255];
		GetWindowText(NAME, &name[0], 255);
		if (((string)name).length() == 0)
		{
			return false;
		} else {
			return true;
		}
	}
}

int ADD()
{
	char link[255], nazwa[255];
	GetWindowText(LINK, &link[0], 255);
	GetWindowText(NAME, &nazwa[0], 255);

	int save;
	string linkout = "https://www.youtube.com/embed/";
	for (int i = 0; i<((string)link).length(); i++)
	{
		if (link[i-1] == '=' && link[i-2] == 'v' && link[i-3] == '?') save = 1;
		if (save == 1) linkout += link[i];
	}
	save = 0;
	linkout += "?showinfo=0";
	fstream outtemp;
	outtemp.open("outtemp.txt", ios::out);
	for (int i = 0; i < lol.size(); i++)
	{
		outtemp << lol[i];
	}
	outtemp.close();
	outtemp.open("outtemp.txt", ios::app);
	outtemp << "<div class=\"container\">\n        <div class=\"cyfra\"></div>\n        <div class=\"mainyt\">\n                <object class=\"iframe\" data=\"";
	outtemp << linkout;
	linkout = "";
	outtemp << "\" frameborder=\"0\" allow=\"autoplay; encrypted-media\"></object>\n        </div>\n        <div class=\"TXT\">";
	outtemp << nazwa;
	outtemp << "</div>\n</div>";
	outtemp.close();
	INTELBOX(7);
	SetWindowText(LINK, "");
	SetWindowText(NAME, "");
	SetWindowText(ID, "");
}

int REMOVE()
{
	GetWindowText(ID, &IDS[0], 255);
	
	vector <string> REM;
	vector <int> REMI;
	string TEMP = "";
	for (int i = 0; i < ((string)IDS).length(); i++)
	{
		if (IDS[i] != ',') TEMP += IDS[i];
		else {
			REM.push_back(TEMP);
			TEMP = "";
		}
		if (i == ((string)IDS).length()-1)
		{
			REM.push_back(TEMP);
			TEMP = "";
		}
	}
	
	for (int i = 0; i<REM.size(); i++)
	{
		REMI.push_back(atoi(REM[i].c_str()));
	}

	fstream outtemp;
	outtemp.open("outtemp.txt", ios::out);
	for (int i = 0; i < lol.size(); i++)
	{
		bool copy = true;
		for (int j = 0; j<REMI.size(); j++)
		{
			if (REMI[j]-1 == i) copy = false;
		}
		if (copy == true) outtemp << lol[i];
	}
	outtemp.close();
	SetWindowText(ID, "");
	INTELBOX(9);
}

int EDIT()
{
	char link[255], nazwa[255];
	GetWindowText(LINK, &link[0], 255);
	GetWindowText(NAME, &nazwa[0], 255);
	GetWindowText(ID, &IDS[0], 255);
	int NR = atoi(((string)IDS).c_str());
	
	bool linkb = true;
	bool nameb = true;
	
	if (((string)link).length() == 0) linkb = false;
	if (((string)nazwa).length() == 0) nameb = false;
	
	int save;
	string linkout = "https://www.youtube.com/embed/";
	if (linkb == true)
	{
		for (int i = 0; i<((string)link).length(); i++)
		{
			if (link[i-1] == '=' && link[i-2] == 'v' && link[i-3] == '?') save = 1;
			if (save == 1) linkout += link[i];
		}
		save = 0;
		linkout += "?showinfo=0";
	}
	
	fstream outtemp;
	int whicha = (((NR-1)*7)+5);
	int whichb = (((NR-1)*7)+3);
	outtemp.open("outtemp.txt", ios::out);
	for (int i = 0; i < lol2.size(); i++)
	{
		if (i == whicha && nameb == true)
			outtemp << "        <div class=\"TXT\">" << nazwa << "</div>" << endl;
		else if (i == whichb && linkb == true)
			outtemp << "        		<object class=\"iframe\" data=\"" << linkout << "\" frameborder=\"0\" allow=\"autoplay; encrypted-media\"></object>" << endl;
		else
			outtemp << lol2[i] << endl;
	}
	outtemp.close();
	SetWindowText(LINK, "");
	SetWindowText(NAME, "");
	SetWindowText(ID, "");
	INTELBOX(8);
}

int ftpsend()
{
    char ftp[] = "HIDDEN";
    char user[] = "HIDDEN";
    char password[] = "HIDDEN";
    char localFile[] = "out.txt";
    char remoteFile[] = "/else/ytwindowsdata.txt";
    
    HINTERNET hInternet;
    HINTERNET hFtpSession;
    
    if( InternetAttemptConnect( 0 ) == ERROR_SUCCESS ) INTELBOX(5);
    else { INTELBOX(4); return - 1; }
    hInternet = InternetOpen( NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0 );
   
    if( hInternet != NULL ) {
        hFtpSession = InternetConnect( hInternet, ftp, INTERNET_DEFAULT_FTP_PORT, user, password, INTERNET_SERVICE_FTP, INTERNET_FLAG_PASSIVE, 0 );
        if( hFtpSession != NULL ) {
            if( FtpPutFile( hFtpSession, localFile, remoteFile, FTP_TRANSFER_TYPE_BINARY, 0 ) ) {
                InternetCloseHandle( hFtpSession );
                InternetCloseHandle( hInternet );
        	}
            else {
                INTELBOX(3);
                return - 1;
            }
        }
        else return - 1;
    }
    else return - 1;
    INTELBOX(2);
}

int SAVE()
{
	fstream outtemp;
	outtemp.open("out.txt", ios::out);
	for (int i = 0; i < lol.size(); i++)
	{
		outtemp << lol[i];
	}
	outtemp.close();
	ftpsend();
}

void RELOADTMP()
{
	fstream TMP;
	string x, xall;
	int y = 0;
	TMP.open("outtemp.txt", ios::in);
	lol2.erase(lol2.begin(),lol2.end());
	lol.erase(lol.begin(),lol.end());
	while(!TMP.eof())
	{
		getline ( TMP, x );
		lol2.push_back(x.c_str());
		xall += x + "\n";
		y++;
		if (y == 7)
		{
			lol.push_back(xall.c_str());
			y = 0;
			xall = "";
		}
	}
	TMP.close();
}

int READ()
{
	GetWindowText(ID, &IDS[0], 255);
	int NR = atoi(((string)IDS).c_str());
	if (NR < 0) NR = 0;
	fstream outtemp;
	int whicha = (((NR-1)*7)+5);
	int whichb = (((NR-1)*7)+3);
	if (whicha > lol2.size() || whichb > lol2.size())
	{
		INTELBOX(10);
	} else {
		int save = 0;
		string LINKP = "", NAMEP = "";
		string PROCESS = lol2[whicha];
		for (int j = 0; j<PROCESS.length(); j++)
		{
			if (PROCESS[j] == '<' && PROCESS[j+1] == '/' && PROCESS[j+2] == 'd' && PROCESS[j+3] == 'i' && PROCESS[j+4] == 'v' && PROCESS[j+5] == '>') save = 0;
			if (save == 1) NAMEP += PROCESS[j];
			if (PROCESS[j] == '>' && PROCESS[j-1] == '"' && PROCESS[j-2] == 'T' && PROCESS[j-3] == 'X' && PROCESS[j-4] == 'T') save = 1;
		}
		
		save = 0;
		PROCESS = lol2[whichb];
		for (int j = 0; j<PROCESS.length(); j++)
		{
			if (PROCESS[j] == '?' && PROCESS[j+1] == 's' && PROCESS[j+2] == 'h' && PROCESS[j+3] == 'o' && PROCESS[j+4] == 'w' && PROCESS[j+5] == 'i') save = 0;
			if (save == 1) LINKP += PROCESS[j];
			if (PROCESS[j] == '/' && PROCESS[j-1] == 'd' && PROCESS[j-2] == 'e' && PROCESS[j-3] == 'b' && PROCESS[j-4] == 'm') save = 1;
		}
		
		string LINKPCHANGED = "https://www.youtube.com/watch?v=" + LINKP;
		SetWindowText(LINK, LINKPCHANGED.c_str());
		SetWindowText(NAME, NAMEP.c_str());
		INTELBOX(6);
	}
	
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    LPSTR lpCmdLine, int nCmdShow) {
    setlocale(LC_ALL, "polish");
    MSG  msg;    
    WNDCLASSW wc = {0};
    wc.lpszClassName = L"EDITPAGE";
    wc.hInstance     = hInstance;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+3);
    wc.lpfnWndProc   = WndProc;
    wc.hCursor       = LoadCursor(0, IDC_ARROW);
    wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassW(&wc)) {
		MessageBox(NULL, "Nie mo¿na wczytaæ programu!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
    
    hwnd = CreateWindowW(wc.lpszClassName, L"EDITPAGE - GUI EDITION",
                WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
                CW_USEDEFAULT, CW_USEDEFAULT,
				600, 364,
				NULL, NULL,
				hInstance, 0);

	if(hwnd == NULL) {
		MessageBox(NULL, "Nie mo¿na stworzyæ okna!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
    while (GetMessage(&msg, NULL, 0, 0)) {
    
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (GetAsyncKeyState(VK_RETURN) && loginScreen == true)
        {
        	PassCHECK();
		}
		if (GetAsyncKeyState(VK_ESCAPE))
        {
        	SendMessage(hwnd, WM_CLOSE, 0, 0);
        	return 0;
		}
    }

    return (int) msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, 
    WPARAM wParam, LPARAM lParam)
{
  	switch(msg)
  	{
      	case WM_CREATE:
      		{
      			system("DEL /Q outtemp.txt&DEL /Q ytwindowsdata.txt&DEL /Q out.txt");
      			
			    INFO = CreateWindow(
						"STATIC",
						"£¹czenie z baz¹ danych...",
						WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_CENTER,
						50, 50, 500, 20,
						hwnd, NULL, NULL, NULL);
				VERSION = CreateWindow(
						"STATIC",
						"",
						WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY | ES_CENTER,
						440, 5, 150, 20,
						hwnd, NULL, NULL, NULL);
				
				MYSQL mysql;
				mysql_init(&mysql);
				if (mysql_real_connect(&mysql, "HIDDEN", "HIDDEN", "HIDDEN", "HIDDEN", 3306, NULL, 0))
				{
					SetWindowText(INFO, "[INFO] Pomyœlnie po³¹czono siê z baz¹ danych.");
					mysql_select_db(&mysql, "zszbrodnica_register");
					mysql_query(&mysql, "SELECT * FROM PASSWORD WHERE PASSWORD.ID = 1");
					idZapytania = mysql_store_result(&mysql);
					wiersz = mysql_fetch_row(idZapytania);
					string VEROUT = VERSIONGUI + " - " + wiersz[0];
					string VERNEW = wiersz[0];
					
					if (VERSIONGUI != VERNEW)
					{
						UPDATE = CreateWindow(
						"BUTTON",
						"UPDATE!",
						WS_VISIBLE | WS_CHILD | WS_BORDER,
						440, 25, 150, 20,
						hwnd, (HMENU)UPDATE_PAGE, NULL, NULL);
					}

					mysql_query(&mysql, "SELECT * FROM PASSWORD WHERE PASSWORD.ID = 0");
					idZapytania = mysql_store_result(&mysql);
					wiersz = mysql_fetch_row(idZapytania);
					SetWindowText(VERSION, VEROUT.c_str());
					PASS = CreateWindow(
						"EDIT",
						"",
						WS_VISIBLE | WS_CHILD | WS_BORDER | ES_PASSWORD | ES_CENTER,
						150, 120, 300, 20,
						hwnd, NULL, NULL, NULL);
									
					PASSBUTTON = CreateWindow(
						"BUTTON",
						"POTWIERDZ",
						WS_VISIBLE | WS_CHILD | WS_BORDER,
						220, 140, 160, 20,
						hwnd, (HMENU)PASS_SUBMIT, NULL, NULL);
				     	
				    SetFocus(PASS);
				    mysql_close(&mysql);
				}
				else
				{
					string VEROUT = VERSIONGUI + " - NULL";
					SetWindowText(VERSION, VEROUT.c_str());
					SetWindowText(INFO, "[ERROR] Nie mo¿na po³aczyæ siê z baz¹ danych!");
				}
			}

		case WM_COMMAND:
			{
				switch(LOWORD(wParam))
				{
					
				case SHOW_HELP:
					{
						MessageBox(hwnd, "INFORMACJE - DAMIAN KREÑSKI - EDITPAGEGUI\nCo czego wymaga:\n USUÑ: ID's(ODZIELONE: ',' BEZ SPACJI);\n EDYTUJ: NAZWA/LINK chocia¿ 1 wypelnione poprawnie, ID's(TYLKO JEDNO, SAMA CYFRA BEZ SPACJI i ',');\n WCZYTAJ: ID's(TYLKO JEDNO, SAMA CYFRA BEZ SPACJI i ',');\n DODAJ: LINK/NAZWA oba poprawnie uzupe³nione.\n\n NAZWA - ci¹g znaków;\n LINK - standardowy link, nie skrócony, bez zbêdnych funkcji w linku.\n\n KLAWISZE FUNKCYJNE: \"X\" Przy:\n LINK - otwieraj¹ podgl¹d,\n NAME - wyszukuj¹ podan¹ fraze w google.", "INFO - EDITPAGE", MB_OK | MB_TASKMODAL);
						break;
					}
				
		        case IDM_PAGE_OPEN:
		            {
		              	ShellExecuteW(0, 0, L"http://krenski.cba.pl/else/", 0, 0 , SW_SHOW);
		                break;
					}
					
				case CONTACT_ME:
					{
						ShellExecuteW(0, 0, L"https://steamcommunity.com/id/NyggaBytes", 0, 0 , SW_SHOW);
						break;
					}
				case UPDATE_PAGE:
					{
						ShellExecuteW(0, 0, L"http://krenski.cba.pl/else/UPDATE", 0, 0 , SW_SHOW);
						break;
					}
						
		        case IDM_FILE_QUIT:
		            {
		              	SendMessage(hwnd, WM_CLOSE, 0, 0);
		                break;
					}
				case PASS_SUBMIT:
					{
						PassCHECK();
						break;
					}
				case ADD_ID:
					{
						bool NEXT = CheckTEXT(3);
						if (NEXT == true)
						{
							ADD();
							RELOADTMP();
							SHOW();
						} else {
							MessageBox(hwnd, "POLA: LINK, NAME. Nie mog¹ byæ puste!", "ERROR:", MB_OK | MB_TASKMODAL);
						}
						break;
					}
				case REMOVE_ID:
					{
						bool NEXT = CheckTEXT(1);
						if (NEXT == true)
						{
							REMOVE();
							RELOADTMP();
							SHOW();
						} else {
							MessageBox(hwnd, "W liœcie ID podano nie poprawne znaki, poprawne: '0-9' i ','.", "ERROR:", MB_OK | MB_TASKMODAL);
						}
						break;
					}
				case EDIT_ID:
					{
						bool NEXT = CheckTEXT(2);
						if (NEXT == true)
						{
							EDIT();
							RELOADTMP();
							SHOW();
						} else {
							MessageBox(hwnd, "ID mo¿e byæ tylko jedno i/lub podano b³êdne znaki, poprawne: '0-9' i ','.", "ERROR:", MB_OK | MB_TASKMODAL);
						}
						break;
					}
				case READ_ID:
					{
						bool NEXT = CheckTEXT(2);
						if (NEXT == true)
						{
							READ();
						} else {
							MessageBox(hwnd, "ID mo¿e byæ tylko jedno lub podano b³êdne znaki, poprawne: '0-9'.", "ERROR:", MB_OK | MB_TASKMODAL);
						}
						break;
					}
				case FILE_SEND:
					{
						SAVE();
						RELOADTMP();
						SHOW();
						system("DEL /Q outtemp.txt&DEL /Q ytwindowsdata.txt&DEL /Q out.txt");
						break;
					}
				case LINK_OPEN_FORMATTED:
					{
						bool NEXT = CheckTEXT(4);
						if (NEXT == true)
						{
							char link[255];
							GetWindowText(LINK, &link[0], 255);
							ShellExecute(NULL, NULL, (((string)link).c_str()), NULL, NULL, SW_SHOW);
						} else {
							MessageBox(hwnd, "POLE: LINK, Nie mo¿e byæ puste!", "ERROR:", MB_OK | MB_TASKMODAL);
						}
						break;
					}
				case NAME_SEARCH:
					{
						bool NEXT = CheckTEXT(5);
						if (NEXT == true)
						{
							char name[255];
							int LENGTH = GetWindowTextLength(NAME);
							GetWindowText(NAME, &name[0], 255);
							string OUTNAME = "https://www.google.pl/search?q=";
							for(int i = 0; i<LENGTH; i++)
							{
								if (name[i] == ' ') OUTNAME += '+';
								else if (name[i] == '&') OUTNAME += "%26";
								else OUTNAME += name[i];
							}
							
							ShellExecute(NULL, NULL, (((string)OUTNAME).c_str()), NULL, NULL, SW_SHOW);
						} else {
							MessageBox(hwnd, "POLE: NAME, Nie mo¿e byæ puste!", "ERROR:", MB_OK | MB_TASKMODAL);
						}
						break;
					}
		        }
		        
		        break;
			}
			
      	case WM_DESTROY:
      		{
	      		system("DEL /Q outtemp.txt&DEL /Q ytwindowsdata.txt&DEL /Q out.txt");
	      		Sleep(500);
	      		PostQuitMessage(0);
          		break;
			}
  }

  return DefWindowProcW(hwnd, msg, wParam, lParam);
}

void PassCHECK()
{
	char name[255];
	string password = wiersz[0];
	GetWindowText(PASS, &name[0], 255);
	
	if ((string)name != password)
	{
		MessageBox(hwnd, "B³êdne has³o!", "ERROR", MB_OK | MB_TASKMODAL);
	} else {
		startup();
	}
}

void startup()
{
	loginScreen = false;
	DestroyWindow(PASS);
	DestroyWindow(PASSBUTTON);
	DestroyWindow(VERSION);
	DestroyWindow(INFO);
	DestroyWindow(UPDATE);
	AddMenus(hwnd);
	list = CreateWindow(
			"EDIT",
			"",
			WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | ES_READONLY,
			5, 5, 585, 200,
			hwnd, NULL, NULL, NULL);
	CreateWindow(
			"STATIC",
			"ID\'s:",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			5, 205, 34, 20,
			hwnd, NULL, NULL, NULL);
	ID = CreateWindow(
			"EDIT",
			"",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			39, 205, 100, 20,
			hwnd, NULL, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"DODAJ",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			439, 225, 152, 40,
			hwnd, (HMENU) ADD_ID, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"USUÑ",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			138, 205, 152, 20,
			hwnd, (HMENU) REMOVE_ID, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"EDYTUJ",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			288, 205, 152, 20,
			hwnd, (HMENU) EDIT_ID, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"WCZYTAJ",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			440, 205, 152, 20,
			hwnd, (HMENU) READ_ID, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"WYŒLIJ",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			5, 265, 585, 20,
			hwnd, (HMENU) FILE_SEND, NULL, NULL);
	INFOBOX = CreateWindow(
			"STATIC",
			"",
			WS_VISIBLE | WS_CHILD | WS_BORDER | ES_CENTER,
			5, 285, 585, 20,
			hwnd, NULL, NULL, NULL);
	CreateWindow(
			"STATIC",
			"URL:",
			WS_VISIBLE | WS_CHILD | WS_BORDER | SS_RIGHT,
			5, 225, 46, 20,
			hwnd, NULL, NULL, NULL);
	CreateWindow(
			"STATIC",
			"NAME:",
			WS_VISIBLE | WS_CHILD | WS_BORDER | SS_RIGHT,
			5, 245, 46, 20,
			hwnd, NULL, NULL, NULL);
	LINK = CreateWindow(
			"EDIT",
			"",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			51, 225, 368, 20,
			hwnd, NULL, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"X",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			419, 225, 20, 20,
			hwnd, (HMENU) LINK_OPEN_FORMATTED, NULL, NULL);
	NAME = CreateWindow(
			"EDIT",
			"",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			51, 245, 368, 20,
			hwnd, NULL, NULL, NULL);
	CreateWindow(
			"BUTTON",
			"X",
			WS_VISIBLE | WS_CHILD | WS_BORDER,
			419, 245, 20, 20,
			hwnd, (HMENU) NAME_SEARCH, NULL, NULL);
	ftpdwn();
	filelist();
	SHOW();
	if (hwnd != NULL) INTELBOX(1);
}

void AddMenus(HWND hwnd) {

    HMENU hMenubar;
    HMENU hMenu;

    hMenubar = CreateMenu();
    hMenu = CreateMenu();

	AppendMenuW(hMenu, MF_STRING, SHOW_HELP, L"&POMOC");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
	AppendMenuW(hMenu, MF_STRING, CONTACT_ME, L"&Kontakt");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_PAGE_OPEN, L"&Strona");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, IDM_FILE_QUIT, L"&Quit");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hMenu, L"&Opcje");
    SetMenu(hwnd, hMenubar);
}
