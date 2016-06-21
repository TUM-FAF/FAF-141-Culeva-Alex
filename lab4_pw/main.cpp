#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <gdiplus.h>
#include <time.h>

using namespace Gdiplus;

#define ID_TIMER 1

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("LaboratoryWork4");

typedef struct _FigureInfo {
    int width, height, x = 1, y = 1, dx, dy;
} FigureInfo;
FigureInfo figureInfo;

void UpdateFigure(RECT *rect) {
    figureInfo.x += figureInfo.dx;
    figureInfo.y += figureInfo.dy;

    if(figureInfo.x < 0) {
        figureInfo.x = 0;
        figureInfo.dx = 1;
    } else if(figureInfo.x + figureInfo.width > rect->right) {
        figureInfo.x = rect->right - figureInfo.width;
        figureInfo.dx = -1;
    }

    if(figureInfo.y < 0) {
        figureInfo.y = 0;
        figureInfo.dy = 1;
    } else if(figureInfo.y + figureInfo.height > rect->bottom) {
        figureInfo.y = rect->bottom - figureInfo.height;
        figureInfo.dy = -1;
    }
    return;
}

void DrawFigure(HDC hdc, RECT rect) {
    Graphics graphics(hdc);
    Pen pen(Color(rand() % 256, rand() % 256, rand() % 256));
    int centerX = rand() % rect.right;
    int centerY = rand() % rect.bottom;
    graphics.DrawEllipse(&pen, centerX, centerY,
                        rect.right - centerX < centerX ? rand() % (rect.right - centerX) : rand() % centerX,
                        rect.bottom - centerY < centerY ? rand() % (rect.bottom - centerY) : rand() % centerY
                        );
    return;
}

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


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HDC hdc;
    switch (message) {
        case WM_CREATE:
            hdc = GetDC(hwnd);
            SetTimer(hwnd, ID_TIMER, 50, NULL);
            break;
        case WM_TIMER: {
            RECT rcClient;
            HDC hdc = GetDC(hwnd);

            GetClientRect(hwnd, &rcClient);

            UpdateFigure(&rcClient);
            DrawFigure(hdc, rcClient);

            ReleaseDC(hwnd, hdc);

            break;
        }
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}


