/* Create a custom font. */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include "text.h"

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

char str[ 255 ]; /* holds output strings */
char fname[ 40 ] = "Default"; /* name of font */

int X = 0, Y = 0; /* current output location */
int maxX, maxY; /* screen dimensions */

HDC memdc; /* store the virtual device handle */
HBITMAP hbit; /* store the virtual bitmap */
HBRUSH hbrush; /* store the brush handle */
HFONT holdf, hnewf1, hnewf2; /* store the font handles */

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
    PAINTSTRUCT paintstruct;
    static TEXTMETRIC tm;
    SIZE size;
    static int fontswitch = 0;

    switch( message )
    {
        case WM_CREATE:
            /* get creen coordinates */
            maxX = GetSystemMetrics( SM_CXSCREEN );
            maxY = GetSystemMetrics( SM_CYSCREEN );

            /* make a compatible memory image device */
            hdc = GetDC( hwnd );
            memdc = CreateCompatibleDC( hdc );
            hbit = CreateCompatibleBitmap( hdc, maxX, maxY );
            SelectObject( memdc, hbit );
            hbrush = (HBRUSH)GetStockObject( WHITE_BRUSH );
            SelectObject( memdc, hbrush );
            PatBlt( memdc, 0, 0, maxX, maxY, PATCOPY );

            /* create a new font */
            hnewf1 = CreateFont( 14, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Courier New" );
            hnewf2 = CreateFont( 20, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Century Gothic" );

            ReleaseDC( hwnd, hdc );
            break;
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case ID_SHOW:
                    /* set text color to black and mode to transparent */
                    SetTextColor( memdc, RGB( 0, 0, 0 ) );
                    SetBkMode( memdc, TRANSPARENT );

                    /* get text metrics */
                    GetTextMetrics( memdc, &tm );

                    sprintf( str, "%s font is %ld pixels high.", fname, tm.tmHeight );
                    TextOut( memdc, X, Y, str, strlen( str ) ); /* output string */
                    Y = Y + tm.tmHeight + tm.tmExternalLeading; /* next line */

                    strcpy( str, "This is on the next line. " );
                    TextOut( memdc, X, Y, str, strlen( str ) ); /* output string */

                    /* compute length of a string */
                    GetTextExtentPoint32( memdc, str, strlen( str ), &size );
                    sprintf( str, "Previous string is %ld units long", size.cx );
                    X = size.cx; /* advance to end of previous string */
                    TextOut( memdc, X, Y, str, strlen( str ) );
                    Y = Y + tm.tmHeight + tm.tmExternalLeading; /* next line */
                    X = 0; /* reset X */

                    sprintf( str, "Screen dimensions: %d %d", maxX, maxY );
                    TextOut( memdc, X, Y, str, strlen( str ) );
                    Y = Y + tm.tmHeight + tm.tmExternalLeading; /* next line */
                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_RESET:
                    X = Y = 0;
                    /* erase by repainting background */
                    PatBlt( memdc, 0, 0, maxX, maxY, PATCOPY );
                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_FONT:
                    switch( fontswitch )
                    {
                        case 0: /* switch to new font1 */
                            holdf = (HFONT)SelectObject( memdc, hnewf1 );
                            fontswitch = 1;
                            strcpy( fname, "Courier New" );
                            break;
                        case 1: /* switch to new font2 */
                            SelectObject( memdc, hnewf2 );
                            fontswitch = 2;
                            strcpy( fname, "Century Gothic" );
                            break;
                        default: /* switch to old font */
                            SelectObject( memdc, holdf );
                            fontswitch = 0;
                            strcpy( fname, "Default" );
                    }
                    break;
                case ID_HELP:
                    MessageBox( hwnd, "F2: Display\nF3: Change font\nF4: Reset", "Text Fun", MB_OK );
                    break;
            }
            break;
        case WM_PAINT: /* process a repaint request */
            hdc = BeginPaint( hwnd, &paintstruct ); /* get DC */

            /* now, copy memory image onto screen */
            BitBlt( hdc, 0, 0, maxX, maxY, memdc, 0, 0, SRCCOPY );
            EndPaint( hwnd, &paintstruct ); /* release DC */
            break;
        case WM_DESTROY: /* terminate the program */
            DeleteDC( memdc );
            DeleteObject( hnewf1 );
            DeleteObject( hnewf2 );
            PostQuitMessage( 0 );
            break;
        default:
            /* Let Windows 95 process any messages not specified in the preceding switch statement. */
            return DefWindowProc( hwnd, message, wParam, lParam );
    }
    return 0;
}
