#include <tchar.h>
#include <stdio.h>
#include <windows.h>
#include <gdiplus.h>
#include <time.h>
using namespace Gdiplus;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("Windows Programming Lab 2");

typedef void (*GraphicsFunction)(HDC, RECT) ;
GraphicsFunction CustomGraphics;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */
    GdiplusStartupInput gdiPlusStartupInput;
    ULONG_PTR gdiPlusToken;

    GdiplusStartup(&gdiPlusToken, &gdiPlusStartupInput, NULL);

    time_t t;
    srand(time(&t));

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
           _T("Windows Programming Lab 2"),       /* Title Text */
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


/*  This function is called by the Windows function DispatchMessage()  */

void DrawRandomLine(HDC hdc, RECT rect) {
    Graphics graphics(hdc);
    Pen pen(Color(rand() % 256, rand() % 256, rand() % 256));
    graphics.DrawLine(&pen, (int)(rand() % rect.right), rand() % rect.bottom,
                    rand() % rect.right, rand() % rect.bottom);
}

void DrawRandomCurve(HDC hdc, RECT rect) {
    Graphics graphics(hdc);
    Pen pen(Color(rand() % 256, rand() % 256, rand() % 256));
    graphics.DrawBezier(&pen, (int)(rand() % rect.right), rand() % rect.bottom,
                    rand() % rect.right, rand() % rect.bottom, rand() % rect.right, rand() % rect.bottom,
                    rand() % rect.right, rand() % rect.bottom);
}

void DrawRandomObject(HDC hdc, RECT rect) {
    Graphics graphics(hdc);
    switch(rand() % 2) {
    case 0: {
        Pen pen(Color(rand() % 256, rand() % 256, rand() % 256));
        int centerX = rand() % rect.right;
        int centerY = rand() % rect.bottom;
        graphics.DrawEllipse(&pen, centerX, centerY,
                             rect.right - centerX < centerX ? rand() % (rect.right - centerX) : rand() % centerX,
                             rect.bottom - centerY < centerY ? rand() % (rect.bottom - centerY) : rand() % centerY
                             );
    }
        break;
    case 1: {
        SolidBrush *brush = new SolidBrush(Color(rand() % 256, rand() % 256, rand() % 256));
        int x = rand() % rect.right;
        int y = rand() % rect.bottom;
        graphics.FillRectangle(brush, x, y,
                                rect.right - x < x ? rand() % (rect.right - x) : rand() % x,
                                rect.bottom - y < y ? rand() % (rect.bottom - y) : rand() % y
                               );
    }
    }
}

void DrawImage(HDC hdc, RECT rect) {
    HBITMAP image = (HBITMAP)LoadImage(0, "c.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    BITMAP bm;
    GetObject(image, sizeof(BITMAP), &bm);

    HDC hdcImage = CreateCompatibleDC(hdc);
    SelectObject(hdcImage, image);
    BitBlt(hdc, 0, 0, 50, 50, hdcImage, 0, 0, SRCCOPY);

    DeleteDC(hdcImage);
    DeleteObject((HBITMAP)image);
}

void DrawGradientRectangle(HDC hdc, RECT rect) {
    int x = rand() % rect.right;
    int y = rand() % rect.bottom;
    Graphics graphics(hdc);
    Rect smallRect = Rect(x, y,
                               rect.right - x < x ? rand() % (rect.right - x) : rand() % x,
                               rect.bottom - y < y ? rand() % (rect.bottom - y) : rand() % y);

    LinearGradientBrush *brush = new LinearGradientBrush(smallRect,
                                                         Color(0xFF, rand()%256, rand()%256, rand()%256),
                                                         Color(0xFF, rand()%256, rand()%256, rand()%256), 0., true
                                                         );
    graphics.FillRectangle(brush, smallRect);
}

void DrawNeededObjects(HDC hdc, RECT rect) {
    for(int i = 0; i < 5; i += 1) { DrawRandomLine(hdc, rect); }
    for(int i = 0; i < 2; i += 1) { DrawRandomCurve(hdc, rect); }
    for(int i = 0; i < 2; i += 1) { DrawRandomObject(hdc, rect); }
    DrawImage(hdc, rect);
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    PAINTSTRUCT ps;
    RECT screenRect;
    GetClientRect(hwnd, &screenRect);

    switch (message) {
        case WM_CREATE: {
            CustomGraphics = DrawNeededObjects;
        }
            break;
        case WM_LBUTTONDOWN: {
            bool isControlPressed = GetKeyState(VK_CONTROL) & (1 << 15);
            bool isSpacePressed = GetKeyState(VK_SPACE) & (1 << 15);
            CustomGraphics = isControlPressed && isSpacePressed ? DrawGradientRectangle : DrawRandomObject;
            InvalidateRect(hwnd, NULL, false);
        }
            break;
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            if (CustomGraphics) {
                CustomGraphics(hdc, screenRect);
                CustomGraphics = NULL;
            }
            EndPaint(hwnd, &ps);
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
