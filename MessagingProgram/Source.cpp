#include<iostream>
#include<string>
#include<sstream>
#include<Windows.h>
#include<Windowsx.h>
#include<tchar.h>
#include<conio.h>
#include<io.h>
#include<fcntl.h>
#include<fstream>


#define clearScreen() system("cls")
using namespace std;



enum {USERNAMEDIALOG, 
      USERNAMEBUTTON, 
      MESSAGEBOX, 
      MESSAGEDIALOG,
      MESSAGEBUTTON};



enum {SPEAK   = 1, 
      URL     = 2, 
      MESSAGE = 3, 
      EXIT    = 4};



TCHAR         test[100] = "\ntesting\0";
TCHAR         messageBuffer[256];
TCHAR         screenBuffer[1024];
TCHAR         username[24];
HINSTANCE hInst;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK messageTextBoxProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK messageProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK usernameProc(HWND, UINT, WPARAM, LPARAM);



void registerChildWindows();
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
	wc.lpszClassName  = "win32app";
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
		            "win32app",
		            "FrankAdamMessaging",
		            WS_VISIBLE | WS_CAPTION,
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


  ShowWindow(hwnd, SW_SHOWDEFAULT);
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
    
    default:
      return DefWindowProc(hwnd, Msg, wParam, lParam);        
  }
  
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
      return 0;
    
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
      return 0;      

    case WM_CLOSE:
      DestroyWindow(hwnd);
      return 0;

    default:
      return DefWindowProcW(hwnd, Msg, wParam, lParam);
  }

}



LRESULT CALLBACK messageProc(HWND hwnd, UINT Msg, 
                             WPARAM wParam, LPARAM lParam) {
  
  HDC           hdc;
  PAINTSTRUCT   ps;
  static RECT   clientRect;
  static HRGN   bgRgn;
  static HBRUSH hBrush;

  switch (Msg) {
    
    case WM_CREATE:
      hdc = GetDC(hwnd);
      InvalidateRect(hwnd,NULL,false);
      ReleaseDC(hwnd, hdc);
      return 0;

    case WM_COMMAND:
      
      switch (LOWORD(wParam)) {
        case MESSAGEBUTTON:
          GetDlgItemText(hwnd, MESSAGEDIALOG, messageBuffer, 24);
          const char* tmp = string{username + string{": "} + messageBuffer + '\n'}.c_str();
          strncat_s(screenBuffer, tmp, strlen(tmp));
          SetDlgItemText(GetWindow(hwnd,MESSAGEDIALOG), MESSAGEDIALOG, "");
          InvalidateRect(hwnd,&clientRect,true);
          break;
      }
      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &clientRect);
      bgRgn = CreateRectRgnIndirect(&clientRect);
      hBrush = CreateSolidBrush(RGB(45, 23, 88));
      FillRgn(hdc, bgRgn, hBrush);
      EndPaint(hwnd, &ps);
      return 0;
    
    default: 
      return DefWindowProcW(hwnd, Msg, wParam, lParam);
  }
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
      hdc = GetDC(hwnd);
      InvalidateRect(hwnd, &clientRect, false);
      ReleaseDC(hwnd, hdc);
      return 0;

    case WM_PAINT:
      hdc = BeginPaint(hwnd, &ps);
      GetClientRect(hwnd, &clientRect);
      bgRgn = CreateRectRgnIndirect(&clientRect);
      hBrush = CreateSolidBrush(RGB(45, 23, 88));
      FillRgn(hdc, bgRgn, hBrush);
      EndPaint(hwnd, &ps);
      
    case WM_CLOSE:
      DestroyWindow(hwnd);      
      return 0;

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
      return 0;
    
    default:
      DefWindowProcA(hwnd,Msg,wParam,lParam);
  }
}

void getUsername(HWND hwnd) {
  
  WNDCLASSEX usernameWin = {0};

  // SET UP WINDOW CLASS  
	usernameWin.lpfnWndProc    = usernameProc;		
	usernameWin.hInstance      = hInst;
	usernameWin.hCursor        = LoadCursor(NULL, IDC_ARROW);
	usernameWin.hbrBackground  = (HBRUSH)(GRAY_BRUSH);
	usernameWin.lpszMenuName   = NULL;
  usernameWin.style          = CS_HREDRAW | CS_VREDRAW;
	usernameWin.lpszClassName  = "usernameClass";

  RegisterClassEx(&usernameWin);

  HWND displayWin =     
      CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_DLGMODALFRAME, 
        L"usernameClass", L"Get Username",
        WS_VISIBLE | WS_CAPTION,
        400, 400, 400, 388, 
        hwnd, NULL, NULL, NULL);  

  CreateWindowW( 
        L"EDIT", L"Enter Username",
        WS_VISIBLE | WS_CHILD | ES_WANTRETURN,
        20, 200, 325, 20, displayWin,
        (HMENU)USERNAMEDIALOG, NULL, NULL);            

  CreateWindowW(
        L"Button", L"Enter",
        WS_VISIBLE | WS_CHILD,
        20, 235, 50, 20, displayWin,
        (HMENU)USERNAMEBUTTON, NULL, NULL);
}

void setupMessaging(HWND hwnd) {

  WNDCLASSEX messageClass = {0};
  WNDCLASSEX textBoxClass = {0};

  messageClass .lpfnWndProc   = messageProc;
  messageClass .hInstance     = hInst;
  messageClass .hCursor       = LoadCursor(NULL, IDC_ARROW);
	messageClass .lpszMenuName  = NULL;
	messageClass .lpszClassName = "messageClass";
  RegisterClassEx(&messageClass);

	textBoxClass.lpfnWndProc    = messageTextBoxProc;		
	textBoxClass.hInstance      = hInst;
	textBoxClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
	textBoxClass.hbrBackground  = (HBRUSH)(GRAY_BRUSH);
	textBoxClass.lpszMenuName   = NULL;
  textBoxClass.style          = CS_HREDRAW | CS_VREDRAW;
	textBoxClass.lpszClassName  = "textBoxClass";
  RegisterClassEx(&textBoxClass);
  


  int margin = 22;

  HWND messageWin =     
      CreateWindowExW(
        WS_EX_APPWINDOW,
        L"messageClass", L"Messaging",
        WS_VISIBLE | WS_CAPTION,
        400, 400, 400, 388, hwnd,
        NULL, NULL, NULL); 
  
  CreateWindowW(
        L"EDIT", L"TYPE HERE",
        WS_VISIBLE | WS_CHILD | ES_WANTRETURN,
        margin, 225 + (margin + 10), 350, 40, 
        messageWin, (HMENU)MESSAGEDIALOG, NULL, NULL);  

  HWND textBoxWin = 
    CreateWindowExW(
        WS_EX_APPWINDOW | WS_EX_STATICEDGE | WS_EX_WINDOWEDGE,
        L"textBoxClass", L"EDIT",
        WS_VISIBLE | WS_CHILD | WS_VSCROLL | ES_READONLY,
        margin, margin, 350, 225, messageWin,
        (HMENU)MESSAGEBOX, NULL, NULL);  
        
  CreateWindowW(
        L"Button", L"Send",
        WS_VISIBLE | WS_CHILD,
        margin, 265 + (margin + 20),
        100, 30, messageWin,
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
