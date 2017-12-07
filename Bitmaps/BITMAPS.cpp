/* Demonstrate two bitmaps. */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

HBITMAP hBit1, hBit2; /* handle of bitmaps */

int WINAPI WinMain( HINSTANCE hThisInst, HINSTANCE hPreviInst, LPSTR lpszArgs, int nWinMode )
{
    HWND hwnd;
    MSG msg;
    WNDCLASS wcl;

    /* Define a window class. */
    wcl.hInstance = hThisInst; /* handle to this instance */
    wcl.lpszClassName = szWinName; /* window class name */
    wcl.lpfnWndProc = WindowFunc; /* window function */
    wcl.style = 0; /* default style */

    wcl.hIcon = LoadIcon( hThisInst, "MYICON" ); /* icon style */
    wcl.hCursor = LoadCursor( hThisInst, "MYCURSOR" ); /* cursor style */

    wcl.lpszMenuName = NULL; /* no main menu */

    wcl.cbClsExtra = 0; /* no extra */
    wcl.cbWndExtra = 0; /* information needed */

    /* Make the window white. */
    wcl.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );

    /* Register the window class. */
    if( !RegisterClass( &wcl ) ) return 0;

    /* Now that a window class has been registered, a window can be created. */
    hwnd = CreateWindow(
        szWinName, /* name of window class */
        "Two Custom Bitmaps", /* title */
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

    /* Display the window. */
    ShowWindow( hwnd, nWinMode );
    UpdateWindow( hwnd );

    /* load the bitmaps */
    hBit1 = LoadBitmap( hThisInst, "MYBP1" ); /* load bitmap */
    hBit2 = LoadBitmap( hThisInst, "MYBP2" ); /* load bitmap */

    /* Create the message loop. */
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg ); /* allow use of keyboard */
        DispatchMessage( &msg ); /* return control to Windows */
    }

    return msg.wParam;
}

/* This function is called by Windows 95 and is passed messages from the message queue. */

LRESULT CALLBACK WindowFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    HDC DC, memDC;

    switch( message )
    {
        case WM_LBUTTONDOWN: /* display the first bitmap */
            DC = GetDC( hwnd ); /* get device context */
            memDC = CreateCompatibleDC( DC ); /* create compatible DC */
            SelectObject( memDC, hBit1 );
            BitBlt( DC, LOWORD( lParam ), HIWORD( lParam ), 64, 64, memDC, 0, 0, SRCCOPY ); /* build image */
            ReleaseDC( hwnd, DC ); /* free the device context */
            DeleteDC( memDC ); /* free the memory context */
            break;
        case WM_RBUTTONDOWN:
            DC = GetDC( hwnd ); /* get device context */
            memDC = CreateCompatibleDC( DC ); /* create compatible DC */
            SelectObject( memDC, hBit2 );
            BitBlt( DC, LOWORD( lParam ), HIWORD( lParam ), 64, 64, memDC, 0, 0, SRCCOPY ); /* build image */
            ReleaseDC( hwnd, DC ); /* free the device context */
            DeleteDC( memDC ); /* free the memory context */
            break;
        case WM_DESTROY: /* terminate the program */
            DeleteObject( hBit1 ); /* remove the bitmaps */
            DeleteObject( hBit2 );
            PostQuitMessage( 0 );
            break;
        default:
            /* Let Windows 95 process any messages not specified in the preceding switch statement. */
            return DefWindowProc( hwnd, message, wParam, lParam );
    }
    return 0;
}
