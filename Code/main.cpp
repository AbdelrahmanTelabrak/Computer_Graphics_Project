#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <bits/stdc++.h>
using namespace std;

#define MAXINT 1e9

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
#define ConvexFilling 10
#define GeneralFilling 11
#define FloodFilling 12
#define FloodFillingRecursive 13
#define CircleLines 14
#define CircleCircles 15
#define ColorButton 16
#define HermiteSquare 17
#define BezierRectangle 18
#define SplineCurve 19
#define EllipseDirect 20
#define EllipsePolar 21
#define EllipseMidPoint 22

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
           800,                 /* The programs width */
           600,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    HWND buttonHandle = CreateWindow(
            _T("BUTTON"),          // button class name
            _T("Color"),        // button text
            WS_VISIBLE | WS_CHILD, // button styles
            710, 0, 80, 25,     // button position and size
            hwnd,                  // parent window handle
            (HMENU) ColorButton,              // button identifier
            NULL,                  // instance handle
            NULL                   // no additional data
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


///-------------------------------------------Filling Algorithms-------------------

//---------------Convex Filling-------------
vector<POINT> points;
int numberOfPoints;
const int MAXENTRIES = 600;

void InitEntries(vector<pair<int, int>>& table)
{
    for(int i=0; i < MAXENTRIES;i++)
    {
        pair<int, int> e;e.first = MAXINT;e.second=-1;
        table.push_back(e);
    }
}

void ScanEdge(POINT v1,POINT v2,vector<pair<int, int>>& table)
{
    if(v1.y==v2.y)return;
    if(v1.y>v2.y)swap(v1,v2);
    double minv=(double)(v2.x-v1.x)/(v2.y-v1.y);
    double x=v1.x;
    int y=v1.y;
    while(y<v2.y)
    {
        if(x<table[y].first)
            table[y].first=(int)ceil(x);
        if(x>table[y].second)
            table[y].second=(int)floor(x);
        cout<<table[y].first<<" "<<table[y].second<<"\n";
        y++;
        x+=minv;
    }
}

void DrawSanLines(HDC hdc,vector<pair<int, int>> table,COLORREF color)
{
    for(int y=0;y<MAXENTRIES;y++)
        if(table[y].first<table[y].second)
            LineDDA(hdc, table[y].first, y, table[y].second, y);
}

void ConvexFill(HDC hdc,vector<POINT> p,int n,COLORREF color)
{
    vector<pair<int, int>> table;
    InitEntries(table);
    POINT v1=p[n-1];
    for(int i=0;i<n;i++)
    {
        cout<<"in loop\n";
        POINT v2=p[i];
        ScanEdge(v1,v2,table);
        v1=p[i];
    }
    DrawSanLines(hdc,table,color);
}
//--------------------------------------------------

//----------------General Filling-------------------

class EdgeInfo {
public:
    double xmin, minv;
    int ymax;

    EdgeInfo() {}

    EdgeInfo(double xmin, double minv, int ymax) : xmin(xmin), minv(minv), ymax(ymax) {}
};

bool compareEdge(const EdgeInfo &edge1, const EdgeInfo &edge2) {
    return edge1.xmin < edge2.xmin;
}
void drawOutline(HDC hdc, vector<POINT> points){
    COLORREF c = RGB(0,0,0);
    for (int i = 0; i < points.size(); ++i) {
        if (i==points.size()-1)
            LineDDA(hdc, points[i].x, points[i].y, points[0].x, points[0].y);
        else
            LineDDA(hdc, points[i].x, points[i].y, points[i+1].x, points[i+1].y);
    }
}

EdgeInfo InitEdgeInfo(POINT &v1, POINT &v2) {
    if (v1.y > v2.y)swap(v1, v2);
    EdgeInfo info = EdgeInfo();
    info.xmin = v1.x;
    info.ymax = v2.y;
    info.minv = (double) (v2.x - v1.x) / (v2.y - v1.y);
    return info;
}

void InitEdgeTable(vector<POINT> points, int n, list<EdgeInfo> table[]) {
    POINT v1 = points[n - 1];
    for (int i = 0; i < n; i++) {
        POINT v2 = points[i];
        if (v1.y == v2.y) {
            v1 = v2;
            continue;
        }
        EdgeInfo rec = InitEdgeInfo(v1, v2);
        table[v1.y].push_back(rec);
        v1 = points[i];
    }
}

void GeneralFill(HDC hdc, vector<POINT> points, int n, COLORREF c) {
    drawOutline(hdc, points);
    auto *table = new list<EdgeInfo>[MAXENTRIES];
    InitEdgeTable(points, n, table);
    int y = 0;
    while (y < MAXENTRIES && table[y].size() == 0)y++;
    if (y == MAXENTRIES)return;
    list<EdgeInfo> ActiveList = table[y];
    while (!ActiveList.empty()) {
        ActiveList.sort(compareEdge);
        for (auto it = ActiveList.begin(); it != ActiveList.end(); it++) {
            int x1 = (int) ceil(it->xmin);
            it++;
            int x2 = (int) floor(it->xmin);
            for (int x = x1; x <= x2; x++)SetPixel(hdc, x, y, c);
        }
        y++;
        auto it = ActiveList.begin();
        while (it != ActiveList.end())
            if (y == it->ymax) it = ActiveList.erase(it); else it++;
        for (auto it = ActiveList.begin(); it != ActiveList.end(); it++)
            it->xmin += it->minv;
        ActiveList.insert(ActiveList.end(), table[y].begin(), table[y].end());
    }
    cout<<ActiveList.size()<<'\n';
    delete[] table;
}

//-----------------------------------------------------


/*********************************/
int Round(double x) {
    return (int)(x + 0.5);
}
class Point {
public:
    float x, y;
    Point() : x(0), y(0) {}

    Point(float x, float y) : x(x), y(y) {}

    Point(const Point& other) : x(other.x), y(other.y) {}

};

class Vector4 {
public:
    Point p1, p2, p3, p4;
    Vector4(const Point& p0, const Point& p1, const Point& p2, const Point& p3): p1(p1), p2(p2), p3(p3), p4(p4) {}

};

//------------------Filling Rectangle with Bezier Curve-------------
void drawBezier(HDC hdc, const Vector4& bezier, COLORREF color, int limitX, int limitY) {
    int a3 = -bezier.p1.x + 3 * bezier.p2.x - 3 * bezier.p3.x + bezier.p4.x;
    int a2 = 3 * bezier.p1.x - 6 * bezier.p2.x + 3 * bezier.p3.x;
    int a1 = -3 * bezier.p1.x + 3 * bezier.p2.x;
    int a0 = bezier.p1.x;

    int b3 = -bezier.p1.y + 3 * bezier.p2.y - 3 * bezier.p3.y + bezier.p4.y;
    int b2 = 3 * bezier.p1.y - 6 * bezier.p2.y + 3 * bezier.p3.y;
    int b1 = -3 * bezier.p1.y + 3 * bezier.p2.y;
    int b0 = bezier.p1.y;

    double dt = 0.0001;
    for (double t = 0; t <= 1; t += dt)
    {
        int x = a0 + a1 * t + a2 * t * t + a3 * t * t * t;
        int y = b0 + b1 * t + b2 * t * t + b3 * t * t * t;
        if (x > limitX && y > limitY)
            SetPixel(hdc, x, y, color);

    }
}

void fillingRectangleWithBezier(HDC hdc, Point RecPoint1, Point RecPoint2, Point RecPoint3, Point RecPoint4) {
    LineParametric(hdc, RecPoint1.x, RecPoint1.y, RecPoint2.x, RecPoint2.y);
    LineParametric(hdc, RecPoint2.x, RecPoint2.y, RecPoint3.x, RecPoint3.y);
    LineParametric(hdc, RecPoint3.x, RecPoint3.y, RecPoint4.x, RecPoint4.y);
    LineParametric(hdc, RecPoint4.x, RecPoint4.y, RecPoint1.x, RecPoint1.y);

    for (int i = RecPoint1.y, f = 0; i < RecPoint2.y && f < 2; i += 4) {
        Point
            p1(RecPoint1.x, RecPoint1.y),
            p2(RecPoint1.x, i),
            p3(RecPoint4.x, i),
            p4(RecPoint4.x, RecPoint4.y);


        Vector4 bezier(p1, p2, p3, p4);
        drawBezier(hdc, bezier, RGB(0, 255, 0), p1.x, p1.y);



    }


}


//------------------Filling Square with Hermit Curve-------------

void drawHermite(HDC hdc, const Vector4& hermite,COLORREF color, int limitX, int limitY) {
    int a3 = -2 * hermite.p2.x + 2 * hermite.p1.x + hermite.p3.x + hermite.p4.x;
    int a2 = 3 * hermite.p2.x - 3 * hermite.p1.x - 2 * hermite.p3.x - hermite.p4.x;
    int a1 = hermite.p3.x;
    int a0 = hermite.p1.x;

    int b3 = -2 * hermite.p2.y + 2 * hermite.p1.y + hermite.p3.y + hermite.p4.y;
    int b2 = 3 * hermite.p2.y - 3 * hermite.p1.y - 2 * hermite.p3.y - hermite.p4.y;
    int b1 = hermite.p3.y;
    int b0 = hermite.p1.y;

    double dt = 0.0001;
    for (double t = 0; t <= 1; t += dt)
    {
        int x = a0 + a1 * t + a2 * t * t + a3 * t * t * t;
        int y = b0 + b1 * t + b2 * t * t + b3 * t * t * t;
        if (x > limitX && y < limitY) SetPixel(hdc, x, y, color);
    }
}



void fillingSquareWithHermite(HDC hdc, Point RecPoint1, Point RecPoint2, Point RecPoint3, Point RecPoint4) {
    LineParametric(hdc, RecPoint1.x, RecPoint1.y, RecPoint2.x, RecPoint2.y);
    LineParametric(hdc, RecPoint2.x, RecPoint2.y, RecPoint3.x, RecPoint3.y);
    LineParametric(hdc, RecPoint3.x, RecPoint3.y, RecPoint4.x, RecPoint4.y);
    LineParametric(hdc, RecPoint4.x, RecPoint4.y, RecPoint1.x, RecPoint1.y);

    for (int i = RecPoint1.y; i < RecPoint2.y ; i += 3) {
        int u1X = RecPoint2.x - RecPoint1.x, u1Y = RecPoint2.y - RecPoint1.y;
        int u2X = RecPoint3.x - RecPoint4.x, u2Y = RecPoint3.y - RecPoint4.y;
        Point
            p1(RecPoint1.x, i),
            p2(RecPoint4.x, i),
            p3(0, u1Y),
            p4(0, u2Y);

        Vector4 bezier(p1, p2, p3, p4);
        drawHermite(hdc, bezier, RGB(0, 255, 0), p1.x, RecPoint3.y);
    }


}

// ----------------------Spline Curves -----------------------------------------//
void hermitMatrixCurve(HDC hdc, Vector& p1, Vector& T1, Vector& p2, Vector& T2)
{
    double a0 = p1[0], a1 = T1[0],
           a2 = -3 * p1[0] - 2 * T1[0] + 3 * p2[0] - T2[0],
           a3 = 2 * p1[0] + T1[0] - 2 * p2[0] + T2[0];

    double b0 = p1[1], b1 = T1[1],
           b2 = -3 * p1[1] - 2 * T1[1] + 3 * p2[1] - T2[1],
           b3 = 2 * p1[1] + T1[1] - 2 * p2[1] + T2[1];

    for (double t = 0; t <= 1; t += 0.001)
    {
        double t2 = t * t, t3 = t2 * t;
        double x = a0 + a1 * t + a2 * t2 + a3 * t3;
        double y = b0 + b1 * t + b2 * t2 + b3 * t3;
        SetPixel(hdc, Round(x), Round(y), color);
    }
}

void DrawCardinalSpline(HDC hdc, Vector P[], double d)
{
    Vector T[numberOfPoints]; //contain all sloopes
    T[0][0] = ((d / 2) * (P[1][0] - P[0][0]));
    T[0][1] = ((d / 2) * (P[1][1] - P[0][1]));

    for (int i = 1; i < numberOfPoints - 1; i++)
    {
        T[i][0] = ((d / 2) * (P[i + 1][0] - P[i - 1][0]));
        T[i][1] = ((d / 2) * (P[i + 1][1] - P[i - 1][1]));
    }
    T[numberOfPoints - 1][0] = ((d / 2) * (P[numberOfPoints - 1][0] - P[numberOfPoints - 2][0]));
    T[numberOfPoints - 1][1] = ((d / 2) * (P[numberOfPoints - 1][1] - P[numberOfPoints - 2][1]));

    for (int i = 0; i < numberOfPoints - 1; i++)
    {
        hermitMatrixCurve(hdc, P[i], T[i], P[i + 1], T[i + 1]);
    }
}


// ----------------------Spline Curves -----------------------------------------//


//----------------------------------Drawing Ellipse------------------------------//


void Draw4Points(HDC hdc,int xc,int yc, int a, int b,COLORREF color)
{
    SetPixel(hdc, xc+a, yc+b, color);
    SetPixel(hdc, xc+a, yc-b, color);
    SetPixel(hdc, xc-a, yc+b, color);
    SetPixel(hdc, xc-a, yc-b, color);
}



DrawEllipse_DirectCartesian(HDC hdc , int xc , int yc , int A , int B , COLORREF color)
{
    // first start at 0 , B --> and take unit step in the x axis
    // untill we reach the boundary between region 1 and region 2
    // then we take unit steps in y

    int x = 0 , y = B ;
    int dx = 2*(B*B)*x;
    int dy = 2*(A*A)*y;
    Draw4Points(hdc , xc , yc ,x , y, color);
    int A2 = (A*A);
    int B2 = (B*B);

    int A2_B2 = (A2*B2);

    while(dy > dx)
    {
        x ++;
        y = round((double) ( sqrt((double) (A2_B2) - (B2 * x*x) ) ) / A );

        Draw4Points(hdc , xc , yc , x , y , color);
        dx  = 2*(B2) * x;
        dy  = 2*(A2) * y;

    }
    while(y >= 0)
    {
        y -- ;
        x  =  round ( (double) ( sqrt((double) (A2_B2) - (A2 * y*y) ) ) / B) ;

        Draw4Points(hdc , xc , yc , x , y , color);
    }


}


DrawEllipse_Polar(HDC hdc , int xc , int yc , int A , int B , COLORREF color)
{
    int x = A , y = 0;
    int mx = std::max(A , B);
    double theta =0 , dtheta = (1.0/mx);
    Draw4Points(hdc , xc , yc , x , y , color);
    while(theta <= 2*M_PI)
    {
        theta+= dtheta;
        x = round(A*cos(theta));
        y = round(B*sin(theta));
        Draw4Points(hdc , xc , yc , x , y , color);
    }
}

DrawEllipse_MidPoint(HDC hdc , int xc , int yc , int A , int B , COLORREF color)
{
    // First Draw the first Region
    int x = 0 , y = B ;
    int d = (B*B) + (0.25 * (A*A)) - ((A*A)*B);
    int dx = 2*(B*B)*x;
    int dy = 2*(A*A)*y;

    Draw4Points(hdc , xc , yc , x , y, color);

    // Draw Region 1 ;
    while(dx < dy)
    {
        if(d <  0)
        {

            dx += 2*(B*B);
            d += dx + (B*B);
        }
        else{
            y --;
            dx += 2 *(B*B);
            dy -= 2*(A*A);
            d += dx - dy + (B*B);
        }
        x ++;
        Draw4Points(hdc , xc , yc , x ,y , color);

    }
    // Calculate d initial for region 2
    int d2 = ((B*B)*((x + 0.5)*(x + 0.5))) + ((A*A)*((y-1)*(y-1))) - ((A*A)*(B*B));
    Draw4Points(hdc , xc , yc ,  x, y , color);
    while(y >=0)
    {
        if(d2 > 0)
        {
            //y--;
            dy -= 2*(A*A);
            d2 -=dy + (A*A);
        }else
        {
            x ++;
            dx +=2*(B*B);
            dy -= 2*(A*A);
            d2 += dx - dy + (A*A);
        }
        y--;
        Draw4Points(hdc , xc , yc , x , y , color);
    }

}



///--------------------------Choose Color-----------------
COLORREF ChooseColor(HWND hwndParent, COLORREF crInitial) {
    static COLORREF customColors[16] = {0};
    CHOOSECOLOR cc = {0};
    cc.lStructSize = sizeof(cc);
    cc.hwndOwner = hwndParent;
    cc.rgbResult = crInitial;
    cc.lpCustColors = customColors;
    cc.Flags = CC_FULLOPEN | CC_RGBINIT;

    if (ChooseColor(&cc)) {
        return cc.rgbResult;
    } else {
        return crInitial;
    }
}
///------------------------------------------------------

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

    ///(O&P&K&L) Filling algorithms
    HMENU Filling = CreateMenu();
    ListSize = 6;
    char * fillings[ListSize] = {"Convex","General", "Flood", "Flood (Recursive)", "Circle with lines", "Circle with circles"};
    int fillings2[ListSize] = {ConvexFilling, GeneralFilling, FloodFilling , FloodFillingRecursive , CircleLines , CircleCircles };
    for(int i=0;i<ListSize;i++){
        AppendMenu(Filling,MF_STRING,fillings2[i],fillings[i]);
        if(i!=ListSize-1){
            AppendMenu(Filling,MF_SEPARATOR,NULL,NULL); //separate line
        }
    }
    AppendMenu(MyMenu, MF_POPUP, (UINT_PTR)Filling, "Filling");
    AppendMenu(Filling, MF_SEPARATOR, NULL, NULL);
    AppendMenu(Filling,MF_POPUP,(UINT_PTR)BezierRectangle,"Bezier");
    AppendMenu(Filling, MF_SEPARATOR, NULL, NULL);
    AppendMenu(Filling, MF_POPUP, (UINT_PTR)HermiteSquare, "Hermite");



    /// q --> Cardinal Spline Curves -----------------------
    HMENU SplineCurves = CreateMenu();
    AppendMenu(SplineCurves,MF_STRING, SplineCurve , "SplineCurve");
    AppendMenu(MyMenu,MF_POPUP,(UINT_PTR)SplineCurves,"Draw Spline Curve");


    ///r --> Drawing Ellipse --------------------------
    HMENU Ellipse = CreateMenu();
    ListSize=3;
    char * algos[ListSize] = {"EllipseDirect","EllipsePolar","EllipseMidPoint"};
    int algos2[ListSize] = {EllipseDirect,EllipsePolar,EllipseMidPoint};
    for(int i=0;i<ListSize;i++){
        AppendMenu(Ellipse,MF_STRING,algos2[i],algos[i]);
        if(i!=ListSize-1){
            AppendMenu(Ellipse,MF_SEPARATOR,NULL,NULL); //separate line
        }
    }
    AppendMenu(MyMenu,MF_POPUP,(UINT_PTR)Ellipse,"Ellipse");


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
    static int p1,p2,p3,p4, xc , yc ,idx=0;
    static double length,width;
    static Vector T;
    static Vector polygon[100];
    //inputs for clipping

    ///(a) Change the background of window to be white
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    SetClassLongPtr(hwnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);
    ///(a) Change the background of window to be white


    /// Variables for Spline
    static Vector pts[100]; ;



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
                case ConvexFilling:
                    click=0;
                    selectt = 8;
                    cout<<"Enter the Number of points:\n";
                    cin>>numberOfPoints;
                    break;
                case GeneralFilling:
                    click=0;
                    selectt = 9;
                    cout<<"Enter the Number of points:\n";
                    cin>>numberOfPoints;
                    break;
                case FloodFilling:
                    click=0;
                    selectt = 10;

                    break;
                case FloodFillingRecursive :
                    click=0;
                    selectt = 11;
                    break;
                case CircleLines:
                    click=0;
                    selectt = 12;
                    break;
                case CircleCircles:
                    click=0;
                    selectt = 13;
                    break;
                case ColorButton:
                    color = ChooseColor(hwnd, RGB(0,0,0));
                    break;
                case HermiteSquare:
                    click = 0;
                    selectt = 17;
                    break;
                case BezierRectangle:
                    selectt = 18;
                    click = 0;
                    break;
                case SplineCurve:
                    click=0;
                    selectt = 19;
                    cout<<"Enter the Number of points:\n";
                    cin>>numberOfPoints;
                    break;
                case EllipseDirect:
                    selectt = 20;
                    break;
                case EllipsePolar:
                    selectt = 21;
                    break;
                case EllipseMidPoint:
                    selectt = 22;
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
//        case WM_LBUTTONDBLCLK:
//            xc = LOWORD(lParam);
//            yc = HIWORD(lParam);
//            break;

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

        ///For Convex Filling --------
        else if (selectt==8){
            if(click==numberOfPoints){
                ConvexFill(hdc, points, numberOfPoints, color);
                click=0;
                points.clear();
                break;
            }
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            points.push_back(pt);
            click++;
        }
        ///---------------------------

        ///For General Filling
        else if (selectt==9){
            if(click==numberOfPoints){
                GeneralFill(hdc, points, numberOfPoints, color);
                click=0;
                points.clear();
                break;
            }
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            points.push_back(pt);
            click++;
        }

        //Filling Square with Hermit Curve
        else if (selectt == 17) {
            Point p1(50, 50);
            Point p2(50, 350);
            Point p3(400, 350);
            Point p4(400, 50);
            cout << " Hermite";
            fillingSquareWithHermite(hdc, p1, p2, p3, p4);

        }
        //Filling Rectangle with Bezier Curve
        else if (selectt == 18) {
            Point p1(100, 150);
            Point p2(100, 400);
            Point p3(300, 400);
            Point p4(300, 150);
            fillingRectangleWithBezier(hdc, p1, p2, p3, p4);
        }

        /// ------- Draw Cardinal Spline curve --------
        else if(selectt == 19)
        {

            pts[idx]=Vector(LOWORD(lParam), HIWORD(lParam));
            if(idx!=numberOfPoints-1){
                idx++;
            }else{
                DrawCardinalSpline(hdc,pts,1);
                idx=0;
            }
        }

        /// ---- Draw Ellipse --- ///

        else if(selectt == 20)
        {

            DrawEllipse_DirectCartesian(hdc , X1 , Y1 , 100 , 200 , RGB(0 ,0 ,255) );
        }

        else if (selectt == 21)
        {
            DrawEllipse_Polar(hdc , X1 , Y1 ,  200 , 100 , RGB(255 , 0 , 0));
        }
        else if (selectt == 22)
        {
            DrawEllipse_MidPoint(hdc , X1 , Y1 , 150 , 250 , RGB(210 , 20 , 200));
        }

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


