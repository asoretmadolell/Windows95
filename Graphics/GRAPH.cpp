/* Set the mapping mode, the window and the viewport extents. */

#include <Windows.h>
//#include <string.h>
#include <stdio.h>
#include "graph.h"

LRESULT CALLBACK WindowFunc( HWND, UINT, WPARAM, LPARAM );

char szWinName[] = "MyWin"; /* name of window class */

char str[ 255 ]; /* holds output strings */

int maxX, maxY; /* screen dimensions */

int X = 10, Y = 10; /* viewport extents */
int orgX = 0, orgY = 0; /* viewport origin */

HDC memdc; /* handle of memory DC */
HBITMAP hbit; /* handle of compatible bitmap */
HBRUSH hbrush, hOldbrush; /* handles of brushes */

/* create pens */
HPEN hOldpen; /* handle of old pen */
HPEN hRedpen, hGreenpen, hBluepen, hYellowpen;

int WINAPI WinMain( HINSTANCE hThisInst, HINSTANCE hPreviInst, LPSTR lpszArgs, int nWinMode )
{
    HWND hwnd;
    MSG msg;
    WNDCLASS wcl;
    HANDLE hAccel;

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
        "Changing Mapping Modes", /* title */
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
        if( !TranslateAccelerator( hwnd, (HACCEL)hAccel, &msg ) )
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

    switch( message )
    {
        case WM_CREATE:
            /* get screen coordinates */
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

            hRedpen = CreatePen( PS_SOLID, 1, RGB( 255, 0, 0 ) );
            hGreenpen = CreatePen( PS_SOLID, 2, RGB( 0, 255, 0 ) );
            hBluepen = CreatePen( PS_SOLID, 3, RGB( 0, 0, 255 ) );
            hYellowpen = CreatePen( PS_SOLID, 4, RGB( 255, 255, 0 ) );

            /* save default pen */
            hOldpen = (HPEN)SelectObject( memdc, hRedpen );
            SelectObject( memdc, hOldpen );

            ReleaseDC( hwnd, hdc );
            break;
        case WM_COMMAND:
            switch( LOWORD( wParam ) )
            {
                case ID_LINES:
                    /* set 2 pixels */
                    SetPixel( memdc, 40, 14, RGB( 0, 0, 0 ) );
                    SetPixel( memdc, 40, 15, RGB( 0, 0, 0 ) );

                    LineTo( memdc, 100, 50 );
                    MoveToEx( memdc, 100, 50, NULL );

                    /* change to green pen */
                    hOldpen = (HPEN)SelectObject( memdc, hGreenpen );
                    LineTo( memdc, 200, 100 );

                    /* change to yellow pen */
                    SelectObject( memdc, hYellowpen );
                    LineTo( memdc, 0, 200 );

                    /* change to blue pen */
                    SelectObject( memdc, hBluepen );
                    LineTo( memdc, 200, 200 );

                    /* change to red pen */
                    SelectObject( memdc, hRedpen );
                    LineTo( memdc, 0, 0 );

                    /* return to default pen */
                    SelectObject( memdc, hOldpen );

                    Arc( memdc, 0, 0, 300, 300, 0, 50, 200, 50 );
                    /* show intersecting lines that define arc */
                    MoveToEx( memdc, 150, 150, NULL );
                    LineTo( memdc, 0, 50 );
                    MoveToEx( memdc, 150, 150, NULL );
                    LineTo( memdc, 200, 50 );

                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_RECTANGLES:
                    /* display, but don't fill */
                    hOldbrush = (HBRUSH)SelectObject( memdc, GetStockObject( HOLLOW_BRUSH ) );

                    /* draw some rectangles */
                    Rectangle( memdc, 50, 50, 300, 300 );
                    RoundRect( memdc, 125, 125, 220, 240, 15, 13 );

                    /* use a red pen */
                    SelectObject( memdc, hRedpen );
                    Rectangle( memdc, 100, 100, 200, 200 );
                    SelectObject( memdc, hOldpen ); /* return to default pen */

                    /* restore default brush */
                    SelectObject( memdc, hOldbrush );

                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_ELLIPSES:
                    /* make blue brush */
                    hbrush = CreateSolidBrush( RGB( 0, 0, 255 ) );
                    hOldbrush = (HBRUSH)SelectObject( memdc, hbrush );

                    /* fill these ellipses with blue */
                    Ellipse( memdc, 50, 200, 100, 280 );
                    Ellipse( memdc, 75, 25, 280, 100 );

                    /* use a red pen and fill with green */
                    SelectObject( memdc, hRedpen );
                    DeleteObject( hbrush ); /* delete brush */
                    /* create green brush */
                    hbrush = CreateSolidBrush( RGB( 0, 255, 0 ) );
                    SelectObject( memdc, hbrush ); /* select green brush */
                    Ellipse( memdc, 100, 100, 200, 200 );

                    /* draw a pie slice */
                    Pie( memdc, 200, 200, 340, 340, 225, 200, 200, 250 );

                    SelectObject( memdc, hOldpen ); /* return to default pen */
                    SelectObject( memdc, hOldbrush ); /* select default brush */
                    DeleteObject( hbrush ); /* delete green brush */

                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_SIZE: /* increment size by 10 each time */
                    X += 10;
                    Y += 10;
                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_ORG: /* change viewport origin */
                    orgX += 50;
                    orgY += 50;
                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_RESET:
                    /* reset current position to 0,0 */
                    MoveToEx( memdc, 0, 0, NULL );
                    /* erase by repainting background */
                    PatBlt( memdc, 0, 0, maxX, maxY, PATCOPY );
                    InvalidateRect( hwnd, NULL, 1 );
                    break;
                case ID_HELP:
                    MessageBox( hwnd, "F2: Lines\nF3: Rectangles\nF4: Ellipses\nF5: Magnify\nF6: Origin\nF7: Reset", "Graphics Fun", MB_OK );
                    break;
            }
            break;
        case WM_PAINT: /* process a repaint request */
            hdc = BeginPaint( hwnd, &paintstruct ); /* get DC */

            /* set mapping mode, window and viewport extents */
            SetMapMode( hdc, MM_ANISOTROPIC );
            SetWindowExtEx( hdc, 200, 200, NULL );
            SetViewportExtEx( hdc, X, Y, NULL );
            SetViewportOrgEx( hdc, orgX, orgY, NULL );

            /* now, copy memory image onto screen */
            BitBlt( hdc, 0, 0, maxX, maxY, memdc, 0, 0, SRCCOPY );
            EndPaint( hwnd, &paintstruct ); /* release DC */
            break;
        case WM_DESTROY: /* terminate the program */
            DeleteObject( hRedpen ); /* delete pens */
            DeleteObject( hGreenpen );
            DeleteObject( hBluepen );
            DeleteObject( hYellowpen );

            DeleteDC( memdc );
            PostQuitMessage( 0 );
            break;
        default:
            /* Let Windows 95 process any messages not specified in the preceding switch statement. */
            return DefWindowProc( hwnd, message, wParam, lParam );
    }
    return 0;
}
