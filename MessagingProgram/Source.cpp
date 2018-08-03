#include<iostream>
#include<string>
#include<sstream>
#include<Windows.h>
#include<Windowsx.h>
#include<tchar.h>
#include<conio.h>
#include<stdio.h>
#include<stdlib.h>


#define clearScreen() system("cls")
using namespace std;



enum {USERNAMEDIALOG, 
      USERNAMEBUTTON, 
      MESSAGEBOX, 
      MESSAGEDIALOG,
      MESSAGEBUTTON,
      USERNAMETITLE};



enum {SPEAK   = 1, 
      URL     = 2, 
      MESSAGE = 3, 
      EXIT    = 4};


static TCHAR  szWindowClass[] = _T("win32app");
static TCHAR  szTitle[] = _T("FrankAdamMessaging");
TCHAR         test[100] = "\ntesting\0";
TCHAR         messageBuffer[256];
TCHAR         screenBuffer[1024];
TCHAR         username[24];
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK messageTextBoxProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK messageProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK usernameProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK messageInputBoxProc(HWND, UINT, WPARAM, LPARAM);



void messageMenu();
void getUsername(HWND);
void setupMessaging(HWND);
int  printMenu();
int  printMenu();
void destroyUsernameWindows(HWND);




int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                     LPSTR lpCmdLine, int nCmdShow) {
	
	WNDCLASSEX wc;

  	// SET UP WINDOW CLASS
	wc.cbSize         = sizeof(WNDCLASSEX);
	wc.style          = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc    = WndProc;
	wc.cbClsExtra     = 0;
	wc.cbWndExtra     = 0;
	wc.hInstance      = hInstance;
	wc.hIcon          = LoadIcon(hInstance, IDI_APPLICATION);
	wc.hCursor        = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground  = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName   = NULL;
	wc.lpszClassName  = szWindowClass;
	wc.hIconSm        = LoadIcon(wc.hInstance, IDI_APPLICATION);

  hInst = hInstance;

	if (!RegisterClassEx(&wc)) {
		MessageBox(
			NULL, 
			_T("Call to RegisterClassEx failed!"),
			_T("Window Desktop Guided Tour"), 
			SW_SHOW);

		return 1;
	}

	HWND hwnd = CreateWindow(
		            szWindowClass,
		            szTitle,
		            WS_SYSMENU | WS_CAPTION,
		            CW_USEDEFAULT, CW_USEDEFAULT,
		            500, 500,
		            NULL,
		            NULL,
		            hInstance,
		            NULL);
    


	if (!hwnd) {
		MessageBox(
			NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			SW_SHOW);

		return 1;
	}


  //ShowWindow(hwnd, SW_SHOWDEFAULT);
  UpdateWindow(hwnd);

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage (&msg);
	}

	return (int)msg.wParam;

}



LRESULT CALLBACK WndProc(HWND hwnd, UINT Msg, 
                         WPARAM wParam, LPARAM lParam) {
                                
  switch (Msg) {

    case WM_CREATE:
      getUsername(hwnd);      
      return 0;
      

    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;			
        
  }
  return DefWindowProc(hwnd, Msg, wParam, lParam);        
  
}



LRESULT CALLBACK messageTextBoxProc(HWND hwnd, UINT Msg, 
                                 WPARAM wParam, LPARAM lParam) {

  PAINTSTRUCT    ps;
  static HBRUSH  hBrush;
  static HRGN    bgRgn;
  static RECT    clientRect;
  static HDC     hdc;
  static int     numLines{0};


  switch (Msg) {
    
    case WM_CREATE:
      InvalidateRect(hwnd, &clientRect, false);
      break;;
    
    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      SetBkMode(hdc, TRANSPARENT);
      SetTextColor(hdc, RGB(255,255,255));
      GetClientRect(hwnd, &clientRect);
      bgRgn = CreateRectRgnIndirect(&clientRect);
      hBrush = CreateSolidBrush(RGB(75, 53, 118));
      FillRgn(hdc, bgRgn, hBrush);
      DrawText(hdc, screenBuffer, strlen(screenBuffer), &clientRect, 0);
      EndPaint(hwnd, &ps);
      ++numLines;
      break;

    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    
  }
  return DefWindowProcW(hwnd, Msg, wParam, lParam);

}



LRESULT CALLBACK messageProc(HWND hwnd, UINT Msg, 
                             WPARAM wParam, LPARAM lParam) {
  
  HDC           hdc;
  PAINTSTRUCT   ps;
  static RECT   clientRect;
  static HRGN   bgRgn;
  static HBRUSH hBrush;
  static string tmp, tmp2;
  
  
  
  switch (Msg) {
    
    case WM_CREATE:
      hdc = GetDC(hwnd);
      InvalidateRect(hwnd,NULL,false);
      ReleaseDC(hwnd, hdc);
      break;

    case WM_COMMAND:      
      if(MESSAGEBUTTON == LOWORD(wParam)){
          GetDlgItemText(hwnd, MESSAGEDIALOG, messageBuffer, 24);
          tmp = string{username + string{": "} + 
            messageBuffer + '\n' + '\0'}.c_str();
          strncat_s(screenBuffer, tmp.c_str(), tmp.size());
          InvalidateRect(hwnd,&clientRect,true);
      } break;        

    case WM_KEYDOWN:      
      if (LOWORD(wParam) == VK_RETURN) {
          InvalidateRect(hwnd,&clientRect,true);
      } break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &clientRect);
      bgRgn = CreateRectRgnIndirect(&clientRect);
      hBrush = CreateSolidBrush(RGB(45, 23, 88));
      FillRgn(hdc, bgRgn, hBrush);
      EndPaint(hwnd, &ps);
      break;
   
  }
  return DefWindowProcW(hwnd, Msg, wParam, lParam);
}


LRESULT CALLBACK usernameProc(HWND hwnd, UINT Msg, 
                             WPARAM wParam, LPARAM lParam) {
  
  PAINTSTRUCT    ps;
  static HBRUSH  hBrush;
  static HRGN    bgRgn;
  static RECT    clientRect;
  static HDC     hdc;
  
  
  switch (Msg) {
    case WM_CREATE:
      hdc = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
      DrawText(hdc, username, strlen(username), NULL, NULL);
      InvalidateRect(hwnd, &clientRect, false);
      ReleaseDC(hwnd, hdc);
      break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &clientRect);
      bgRgn = CreateRectRgnIndirect(&clientRect);
      hBrush = CreateSolidBrush(RGB(45, 23, 88));
      FillRgn(hdc, bgRgn, hBrush);
      EndPaint(hwnd, &ps);
      break;
      
    case WM_CLOSE:
      DestroyWindow(hwnd);      
      break;

    case WM_COMMAND:

      switch (LOWORD(wParam)) {
        
        case USERNAMEBUTTON:          
          GetDlgItemText(hwnd, USERNAMEDIALOG, username, 24);                              
          DestroyWindow(hwnd);
          setupMessaging(GetParent(hwnd));                    
          break;

        default: 
          break;
      }  
      break;
        
  }
  return DefWindowProcA(hwnd,Msg,wParam,lParam);
}

LRESULT CALLBACK messageInputBoxProc(HWND hwnd, UINT Msg, 
                                     WPARAM wParam, LPARAM lParam) {
  
  PAINTSTRUCT    ps;
  static HBRUSH  hBrush;
  static HRGN    bgRgn;
  static RECT    clientRect;
  static HDC     hdc;

  switch (Msg) {
    case WM_CREATE:      
      break;
    
    case WM_KEYDOWN:
      if (LOWORD(wParam) == VK_RETURN) {
        SendMessage(hwnd, WM_PAINT, wParam, lParam);
      }
      break;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &clientRect);
      bgRgn = CreateRectRgnIndirect(&clientRect);
      hBrush = CreateSolidBrush(RGB(90, 145, 88));
      FillRgn(hdc, bgRgn, hBrush);
      EndPaint(hwnd, &ps);
      break;
  }
  return DefWindowProcW(hwnd,Msg,wParam,lParam);
}

void getUsername(HWND hwnd) {
  
  WNDCLASSW usernameWin = {0};  
  usernameWin.lpfnWndProc    = usernameProc;		
  usernameWin.hInstance      = hInst;
  usernameWin.hCursor        = LoadCursor(NULL, IDC_ARROW);
  usernameWin.hbrBackground  = (HBRUSH)(GRAY_BRUSH);
  usernameWin.lpszMenuName   = NULL;
  usernameWin.style          = CS_HREDRAW | CS_VREDRAW;
  usernameWin.lpszClassName  = L"usernameClass";

  RegisterClassW(&usernameWin);

  HWND displayWin =     
      CreateWindowW(
        L"usernameClass", _T(L"Get Username"),
        WS_VISIBLE | WS_CAPTION | WS_SYSMENU,
        200, 200, 400, 150, 
        hwnd, NULL, NULL, NULL);  

  CreateWindowW(L"EDIT", L" ENTER USERNAME",
                WS_VISIBLE | WS_CHILD | ES_READONLY, 
                120, 15, 140, 20, displayWin,
                (HMENU)USERNAMETITLE, NULL, NULL);

  CreateWindowW( 
        L"EDIT", NULL,
        WS_VISIBLE | WS_CHILD | ES_WANTRETURN,
        20, 40, 340, 20, displayWin,
        (HMENU)USERNAMEDIALOG, NULL, NULL);            

  CreateWindowW(
        L"Button", L"Enter",
        WS_VISIBLE | WS_CHILD,
        140, 75, 100, 20, displayWin,
        (HMENU)USERNAMEBUTTON, NULL, NULL);
}

void setupMessaging(HWND hwnd) {

  WNDCLASSW messageClass    = {0};
  WNDCLASSW textBoxClass    = {0};
  WNDCLASSW messageInputBox = {0};

  messageClass .lpfnWndProc   = messageProc;
  messageClass .hInstance     = hInst;
  messageClass .hCursor       = LoadCursor(NULL, IDC_ARROW);
  messageClass .lpszMenuName  = NULL;
  messageClass .lpszClassName = L"messageClass";
  RegisterClassW(&messageClass);

  textBoxClass.lpfnWndProc    = messageTextBoxProc;		
  textBoxClass.hInstance      = hInst;
  textBoxClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
  textBoxClass.hbrBackground  = (HBRUSH)(GRAY_BRUSH);
  textBoxClass.lpszMenuName   = NULL;
  textBoxClass.style          = CS_HREDRAW | CS_VREDRAW;
  textBoxClass.lpszClassName  = L"textBoxClass";
  RegisterClassW(&textBoxClass);
  
  textBoxClass.lpfnWndProc    = messageInputBoxProc;		
  textBoxClass.hInstance      = hInst;
  textBoxClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
  textBoxClass.hbrBackground  = (HBRUSH)(GRAY_BRUSH);
  textBoxClass.lpszMenuName   = NULL;
  textBoxClass.style          = CS_HREDRAW | CS_VREDRAW;
  textBoxClass.lpszClassName  = L"inputMessageInputClass";
  RegisterClassW(&textBoxClass);


  int margin = 12;

  HWND messageWin =     
      CreateWindowW(
        L"messageClass", L"Messaging",
        WS_VISIBLE | WS_BORDER,
        400, 400, 408, 544, hwnd,
        NULL, NULL, NULL); 

  CreateWindowW(
        L"EDIT", L"inputBox",
        WS_VISIBLE | WS_CHILD,
        margin, 376 + (margin + 11), 293, 93, 
        messageWin, (HMENU)MESSAGEDIALOG, NULL, NULL);  
 
  CreateWindowW(
        L"textBoxClass", L"messageBox",
        WS_VISIBLE | WS_CHILD,
        margin, margin, 367, 375, messageWin,
        (HMENU)MESSAGEBOX, NULL, NULL);  
        
  CreateWindowW(
        L"Button", L"Send",
        WS_VISIBLE | WS_CHILD,
        307, 376 + (margin + 11),
        73, 94, messageWin,
        (HMENU)MESSAGEBUTTON, NULL, NULL);  
}

void messageMenu(){

  clearScreen();

  while(1){
        
    switch(printMenu()){
      
      case SPEAK: {        
        ostringstream out{};
        string message{};
        out << "-ExecutionPolicy Bypass -NoProfile";
        out << "-NonInteractive -WindowStyle Hidden -File ";                
        out << "\"speak.ps1\" \"";
                
        clearScreen();
        printf("\n\n\tEnter Message: ");
        getline(cin, message);
        out << message << "\"";
        clearScreen();         
        
        printf("\n\n\t>> Synthesizing \"%s\"", message.c_str());
        ShellExecute(NULL, 
                     "open", 
                     "powershell.exe", 
                     out.str().c_str(), 
                     NULL, 
                     SW_HIDE);
        break;     
      }      
      case URL: {
        string url{};        
        ostringstream out{};
        
        clearScreen();
        printf("\n\n\tEnter URL: ");
        getline(cin, url);

        printf("\n\n\t\t1 - Save to Clipboard");
        printf("\n\t\t2 - Open Webpage");

        switch(_getch()) {
          
          case '1':
            out << "-ExecutionPolicy Bypass -NoProfile -NonInteractive ";
            out << "-WindowStyle Hidden Set-Clipboard -Value " << url;    
            ShellExecute(NULL, 
                         "open", 
                         "powershell.exe", 
                         out.str().c_str(), 
                         NULL, 
                         SW_HIDE); 
            clearScreen();
            printf("\n\n\t>> URL saved to clipboard.");
            break;
          
          case '2':
            printf("\n\n\t>> Opening %s...", url.c_str());
            ShellExecute(NULL, 
                         "open", 
                         "chrome.exe", 
                         url.c_str(), 
                         NULL, 
                         SW_MINIMIZE);            
            break;
          
          default: break;
        }
        
        break;
      }
      case MESSAGE: {
        string message;

        clearScreen();
        printf("\n\n\tEnter Message: ");
        getline(cin, message);
        while (MessageBox(NULL, message.c_str(), NULL, MB_YESNO) == IDABORT) {
          MessageBox(NULL, "How dare you 'X' that MessageBox", NULL, MB_OK);
        }        
        break;
      }
      case EXIT:        
        exit(1);
        break;
      
      default: break;

    }
  }
}

int printMenu(){

  printf("\n\n\tSelect Item to Send:\n");
  printf("\n\t1 - Speech");
  printf("\n\t2 - URL");
  printf("\n\t3 - Message");
  printf("\n\t4 - Exit");

  return _getch() - 48;
}
