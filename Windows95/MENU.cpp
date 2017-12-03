/* Process accelerator keys. */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include "menu.h"

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

int WINAPI WinMain( HINSTANCE hThisInst, HINSTANCE hPreviInst, LPSTR lpszArgs, int nWinMode )
{
    HWND hwnd;
    MSG msg;
    WNDCLASS wcl;
    HACCEL hAccel;

    /* Define a window class. */
    wcl.hInstance = hThisInst; /* handle to this instance */
    wcl.lpszClassName = szWinName; /* window class name */
    wcl.lpfnWndProc = WindowFunc; /* window function */
    wcl.style = 0; /* default style */

    wcl.hIcon = LoadIcon( NULL, IDI_APPLICATION ); /* icon style */
    wcl.hCursor = LoadCursor( NULL, IDC_ARROW ); /* cursor style */
    
    /* specify name of menu resource */
    wcl.lpszMenuName = "MYMENU"; /* main menu */

    wcl.cbClsExtra = 0; /* no extra */
    wcl.cbWndExtra = 0; /* information needed */

    /* Make the window white. */
    wcl.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );

    /* Register the window class. */
    if( !RegisterClass( &wcl ) ) return 0;

    /* Now that a window class has been registered, a window can be created. */
    hwnd = CreateWindow(
        szWinName, /* name of window class */
        "Using Menus", /* title */
        WS_OVERLAPPEDWINDOW, /* window style - normal */
        CW_USEDEFAULT, /* X coordinate - let Windows decide */
        CW_USEDEFAULT, /* Y coordinate - let Windows decide */
        CW_USEDEFAULT, /* width - let Windows decide */
        CW_USEDEFAULT, /* height - let Windows decide */
        HWND_DESKTOP, /* no parent window */
        NULL, /* no menu */
        hThisInst, /* handle of this instance of the program */
        NULL /* no additional arguments */
        );

    /* load the keyboard accelerators */
    hAccel = LoadAccelerators( hThisInst, "MYMENU" );

    /* Display the window. */
    ShowWindow( hwnd, nWinMode );
    UpdateWindow( hwnd );

    /* Create the message loop. */
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        if( !TranslateAccelerator( hwnd, hAccel, &msg ) )
        {
            TranslateMessage( &msg ); /* allow use of keyboard */
            DispatchMessage( &msg ); /* return control to Windows */
        }
    }

    return msg.wParam;
}

/* This function is called by Windows 95 and is passed messages from the message queue. */

LRESULT CALLBACK WindowFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    int response;

    switch( message )
    {
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case IDM_ALPHA:
                    MessageBox( hwnd, "Alpha", "", MB_OK );
                    break;
                case IDM_BETA:
                    MessageBox( hwnd, "Beta", "", MB_OK );
                    break;
                case IDM_GAMMA:
                    MessageBox( hwnd, "Gamma", "", MB_OK );
                    break;
                case IDM_EPSILON:
                    MessageBox( hwnd, "Epsilon", "", MB_OK );
                    break;
                case IDM_ZETA:
                    MessageBox( hwnd, "Zeta", "", MB_OK );
                    break;
                case IDM_ETA:
                    MessageBox( hwnd, "Eta", "", MB_OK );
                    break;
                case IDM_THETA:
                    MessageBox( hwnd, "Theta", "", MB_OK );
                    break;
                case IDM_HELP:
                    MessageBox( hwnd, "No Help", "Help", MB_OK );
                    break;
            }
            break;
        case WM_RBUTTONDOWN: /* process right button */
            response = MessageBox( hwnd, "Press One:", "Right Button", MB_ABORTRETRYIGNORE );
            switch( response )
            {
                case IDABORT:
                    MessageBox( hwnd, "", "Abort", MB_OK );
                    break;
                case IDRETRY:
                    MessageBox( hwnd, "", "Retry", MB_OK );
                    break;
                case IDIGNORE:
                    MessageBox( hwnd, "", "Ignore", MB_OK );
                    break;
            }
            break;
        case WM_LBUTTONDOWN: /* process left button */
            response = MessageBox( hwnd, "Continue?", "Left Button", MB_ICONHAND | MB_YESNO );
            switch( response )
            {
                case IDYES:
                    MessageBox( hwnd, "Press Button", "Yes", MB_OK );
                    break;
                case IDNO:
                    MessageBox( hwnd, "Press Button", "No", MB_OK );
                    break;
            }
            break;
        case WM_DESTROY: /* terminate the program */
            PostQuitMessage( 0 );
            break;
        default:
            /* Let Windows 95 process any messages not specified in the preceding switch statement. */
            return DefWindowProc( hwnd, message, wParam, lParam );
    }
    return 0;
}
