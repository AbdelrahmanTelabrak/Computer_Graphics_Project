///Abdelrahman Mohamed Ramadan 20200293
///Abdelrahman Tarek 20200288
///Mohamed Samir 20200448

#include <Windows.h>
#include<bits/stdc++.h>
#include<math.h>
#define sarsora
using namespace std;
void swap(int &x1,int &y1,int &x2,int &y2){
    int temp=x1;
    x1=x2;
    x2=temp;
    int temp2=y1;
    y1=y2;
    y2=temp2;
}

int Round(double x){
    return (int)(x+0.5);
}


void LineDDA(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
    int dx = x2 - x1, dy = y2 - y1;
    if (abs(dx) >= abs(dy)) {
        if (x1 > x2) {
            swap(x1, y1, x2, y2);
            dx = -dx; dy = -dy;
        }
        if (dy >= 0) {
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);
            int d_initial = dx - 2 * dy;
            int change_1 = -2 * dy;
            int change_2 = 2 * (dx - dy);
            SetPixel(hdc, x, y, c);
            while (x < x2) {

                if (d_initial > 0) {
                    x++;
                    d_initial += change_1;
                }
                else {
                    x++, y++;
                    d_initial += change_2;
                }
                SetPixel(hdc, x, y, c);
            }

        }
        else {
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);
            int d_initial = dx - 2 * abs(dy);
            int change_1 = -2 * abs(dy);
            int change_2 = 2 * (dx - abs(dy));
            SetPixel(hdc, x, y, c);
            while (x < x2) {
                if (d_initial > 0) {
                    x++;
                    d_initial += change_1;
                }
                else {
                    x++, y--;
                    d_initial += change_2;
                }
                SetPixel(hdc, x, y, c);
            }
        }



    }
    else {
        if (y1 > y2)
        {
            swap(x1, y1, x2, y2);
            dx = -dx; dy = -dy;
        }
        if (dx >= 0) {
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);
            int d_initial = dy - 2 * dx;
            int change_1 = -2 * dx;
            int change_2 = 2 * (dy - dx);
            SetPixel(hdc, x, y, c);
            while (y < y2) {

                if (d_initial >= 0) {
                    y++;
                    d_initial += change_1;
                }
                else {
                    x++, y++;
                    d_initial += change_2;
                }
                SetPixel(hdc, x, y, c);
            }
        }
        else {
            int x = x1, y = y1;
            SetPixel(hdc, x, y, c);
            int d_initial = dy - 2 * abs(dx);
            int change_1 = -2 * abs(dx);
            int change_2 = 2 * (dy - abs(dx));
            SetPixel(hdc, x, y, c);
            while (y < y2) {

                if (d_initial > 0) {
                    y++;
                    d_initial += change_1;
                }
                else {
                    x--, y++;
                    d_initial += change_2;
                }
                SetPixel(hdc, x, y, c);
            }
        }
    }
}
class OutCode {
public:
    unsigned char All:4;
    bool left, right, up, bottom;

    OutCode() {
        All = 0;
        left = right = up = bottom = false;
    }
};

OutCode GetOutCode(double x, double y, int xleft, int ytop, int xright, int ybottom) {
    OutCode code;

    if (x < xleft) {
        code.left = true;
        code.All |= 1;
    } else if (x > xright) {
        code.right = true;
        code.All |= 2;
    }

    if (y > ybottom) {
        code.bottom = true;
        code.All |= 4;
    } else if (y < ytop) {
        code.up = true;
        code.All |= 8;
    }

    return code;
}

void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}
void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left){ VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);}
            else if(out1.up){HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);}
            else if(out1.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        } else
        {
            if(out2.left)VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.up)HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }

    if(!out1.All && !out2.All)
    {
        LineDDA(hdc, x1, y1, x2, y2,
                RGB(255, 0, 0));
    }


}

int c=0,cc=0;

int x1,yy1,x2,y2,x3,y3,x4,y4,xl1,yl1,xl2,yl2;

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{

    HDC hdc;


    switch(mcode){

        case WM_RBUTTONDOWN:
            if(c==0)
            { x1=LOWORD(lp);
                yy1=HIWORD(lp);
                c++;
            }
            else if(c==1)
            { x2=LOWORD(lp);
                y2=HIWORD(lp);
                x2=x1;
                c++;
            }
            else if(c==2) {
                x3 = LOWORD(lp);
                y3 = HIWORD(lp);
                c++;
                y3=y2;
            }
            else if(c==3) {
                x4 = LOWORD(lp);
                y4 = HIWORD(lp);
                x4 = x3;
                y4 = yy1;
                hdc = GetDC(hwnd);
                LineDDA(hdc, x1, yy1, x2, y2, RGB(255, 0, 0));
                LineDDA(hdc, x2, y2, x3, y3, RGB(255, 0, 0));
                LineDDA(hdc, x3, y3, x4, y4, RGB(255, 0, 0));
                LineDDA(hdc, x4, y4, x1, yy1, RGB(255, 0, 0));
                ///SHADDING THE RECTANGULAR
                for(double i=x1;i<=x4+0.1;i+=0.1){
                    LineDDA(hdc,i,yy1,i,y3,RGB(0,255, 0));
                }
                c=0;
                ReleaseDC(hwnd,hdc);

            }
            break;


        case WM_LBUTTONDOWN:
            if(cc==0) {
                xl1 = LOWORD(lp);
                yl1 = HIWORD(lp);
                cc++;
            }
            else if(cc==1){
                xl2=LOWORD(lp);
                yl2 = HIWORD(lp);
                cc=0;
                hdc = GetDC(hwnd);
                cout<<xl1<<" "<<yl1<<endl;
                CohenSuth(hdc,xl1, yl1, xl2, yl2,x1,y2,x4,yy1);
                ReleaseDC(hwnd,hdc);
                }



            break;

        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default: return DefWindowProc(hwnd, mcode, wp, lp);
    }
    return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
    WNDCLASS wc;
    wc.cbClsExtra = wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hInstance = hinst;
    wc.lpfnWndProc = MyWndProc;
    wc.lpszClassName ="MyClass";
    wc.lpszMenuName = NULL;
    wc.style = CS_HREDRAW | CS_VREDRAW;
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("MyClass", "My First Window", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, NULL, NULL, hinst, 0);
    ShowWindow(hwnd, nsh);
    UpdateWindow(hwnd);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
