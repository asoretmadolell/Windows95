/* Demonstrate A Scroll Bar */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include "stdctls3.h"

#define RANGEMAX 50

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );
BOOL CALLBACK DialogFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

HINSTANCE hInst;

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
        "Using a Scroll Bar", /* title */
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

    hInst = hThisInst; /* save the current instance handle */

    /* load accelerators */
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
    switch( message )
    {
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case IDM_DIALOG1:
                    DialogBox( hInst, "MYDB", hwnd, DialogFunc );
                    break;
                case IDM_HELP:
                    MessageBox( hwnd, "Help", "Use the Scroll Bar", MB_OK );
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

/* A simple dialog function. */
BOOL CALLBACK DialogFunc( HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    char str[ 80 ];
    static int pos = 0; /* slider box position */

    HDC hdc;

    switch( message )
    {
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case IDCANCEL:
                    EndDialog( hdwnd, 0 );
                    return 1;
            }
            break;
        case WM_VSCROLL:
            SetScrollRange( (HWND)lParam, SB_CTL, 0, RANGEMAX, 1 );
            switch( LOWORD( wParam ) )
            {
                case SB_LINEDOWN:
                    pos++;
                    if( pos > RANGEMAX ) pos = RANGEMAX;
                    SetScrollPos( (HWND)lParam, SB_CTL, pos, 1 );
                    hdc = GetDC( hdwnd );
                    sprintf( str, "%d", pos );
                    TextOut( hdc, 55, 30, "    ", 4 );
                    TextOut( hdc, 55, 30, str, strlen( str ) );
                    ReleaseDC( hdwnd, hdc );
                    return 1;
                case SB_LINEUP:
                    pos--;
                    if( pos < 0 ) pos = 0;
                    SetScrollPos( (HWND)lParam, SB_CTL, pos, 1 );
                    hdc = GetDC( hdwnd );
                    sprintf( str, "%d", pos );
                    TextOut( hdc, 55, 30, "    ", 4 );
                    TextOut( hdc, 55, 30, str, strlen( str ) );
                    ReleaseDC( hdwnd, hdc );
                    return 1;
                case SB_THUMBPOSITION:
                    pos = HIWORD(wParam); /* get current position */
                    SetScrollPos( (HWND)lParam, SB_CTL, pos, 1 );
                    hdc = GetDC( hdwnd );
                    sprintf( str, "%d", pos );
                    TextOut( hdc, 55, 30, "    ", 4 );
                    TextOut( hdc, 55, 30, str, strlen( str ) );
                    ReleaseDC( hdwnd, hdc );
                    return 1;
                case SB_THUMBTRACK:
                    pos = HIWORD( wParam ); /* get current position */
                    SetScrollPos( (HWND)lParam, SB_CTL, pos, 1 );
                    hdc = GetDC( hdwnd );
                    sprintf( str, "%d", pos );
                    TextOut( hdc, 55, 30, "    ", 4 );
                    TextOut( hdc, 55, 30, str, strlen( str ) );
                    ReleaseDC( hdwnd, hdc );
                    return 1;
                case SB_PAGEDOWN:
                    pos += 5;
                    if( pos > RANGEMAX ) pos = RANGEMAX;
                    SetScrollPos( (HWND)lParam, SB_CTL, pos, 1 );
                    hdc = GetDC( hdwnd );
                    sprintf( str, "%d", pos );
                    TextOut( hdc, 55, 30, "    ", 4 );
                    TextOut( hdc, 55, 30, str, strlen( str ) );
                    ReleaseDC( hdwnd, hdc );
                    return 1;
                case SB_PAGEUP:
                    pos -= 5;
                    if( pos < 0 ) pos = 0;
                    SetScrollPos( (HWND)lParam, SB_CTL, pos, 1 );
                    hdc = GetDC( hdwnd );
                    sprintf( str, "%d", pos );
                    TextOut( hdc, 55, 30, "    ", 4 );
                    TextOut( hdc, 55, 30, str, strlen( str ) );
                    ReleaseDC( hdwnd, hdc );
                    return 1;
            }
    }
    return 0;
}
