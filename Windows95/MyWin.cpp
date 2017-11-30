/* A clock program. */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include <time.h>

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

char str[ 80 ] = "Sample Output"; /* holds output string */

int X = 1, Y = 1; /* screen location */

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

    wcl.hIcon = LoadIcon( NULL, IDI_APPLICATION ); /* icon style */
    wcl.hCursor = LoadCursor( NULL, IDC_ARROW ); /* cursor style */
    wcl.lpszMenuName = NULL; /* no menu */

    wcl.cbClsExtra = 0; /* no extra */
    wcl.cbWndExtra = 0; /* information needed */

    /* Make the window white. */
    wcl.hbrBackground = (HBRUSH)GetStockObject( WHITE_BRUSH );

    /* Register the window class. */
    if( !RegisterClass( &wcl ) ) return 0;

    /* Now that a window class has been registered, a window can be created. */
    hwnd = CreateWindow(
        szWinName, /* name of window class */
        "Clock", /* title */
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

    /* start a timer - interrupt once per second */
    SetTimer( hwnd, 1, 1000, NULL );

    UpdateWindow( hwnd );

    /* Create the message loop. */
    while( GetMessage( &msg, NULL, 0, 0 ) )
    {
        TranslateMessage( &msg ); /* allow use of keyboard */
        DispatchMessage( &msg ); /* return control to Windows */
    }

    KillTimer( hwnd, 1 ); /* stop the timer */

    return msg.wParam;
}

/* This function is called by Windows 95 and is passed messages from the message queue. */

LRESULT CALLBACK WindowFunc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
    HDC hdc;
    PAINTSTRUCT paintstruct;
    struct tm* newtime;
    time_t t;

    switch( message )
    {
        case WM_PAINT: /* process a repaint request */
            hdc = BeginPaint( hwnd, &paintstruct ); /* get DC */
            TextOut( hdc, X, Y, str, strlen( str ) ); /* output string */
            EndPaint( hwnd, &paintstruct ); /* release DC */
            break;
        case WM_TIMER: /* timer went off */
            /* get the new time */
            t = time( NULL );
            newtime = localtime( &t );

            /* display the new time */
            strcpy( str, asctime( newtime ) );
            str[ strlen( str ) - 1 ] = '\0'; /* remove /r/n */
            InvalidateRect( hwnd, NULL, 0 ); /* update screen */
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
