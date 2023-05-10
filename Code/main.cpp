#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;

///defines ============================================================= for algorithms
#define Mouse_ARROW 0
#define Mouse_HAND 1
#define DDA 2
#define Midpoint 3
#define Parametric 4
#define ClippingRectanglePoint 5
#define ClippingRectangleLine 6
#define ClippingRectanglePolygon 7
#define ClippingSquarePoint 8
#define ClippingSquareLine 9
///defines =============================================================

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



double GetDistanceBetween2Points(int x1,int x2,int y1,int y2)
{
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

}
COLORREF color=RGB(0, 0, 0); ///default color (black)

///-------------------------------------------code line ------------------
void LineDDA(HDC hdc, int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;
    SetPixel(hdc, x1, y1, color);

    if (abs(dx) >= abs(dy)) //sloop<1
    {
        int x = x1;
        int xinc;
        if (dx > 0)
        {
            xinc = 1;
        }
        else xinc = -1;

        double y = y1, yinc = (double)dy / dx * xinc;
        while (x != x2)
        {
            x += xinc;
            y += yinc;
            SetPixel(hdc, x, round(y), color);
        }
    }
    else
    {
        int y = y1;
        int yinc;
        if (dy > 0)
        {
            yinc = 1;
        }
        else yinc = -1;
        double x = x1, xinc = (double)dx / dy * yinc;
        while (y != y2)
        {
            x += xinc;
            y += yinc;
            SetPixel(hdc, round(x), y, color);
        }
    }

}
void LineMidPoint(HDC hdc, int x1, int y1, int x2, int y2)
{
    int x, y, dx, dy, d, d1, d2;
    dx = x2 - x1;
    dy = y2 - y1;
    if (abs(dy) <= abs(dx)) //sloope < 1
    {
        d = 2*abs(dy)-abs(dx);
        d1= 2*(abs(dy)-abs(dx));
        d2= 2*abs(dy);
        if (dx >= 0)
        {
            x = x1;
            y = y1;
        }
        else
        {
            x = x2;
            y = y2;
            swap(x1,x2);
        }
        SetPixel(hdc, x, y, color);
        while(x<x2)
        {
            if (d < 0)
            {
                d += d2 ;
            }
            else
            {
                d += d1 ;
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) y++;
                else y--;
            }
            x++;
            SetPixel(hdc, x, y, color);
        }
    }
    else
    {
        d = 2*abs(dx)-abs(dy);
        d1= 2*(abs(dx)-abs(dy));
        d2= 2*abs(dx);
        if (dy >= 0)
        {
            x = x1;
            y = y1;
        }
        else
        {
            x = x2;
            y = y2;
            swap(y1,y2);
        }
        SetPixel(hdc, x, y, color);
        while(y<y2)
        {
            if (d <= 0)
            {
                d += d2;
            }
            else
            {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) x++;
                else x--;
                d += d1;
            }
            y++;
            SetPixel(hdc, x, y, color);
        }
    }
}
void LineParametric(HDC hdc, int x1, int y1, int x2, int y2)
{
    for (double t = 0; t <= 1; t += 0.001)
    {
        double x = x1 + t * (x2 - x1);
        double y = y1 + t * (y2 - y1);
        SetPixel(hdc, round(x), round(y), color);
    }
}
///-------------------------------------------code line ------------------


///-------------------------------------------Clipping Algorithms-----------------

//--Point Clipping
void PointClipping(HDC hdc, int x, int y, int xleft, int ytop, int xright, int ybottom)
{
    if (x >= xleft && x <= xright && y >= ytop && y <= ybottom)
        SetPixel(hdc, x, y,color);
}



//--Line Clipping
union OutCode
{
    unsigned All : 4;
    struct{unsigned left : 1, top : 1, right : 1, bottom : 1;};
};
OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom)
{
    OutCode out;
    out.All = 0;
    if (x < xleft)out.left = 1;
    else if (x > xright)out.right = 1;
    if (y < ytop)out.top = 1;
    else if (y > ybottom)out.bottom = 1;
    return out;
}
void VIntersect(double xs, double ys, double xe, double ye, int x, double* xi, double* yi)
{
    *xi = x;
    *yi = ys + (x - xs) * (ye - ys) / (xe - xs);
}
void HIntersect(double xs, double ys, double xe, double ye, int y, double* xi, double* yi)
{
    *yi = y;
    *xi = xs + (y - ys) * (xe - xs) / (ye - ys);
}
void CohenSuth(HDC hdc, int xs, int ys, int xe, int ye, int xleft, int ytop, int xright, int ybottom)
{
    double x1 = xs, y1 = ys, x2 = xe, y2 = ye;//line
    OutCode out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
    OutCode out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
    while ((out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi, yi;
        if (out1.All)
        {
            if (out1.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out1.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out1.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x1 = xi;
            y1 = yi;
            out1 = GetOutCode(x1, y1, xleft, ytop, xright, ybottom);
        }
        else
        {
            if (out2.left)VIntersect(x1, y1, x2, y2, xleft, &xi, &yi);
            else if (out2.top)HIntersect(x1, y1, x2, y2, ytop, &xi, &yi);
            else if (out2.right)VIntersect(x1, y1, x2, y2, xright, &xi, &yi);
            else HIntersect(x1, y1, x2, y2, ybottom, &xi, &yi);
            x2 = xi;
            y2 = yi;
            out2 = GetOutCode(x2, y2, xleft, ytop, xright, ybottom);
        }
    }
    if (!out1.All && !out2.All)
    {
        LineMidPoint(hdc, x1, y1, x2, y2);
    }
}



//--Polygon clipping
struct Vertex
{
    double x, y;
    Vertex(int x1 = 0, int y1 = 0)
    {
        x = x1;
        y = y1;
    }
};
typedef vector<Vertex> VertexList;
typedef bool (*IsInFunc)(Vertex& v, int edge);
typedef Vertex(*IntersectFunc)(Vertex& v1, Vertex& v2, int edge);
VertexList ClipWithEdge(VertexList p, int edge, IsInFunc In, IntersectFunc Intersect)
{
    VertexList OutList;
    Vertex v1 = p[p.size() - 1];
    bool v1_in = In(v1, edge);
    for (int i = 0; i < (int)p.size(); i++)
    {
        Vertex v2 = p[i];
        bool v2_in = In(v2, edge);
        if (!v1_in && v2_in)
        {
            OutList.push_back(Intersect(v1, v2, edge));
            OutList.push_back(v2);
        }
        else if (v1_in && v2_in) OutList.push_back(v2);
        else if (v1_in) OutList.push_back(Intersect(v1, v2, edge));
        v1 = v2;
        v1_in = v2_in;
    }
    return OutList;
}
bool InLeft(Vertex& v, int edge)
{
    return v.x >= edge;
}
bool InRight(Vertex& v, int edge)
{
    return v.x <= edge;
}
bool InTop(Vertex& v, int edge)
{
    return v.y >= edge;
}
bool InBottom(Vertex& v, int edge)
{
    return v.y <= edge;
}
Vertex VIntersect(Vertex& v1, Vertex& v2, int xedge)
{
    Vertex res;
    res.x = xedge;
    res.y = v1.y + (xedge - v1.x) * (v2.y - v1.y) / (v2.x - v1.x);
    return res;
}
Vertex HIntersect(Vertex& v1, Vertex& v2, int yedge)
{
    Vertex res;
    res.y = yedge;
    res.x = v1.x + (yedge - v1.y) * (v2.x - v1.x) / (v2.y - v1.y);
    return res;
}

struct Vector
{
    double v[2];
    Vector(double x = 0, double y = 0)
    {
        v[0] = x;
        v[1] = y;
    }
    double& operator[](int i)
    {
        return v[i];
    }
};

void PolygonClip(HDC hdc, Vector p[], int n, int xleft, int ytop, int xright, int ybottom)
{
    VertexList vlist;
    for (int i = 0; i < n; i++)vlist.push_back(Vertex(p[i][0], p[i][1]));
    vlist = ClipWithEdge(vlist, xleft, InLeft, VIntersect);
    vlist = ClipWithEdge(vlist, ytop, InTop, HIntersect);
    vlist = ClipWithEdge(vlist, xright, InRight, VIntersect);
    vlist = ClipWithEdge(vlist, ybottom, InBottom, HIntersect);
    Vertex v1 = vlist[vlist.size() - 1];
    for (int i = 0; i < (int)vlist.size(); i++)
    {
        Vertex v2 = vlist[i];
        LineDDA(hdc, v1.x, v1.y, v2.x, v2.y);
        v1 = v2;
    }
}
//draw Rectangle code
void drawRectangle(HDC hdc, Vector& p1, double sideLen, double sidewidth, COLORREF c)
{
    double left = p1[0];
    double right = p1[0] + sideLen;
    double top = p1[1];
    double bottom = p1[1] + sidewidth;

    LineMidPoint(hdc,left,top,right,top);
    LineMidPoint(hdc,left,top,left,bottom);
    LineMidPoint(hdc,right,top,right,bottom);
    LineMidPoint(hdc,left,bottom,right,bottom);
}

///-------------------------------------------Clipping Algorithms-----------------





int ListSize=0;
HMENU MyMenu; ///only creates a variable that can hold a reference to a menu that will be created later using the appropriate Windows API functions.
void CreateMenus(HWND hwnd){
    MyMenu =CreateMenu();

    ///(b) Change the shape of your window mouse-----------------------------
    HMENU Mouse = CreateMenu();
    ListSize=2;
    char * Cursors[ListSize] = {"Arrow","Hand"};
    int Cursors2[ListSize] = {Mouse_ARROW,Mouse_HAND};
    for(int i=0;i<ListSize;i++){
        AppendMenu(Mouse,MF_STRING,Cursors2[i],Cursors[i]);
        if(i!=ListSize-1){
            AppendMenu(Mouse,MF_SEPARATOR,NULL,NULL); //separate line
        }
    }

    AppendMenu(MyMenu,MF_POPUP,(UINT_PTR)Mouse,"Mouse");
    ///(b) Change the shape of your window mouse-----------------------------





    ///(i) Implement line algorithms [DDA, Midpoint and parametric------------------
    HMENU Line = CreateMenu();
    ListSize=3;
    char * algorithms[ListSize] = {"DDA","Midpoint","Parametric"};
    int algorithms2[ListSize] = {DDA,Midpoint,Parametric};
    for(int i=0;i<ListSize;i++){
        AppendMenu(Line,MF_STRING,algorithms2[i],algorithms[i]);
        if(i!=ListSize-1){
            AppendMenu(Line,MF_SEPARATOR,NULL,NULL); //separate line
        }
    }
    AppendMenu(MyMenu,MF_POPUP,(UINT_PTR)Line,"Line");
    ///(i) Implement line algorithms [DDA, Midpoint and parametric------------------





    ///(s) Clipping algorithms using Rectangle as Clipping Window[Point ,Line, Polygon]
        HMENU Clipping = CreateMenu();
        //-----rectangle------
        HMENU Rectangle = CreateMenu();
        AppendMenu(Rectangle,MF_STRING,ClippingRectanglePoint,"Point");
        AppendMenu(Rectangle,MF_SEPARATOR,NULL,NULL);
        AppendMenu(Rectangle,MF_STRING,ClippingRectangleLine,"Line");
        AppendMenu(Rectangle,MF_SEPARATOR,NULL,NULL);
        AppendMenu(Rectangle,MF_STRING,ClippingRectanglePolygon,"Polygon");

        //-----Square------
        HMENU Square=CreateMenu();
        AppendMenu(Square,MF_STRING,ClippingSquarePoint,"Point");
        AppendMenu(Square,MF_SEPARATOR,NULL,NULL);
        AppendMenu(Square,MF_STRING,ClippingSquareLine,"Line");


    //add two menus to Clipping menu
        AppendMenu(Clipping,MF_POPUP,(UINT_PTR)Rectangle,"Rectangle");
        AppendMenu(Clipping,MF_SEPARATOR,NULL,NULL);
        AppendMenu(Clipping,MF_POPUP,(UINT_PTR)Square,"Square");

    // add clipping menu to MyMenu
        AppendMenu(MyMenu,MF_POPUP,(UINT_PTR)Clipping,"Clipping algorithms");

    ///(s) Clipping algorithms using Rectangle as Clipping Window[Point ,Line, Polygon

    SetMenu(hwnd,MyMenu);
}


LPCSTR mouse=IDC_ARROW; //default for mouse
int selectt = -1;
int X1,Y1,X2,Y2;//inputs for line
static bool stat = false;
int sides=5; //sides for polygon
LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc= GetDC(hwnd);
    static int click = 0;

    //inputs for clipping
    static int p1,p2,p3,p4,idx=0;
    static double length,width;
    static Vector T;
    static Vector polygon[100];
    //inputs for clipping


    ///(a) Change the background of window to be white
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    ///(a) Change the background of window to be white



    switch (message)                  /* handle the messages */
    {
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                ///select from our menue
                case Mouse_ARROW:
                    mouse = IDC_ARROW;
                    break;
                case Mouse_HAND:
                    mouse = IDC_HAND;
                    break;

                case DDA:
                    selectt = 0;
                    break;
                case Midpoint:
                    selectt = 1;
                    break;
                case Parametric:
                    selectt = 2;
                    break;
                case ClippingRectanglePoint:
                    click=0;
                    selectt = 3;
                    break;
                case ClippingRectangleLine:
                    click=0;
                    selectt = 4;
                    break;
                case ClippingRectanglePolygon:
                    click=0;
                    selectt = 5;
                    break;
                case ClippingSquarePoint:
                    click=0;
                    selectt = 6;
                    break;
                case ClippingSquareLine:
                    click=0;
                    selectt = 7;
                    break;
            }
            break;
        case WM_LBUTTONDOWN:
            X1 = LOWORD(lParam);
            Y1 = HIWORD(lParam);
            break;
        case WM_RBUTTONDOWN:
            X2 = LOWORD(lParam);
            Y2 = HIWORD(lParam);
            cout<<"X2: "<<X2<<" Y2: "<<Y2<<endl;

        ///for line---------------
            if(selectt==0){
                LineDDA(hdc,X1,Y1,X2,Y2);
            }else if(selectt==1){
                LineMidPoint(hdc,X1,Y1,X2,Y2);
            }else if(selectt==2){
                LineParametric(hdc,X1,Y1,X2,Y2);
            }
        ///for line---------------


        ///for clipping Rectangle Point ---------
            else if(selectt==3){
                if(stat==false){ // to check if first time
                    stat = true;
                    click = 0;
                }

                if(click==0){
                    click++;
                    T[0]=LOWORD(lParam);
                    T[1]=HIWORD(lParam);
                    cout<<"T[0]: "<<T[0]<<" T[1]: "<<T[1]<<endl;
                    cout<<click<<endl;
                    break;
                }else if(click==1){
                    click++;
                    p1=LOWORD(lParam);
                    p2=HIWORD(lParam);
                    cout<<"p1: "<<p1<<" p2: "<<p2<<endl;
                    break;
                }else if(click==2){
                    p3 = LOWORD(lParam);
                    p4 = HIWORD(lParam);

                    //get width and length
                    length = GetDistanceBetween2Points(T[0],p1,T[1],p2);
                    width = GetDistanceBetween2Points(T[0],p3,T[1],p4);

                    //finally draw rectangle
                    drawRectangle(hdc,T,length,width,color);
                    click++;
                    break;
                }else if(click==3){
                    //get points for ever (so not increment click)
                    p1 = LOWORD(lParam);
                    p2 = HIWORD(lParam);

                    PointClipping(hdc,p1,p2,T[0],T[1],T[0]+length,T[1]+width);
                    break;
                }
            }
        ///for clipping Rectangle Point ---------



        ///for clipping Rectangle Point ---------
            else if(selectt==4){
                if(click==0){
                    click++;
                    T[0]=LOWORD(lParam);
                    T[1]=HIWORD(lParam);
                    cout<<"T[0]: "<<T[0]<<" T[1]: "<<T[1]<<endl;
                    cout<<click<<endl;
                    break;
                }else if(click==1){
                    click++;
                    p1=LOWORD(lParam);
                    p2=HIWORD(lParam);
                    cout<<"p1: "<<p1<<" p2: "<<p2<<endl;
                    break;
                }else if(click==2){
                    p3 = LOWORD(lParam);
                    p4 = HIWORD(lParam);

                    //get width and length
                    length = GetDistanceBetween2Points(T[0],p1,T[1],p2);
                    width = GetDistanceBetween2Points(T[0],p3,T[1],p4);

                    //finally draw rectangle
                    drawRectangle(hdc,T,length,width,color);
                    click++;
                    break;
                }else if(click==3){
                    click++;
                    p1=LOWORD(lParam);
                    p2= HIWORD(lParam);
                    break;
                }else if(click==4){
                    p3=LOWORD(lParam);
                    p4= HIWORD(lParam);
                    CohenSuth(hdc,p1,p2,p3,p4,T[0],T[1],T[0]+length,T[1]+width);

                    click=3;//to get another line
                    break;
                }
            }
        ///for clipping Rectangle Point ---------




        ///for clipping Rectangle Polygon ---------
            else if(selectt==5){
                if(stat==false){
                    stat=true;
                }

                if(click==0){
                    click++;
                    T[0]=LOWORD(lParam);
                    T[1]=HIWORD(lParam);
                    cout<<"T[0]: "<<T[0]<<" T[1]: "<<T[1]<<endl;
                    cout<<click<<endl;
                    break;
                }else if(click==1){
                    click++;
                    p1=LOWORD(lParam);
                    p2=HIWORD(lParam);
                    cout<<"p1: "<<p1<<" p2: "<<p2<<endl;
                    break;
                }else if(click==2){
                    p3 = LOWORD(lParam);
                    p4 = HIWORD(lParam);

                    //get width and length
                    length = GetDistanceBetween2Points(T[0],p1,T[1],p2);
                    width = GetDistanceBetween2Points(T[0],p3,T[1],p4);

                    //finally draw rectangle
                    drawRectangle(hdc,T,length,width,color);
                    click++;
                    break;
                }else if(click==3){
                    polygon[idx] = Vector(LOWORD(lParam), HIWORD(lParam));
                    if(idx!=sides-1){
                        idx++;
                    }else {
                        PolygonClip(hdc,polygon,sides,T[0],T[1],T[0]+length,T[1]+width);
                        idx=0;
                        stat=false;
                    }
                    break;
                }
            }
        ///for clipping Rectangle Polygon ---------



        ///for clipping Square Point ---------
            else if(selectt==6){
                if(click==0){
                    click++;
                    T[0]=LOWORD(lParam);
                    T[1]=HIWORD(lParam);
                    cout<<"T[0]: "<<T[0]<<" T[1]: "<<T[1]<<endl;
                    cout<<click<<endl;
                    break;
                }else if(click==1){
                    click++;
                    p1=LOWORD(lParam);
                    p2=HIWORD(lParam);
                    length = GetDistanceBetween2Points(T[0],p1,T[1],p2);
                    drawRectangle(hdc,T,length,length,color);
                    break;
                }else if(click==2){
                    //get points for ever (so not increment click)
                    p1=LOWORD(lParam);
                    p2=HIWORD(lParam);
                    PointClipping(hdc,p1,p2,T[0],T[1],T[0]+length,T[1]+length);
                    break;
                }
            }
        ///for clipping Square Point ---------



        ///for clipping Square Line ---------
        else if(selectt==7){
            if(click==0){
                click++;
                T[0]=LOWORD(lParam);
                T[1]=HIWORD(lParam);
                cout<<"T[0]: "<<T[0]<<" T[1]: "<<T[1]<<endl;
                cout<<click<<endl;
                break;
            }else if(click==1){
                click++;
                p1=LOWORD(lParam);
                p2=HIWORD(lParam);
                length = GetDistanceBetween2Points(T[0],p1,T[1],p2);
                drawRectangle(hdc,T,length,length,color);
                break;
            }else if(click==2){
                click++;
                p1=LOWORD(lParam);
                p2= HIWORD(lParam);
                break;
            }else if(click==3){
                p3=LOWORD(lParam);
                p4= HIWORD(lParam);
                CohenSuth(hdc,p1,p2,p3,p4,T[0],T[1],T[0]+length,T[1]+length);

                click=2;//to get another line
                break;
            }
        }
        ///for clipping Square Line ---------
            break;
        case WM_SETCURSOR:
            SetCursor(LoadCursor(NULL,mouse));
            break;
        case WM_CREATE:
            CreateMenus(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


