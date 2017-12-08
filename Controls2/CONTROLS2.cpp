/* Demonstrate check boxes. */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include "controls2.h"

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );
LRESULT CALLBACK DialogFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

HINSTANCE hInst;

int status1 = 0, status2 = 0; /* holds status of check boxes */

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
        "Using Check Boxes", /* title */
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
    char str[ 80 ];

    switch( message )
    {
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case ID_DIALOG1:
                    DialogBox( hInst, "MYDB", hwnd, (DLGPROC)DialogFunc );
                    break;
                case ID_STATUS: /* show check box status */
                    if( status1 ) strcpy( str, "Checkbox 1 is checked\n" );
                    else strcpy( str, "Checkbox 1 is not checked\n" );
                    if( status2 ) strcat( str, "Checkbox 2 is checked\n" );
                    else strcat( str, "Checkbox 2 is not checked\n" );
                    MessageBox( hwnd, str, "", MB_OK );
                    break;
                case ID_HELP:
                    MessageBox( hwnd, "Help", "", MB_OK );
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
LRESULT CALLBACK DialogFunc( HWND hdwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    switch( message )
    {
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case IDCANCEL:
                    EndDialog( hdwnd, 0 );
                    return 1;
                case IDOK:
                    /* update global checkbox status variables */
                    status1 = SendDlgItemMessage( hdwnd, ID_CB1, BM_GETCHECK, 0, 0 ); // is box checked?
                    status2 = SendDlgItemMessage( hdwnd, ID_CB2, BM_GETCHECK, 0, 0 ); // is box checked?
                    EndDialog( hdwnd, 0 );
                    return 1;
                case ID_CB1:
                    /* user selected 1st check box, so check it */
                    SendDlgItemMessage( hdwnd, ID_CB1, BM_SETCHECK, 1, 0 );
                    return 1;
            }
    }
    return 0;
}
