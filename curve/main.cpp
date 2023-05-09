#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;
/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           544,                 /* The programs width */
           375,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

/////////////////////////////////////////
void hermiteCurve(HDC hdc,int x_0,int sx_0,int x_1,int sx_1,int y_0,int sy_0,int y_1,int sy_1){
    int alf1=2*x_0+1*sx_0-2*x_1+sx_1;
    int beta1=-3*x_0-2*sx_0+3*x_1-sx_1;
    int gamm1=sx_0;
    int sigma1=x_0;

    int alf2=2*y_0+1*sy_0-2*y_1+sy_1;
    int beta2=-3*y_0-2*sy_0+3*y_1-sy_1;
    int gamm2=sy_0;
    int sigma2=y_0;
    for(double t=0;t<=1;t+=0.001){
        double xt=(t*t*t*alf1)+(t*t*beta1)+(t*gamm1)+sigma1;
        double yt=(t*t*t*alf2)+(t*t*beta2)+(t*gamm2)+sigma2;

        SetPixel(hdc,xt,yt,RGB(255,50,2));
    }
}
/*  This function is called by the Windows function DispatchMessage()  */
int cnt=1;
bool status;
  // curve
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc = GetDC(hwnd);
    int x0,sx0,x1,sx1,y0,sy0,y1,sy1;
    if(status){
        hermiteCurve(hdc,x0,sx0,x1,sx1,y0,sy0,y1,sy1);
    }
    //hermiteCurve(hdc,50,100,150,200,300,400,500,550);
    switch (message)                  /* handle the messages */
    {

        case WM_LBUTTONDBLCLK:
           if (cnt == 1) {
                x0 = LOWORD(lParam);
                y0 = HIWORD(lParam);
                cnt++;
                InvalidateRect(hwnd, NULL, false);
            }
            else if (cnt == 2) {
                sx0 = LOWORD(lParam);
                sy0 = HIWORD(lParam);
                cnt++;
                InvalidateRect(hwnd, NULL, false);
            }
            else if (cnt == 3) {
                x1 = LOWORD(lParam);
                y1 = HIWORD(lParam);
                cnt++;
                InvalidateRect(hwnd, NULL, false);
            }
            else if (cnt == 4) {
                sx1 = LOWORD(lParam);
                sy1 = HIWORD(lParam);
                status = true;
                InvalidateRect(hwnd, NULL, false);

            }
           break;

        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
