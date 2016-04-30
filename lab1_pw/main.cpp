#include <tchar.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>
#include <windowsx.h>

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

#define ID_BUTTON_PRESSME 1
#define ID_BUTTON_DONTPRESSME 2
#define ID_TEXTEDIT_EDITME 3
#define ID_TEXTEDIT_DONTEDITME 4

TCHAR szClassName[ ] = _T("WindowsApp");
const char *MiddleScreenText = "Done with Pride and Prejudice by Culeva Alex";
HFONT hFont;
COLORREF RED = RGB(255,0,0);
HWND textBoxEditMe;

typedef struct RGB {
    int red;
    int green;
    int blue;
} RGB;

int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, LPSTR lpszArgument, int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    hFont = CreateFont (19, 0, 0, 0, FW_DONTCARE, TRUE, FALSE, FALSE, ANSI_CHARSET, \
		  OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, \
		  DEFAULT_PITCH | FF_SWISS, (LPCSTR)L"Helvetica");

    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl)) return 0;

    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Windows Programming Lab 1"),       /* Title Text */
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

    ShowWindow (hwnd, nCmdShow);

    while (GetMessage (&messages, NULL, 0, 0)) {
        TranslateMessage(&messages);
        DispatchMessage(&messages);
    }

    return messages.wParam;
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    PAINTSTRUCT ps;
    HDC hdc;
    RECT screenRect;
    GetClientRect(hwnd, &screenRect);

    switch (message) {
        case WM_CREATE: {
            HWND buttonPressMe = CreateWindowW(L"Button", L"Press Me!", WS_VISIBLE | WS_CHILD,
                          screenRect.right * 1./3, screenRect.bottom * 4./5, 120, 25,
                          hwnd, (HMENU) ID_BUTTON_PRESSME, NULL, NULL);
            SendMessage(buttonPressMe, WM_SETFONT, WPARAM (hFont), TRUE);
            CreateWindowW(L"Button", L"Don't Press Me!", WS_VISIBLE | WS_CHILD,
                          screenRect.right * 2./3, screenRect.bottom * 4./5, 120, 25,
                          hwnd, (HMENU) ID_BUTTON_DONTPRESSME, NULL, NULL);

            textBoxEditMe = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Edit Me!",
                           WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                           screenRect.right * 1./3, screenRect.bottom * 1./5, 60, 25, hwnd,
                           (HMENU)ID_TEXTEDIT_EDITME, GetModuleHandle(NULL), NULL);
            CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "Don't Edit Me!",
                           WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
                           screenRect.right * 2./3, screenRect.bottom * 1./5, 100, 25, hwnd,
                           (HMENU)ID_TEXTEDIT_DONTEDITME, GetModuleHandle(NULL), NULL);
        }
            break;
        case WM_PAINT: {
            hdc = BeginPaint(hwnd, &ps);
            DrawText(hdc, MiddleScreenText, -1, &screenRect, DT_VCENTER | DT_SINGLELINE | DT_CENTER);
            EndPaint(hwnd, &ps);
        }
            break;
        case WM_CTLCOLOREDIT: {
            HWND leftParam = (HWND)lParam;
            if (leftParam == GetDlgItem(hwnd, ID_TEXTEDIT_EDITME) || leftParam == GetDlgItem(hwnd, ID_TEXTEDIT_DONTEDITME)) {
                HDC editHDC = (HDC)wParam;
                int r = rand() % 256;
                int g = rand() % 256;
                int b = rand() % 256;
                SetTextColor(editHDC, RGB(110, 105, 200));
                SetBkColor(editHDC, RGB(r, g, b));

                return (LRESULT)CreateSolidBrush(RGB(r, g, b));
            }
        }
            break;
        case WM_COMMAND: {
            switch (wParam) {
                case ID_BUTTON_PRESSME: {
                    HDC hdc = BeginPaint(textBoxEditMe, &ps);
                    SendMessage(hwnd, WM_CTLCOLOREDIT, (WPARAM)hdc, (LPARAM)ID_TEXTEDIT_DONTEDITME);
                    EndPaint(textBoxEditMe, &ps);
                }
                    break;
                default:
                    return DefWindowProc(hwnd, message, wParam, lParam);
            }
        }
            break;
        case WM_SIZE: {
            InvalidateRect(hwnd, &screenRect, true);
        }
            break;
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        case WM_CLOSE:
            MessageBox(hwnd, "You will never close this!", "HAHAHA!", MB_OK);
            return 0;
        case WM_SYSCOMMAND:
            switch (wParam) {
            case SC_MAXIMIZE:
                return DefWindowProc(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, lParam);
            case SC_MINIMIZE:
                return DefWindowProc(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, lParam);
            default:
                return DefWindowProc (hwnd, message, wParam, lParam);
            }
        default:
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
