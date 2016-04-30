#include <tchar.h>
#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

TCHAR szClassName[ ] = _T("Windows Programming Lab 2");

#define ID_BUTTON_MESSAGEBOX 1
#define ID_MENUITEM_SETTINGS 2
#define ID_MENUITEM_BAR 3
#define ID_MENUITEM_ABOUT 4

bool IsKeyDown = false;
COLORREF BgColor = RGB(0xFF, 0xFF, 0xFF);

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow) {
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

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

    if (!RegisterClassEx (&wincl)) return 0;

    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Windows Programming Lab 2"),       /* Title Text */
           WS_OVERLAPPEDWINDOW | WS_VSCROLL, /* default window */
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

void ChangeBkGnd(HWND hwnd) {
  BgColor = IsKeyDown ? RGB(0xFF, 0, 0) : RGB(0xFF, 0xFF, 0xFF);
  InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    RECT screenRect;
    PAINTSTRUCT ps;
    GetClientRect(hwnd, &screenRect);
    HDC hdc;
    HBRUSH brush;

    switch (message) {
        case WM_CREATE: {
            CreateWindowW(L"Button", L"Alert!", WS_VISIBLE | WS_CHILD,
                          screenRect.right * 1./3, screenRect.bottom * 4./5, 120, 25,
                          hwnd, (HMENU) ID_BUTTON_MESSAGEBOX, NULL, NULL);
            HMENU mainMenu = GetSystemMenu(hwnd, false);
            InsertMenu(mainMenu, 5, MF_BYCOMMAND | MF_SEPARATOR, ID_MENUITEM_BAR, "");
            InsertMenu(mainMenu, 6, MF_BYPOSITION, ID_MENUITEM_SETTINGS, "Settings...");
            InsertMenu(mainMenu, 7, MF_BYPOSITION, ID_MENUITEM_ABOUT, "About...");

        }
            break;
        case WM_COMMAND: {
            switch (wParam) {
                case ID_BUTTON_MESSAGEBOX: {
                    MessageBox(hwnd, "You just got alerted! Gratz :)", "ALERT!", MB_OK);
                }
                    break;
                default:
                    return DefWindowProc(hwnd, message, wParam, lParam);
            }
        }
            break;
        case WM_SYSCOMMAND: {
            switch (wParam) {
                case ID_MENUITEM_ABOUT: {
                    MessageBox(hwnd, "This is just a laboratory work program. UTM licensed!", "About", MB_OK);
                }
                    break;
                case ID_MENUITEM_SETTINGS: {
                    MessageBox(hwnd, "'Settings', really?", "Settings", MB_OKCANCEL);
                }
                    break;
                default: return DefWindowProc(hwnd, message, wParam, lParam);
            }
        }
            break;
        case WM_KEYDOWN: {
            bool isCPressed = GetKeyState(0x43) & (1 << 15);
            bool isVPressed = GetKeyState(0x56) & (1 << 15);
            bool isBPressed = GetKeyState(0x42) & (1 << 15);

            if (isCPressed && isVPressed) {
                IsKeyDown = true;
                ChangeBkGnd(hwnd);
            } else if (isVPressed && isBPressed) {
                MessageBox(hwnd, "No time to do something else :(", "Sad face", MB_OK);
            }
        }
            break;
        case WM_KEYUP: {
            bool isCPressed = GetKeyState(0x43) & (1 << 15);
            bool isVPressed = GetKeyState(0x56) & (1 << 15);

            if (!(isCPressed && isVPressed) && IsKeyDown) {
                IsKeyDown = false;
                ChangeBkGnd(hwnd);
            }
        }
            break;
        case WM_VSCROLL: {
            int verticalScrollColor = HIWORD(wParam) / 100. * 255;
            BgColor = RGB(0xFF, verticalScrollColor, 0xFF);
            InvalidateRect(hwnd, NULL, true);
        }
            break;
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            brush = CreateSolidBrush(BgColor);
            FillRect(hdc, &screenRect, brush);
            EndPaint(hwnd, &ps);
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
