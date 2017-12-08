/* Demonstrate A Scroll Bar */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include "text.h"

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

char str[ 255 ]; /* holds output strings */

int X = 0, Y = 0; /* current output location */
int maxX, maxY; /* screen dimensions */

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
        "Fun With Text", /* title */
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
    HDC hdc;
    TEXTMETRIC tm;
    SIZE size;

    switch( message )
    {
        case WM_CREATE:
            /* get creen coordinates */
            maxX = GetSystemMetrics( SM_CXSCREEN );
            maxY = GetSystemMetrics( SM_CYSCREEN );
            break;
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case ID_SHOW:
                    hdc = GetDC( hwnd ); /* get device context */

                    /* set text color to black */
                    SetTextColor( hdc, RGB( 0, 0, 0 ) );
                    /* set background color to turquoise */
                    SetBkColor( hdc, RGB( 0, 255, 255 ) );

                    /* get text metrics */
                    GetTextMetrics( hdc, &tm );

                    sprintf( str, "The font is %ld pixels high.", tm.tmHeight );
                    TextOut( hdc, X, Y, str, strlen( str ) ); /* output string */
                    Y = Y + tm.tmHeight + tm.tmExternalLeading; /* next line */

                    strcpy( str, "This is on the next line. " );
                    TextOut( hdc, X, Y, str, strlen( str ) ); /* output string */

                    /* compute length of a string */
                    GetTextExtentPoint32( hdc, str, strlen( str ), &size );
                    sprintf( str, "Previous string is %ld units long", size.cx );
                    X = size.cx; /* advance to end of previous string */
                    TextOut( hdc, X, Y, str, strlen( str ) );
                    Y = Y + tm.tmHeight + tm.tmExternalLeading; /* next line */
                    X = 0; /* reset X */

                    sprintf( str, "Screen dimensions: %d %d", maxX, maxY );
                    TextOut( hdc, X, Y, str, strlen( str ) );
                    Y = Y + tm.tmHeight + tm.tmExternalLeading; /* next line */
                    ReleaseDC( hwnd, hdc ); /* Release DC */
                    break;
                case ID_RESET:
                    X = Y = 0;
                    break;
                case ID_HELP:
                    MessageBox( hwnd, "F2: Display\nF3: Reset", "Help", MB_OK );
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
