#define  STRICT //строгое присваивание типов
#define  WIN32_LEAN_AND_MEAN // Exclude rarely-used stuff from Windows headers
#define nameClass "kjorugeoa"

#include <cstdlib>
#include <ctime>
#include <Windows.h>
#include "Resource.h"

#define XSCREEN GetSystemMetrics(SM_CXSCREEN) *0.8
#define YSCREEN GetSystemMetrics(SM_CYSCREEN) *0.6
#define SIZE 500
#define RANGE 3500
//line и page лучше просто константами
#define LINE 0.01 * RANGE
#define PAGE 0.2 * RANGE
#define RADIUS 70
#define COORD 4000

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

//точка входа
int WINAPI WinMain(
    _In_ HINSTANCE hInstance, //экземпляр дескр.
    _In_opt_ HINSTANCE, //hPrevInstance,
    _In_ LPSTR, // lpCmdLine,
    _In_ int nCmdShow
) {

    //стр-ра окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = nameClass;
    wc.hbrBackground = HBRUSH(COLOR_WINDOW + 1);

    //регистрация окна
    if (::RegisterClass(&wc) == NULL) {
        return -1;
    }

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

    if (!hMenu) {
        return -1;
    }

    HWND hWnd = CreateWindow(nameClass, "Главное окно", WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL,
        (GetSystemMetrics(SM_CXSCREEN) - (int)XSCREEN) / 2, (GetSystemMetrics(SM_CYSCREEN) - (int)YSCREEN) / 2,
        (int)XSCREEN, (int)YSCREEN, NULL, hMenu, hInstance, NULL);

    if (hWnd == NULL) {
        return -1;
    }

    ::ShowWindow(hWnd, nCmdShow);

    POINT point[SIZE];

    srand(time(NULL));
    for (int i = 0; i < SIZE; ++i) {
        point[i].x = rand() % (COORD);
        point[i].y = rand() % (COORD);
    }

    SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)point);

    MSG msg;

    while (::GetMessage(&msg, NULL, NULL, NULL)) {
        ::DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

    switch (message) {

    //case WM_CREATE:
    //{
    // //смерть
    //}

    case WM_VSCROLL:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);

        SCROLLINFO scrollInfo = {};
        scrollInfo.cbSize = sizeof(scrollInfo);
        scrollInfo.fMask = SIF_POS | SIF_TRACKPOS | SIF_RANGE;
        GetScrollInfo(hWnd, SB_VERT, &scrollInfo);

        int scrollPos = scrollInfo.nPos;

        switch (LOWORD(wParam)) {
        case SB_LINEUP: {
            scrollPos -= (int)LINE;
            break;
        }
        case SB_LINEDOWN: {
            scrollPos += (int)LINE;
            break;
        }
        case SB_PAGEUP: {
            scrollPos -= (int)PAGE;
            break;
        }
        case SB_PAGEDOWN: {
            scrollPos += (int)PAGE;
            break;
        }
        case SB_THUMBTRACK: {
            scrollPos = scrollInfo.nTrackPos;
            break;
        }
        }

        if (scrollPos > scrollInfo.nMax) {
            scrollPos = scrollInfo.nMax;
        }
        else if (scrollPos < scrollInfo.nMin) {
            scrollPos = scrollInfo.nMin;
        }
        //насколько необходимо прокрутить
        int scroll = scrollInfo.nPos - scrollPos;

        scrollInfo.nPos = scrollPos;
        SetScrollInfo(hWnd, SB_VERT, &scrollInfo, true);

        if (scroll < 0) {
            rect.bottom = rect.top - scroll;
        }
        if (scroll > 0) {
            rect.top = rect.bottom - scroll;
        }

        ScrollWindow(hWnd, NULL, scroll, NULL, NULL);
        InvalidateRect(hWnd, &rect, true);

        return 0;
    }
    
    case WM_HSCROLL:
    {
        RECT rect;
        GetClientRect(hWnd, &rect);

        SCROLLINFO scrollInfo = {};
        scrollInfo.cbSize = sizeof(scrollInfo);
        scrollInfo.fMask = SIF_POS | SIF_TRACKPOS | SIF_RANGE;

        GetScrollInfo(hWnd, SB_HORZ, &scrollInfo);

        int scrollPos = scrollInfo.nPos;

        switch (LOWORD(wParam)) {
        case SB_LINELEFT: {
            scrollPos -= (int)LINE;

            break;
        }
        case SB_LINERIGHT: {
            scrollPos += (int)LINE;

            break;
        }
        case SB_PAGELEFT: {
            scrollPos -= (int)PAGE;

            break;
        }
        case SB_PAGERIGHT: {
            scrollPos += (int)PAGE;

            break;
        }
        case SB_THUMBTRACK: {
            scrollPos = scrollInfo.nTrackPos;
            break;
        }
        }

        if (scrollPos > scrollInfo.nMax) {
            scrollPos = scrollInfo.nMax;
        }
        else if (scrollPos < scrollInfo.nMin) {
            scrollPos = scrollInfo.nMin;
        }

        //насколько необходимо прокрутить
        int scroll = scrollInfo.nPos - scrollPos;

        scrollInfo.nPos = scrollPos;
        SetScrollInfo(hWnd, SB_HORZ, &scrollInfo, true);
       
        if (scroll < 0) {
            rect.left = rect.right - scroll;
        }
        if (scroll > 0) {
            rect.right = rect.left - scroll;
        }

        ScrollWindow(hWnd, scroll, NULL, NULL, NULL);
        InvalidateRect(hWnd, &rect, true);

        return 0;
    }

    case WM_KEYDOWN: {
        //можно добавить управление самый вверх и самый низ

        switch (wParam) {
        case VK_UP:
            PostMessage(hWnd, WM_VSCROLL, SB_LINEUP, NULL);

            break;

        case VK_DOWN:
            PostMessage(hWnd, WM_VSCROLL, SB_LINEDOWN, NULL);

            break;

        case VK_LEFT:
            PostMessage(hWnd, WM_HSCROLL, SB_LINELEFT, NULL);

            break;

        case VK_RIGHT:
            PostMessage(hWnd, WM_HSCROLL, SB_LINERIGHT, NULL);


            break;
        }
        return 0;

    }

    case WM_COMMAND: {

        HMENU hMenu = GetMenu(hWnd);

        switch (LOWORD(wParam)) {
        case ID_EXIT:
            DestroyWindow(hWnd);
            break;

        case ID_DRAW_CIRCLES: {
            EnableMenuItem(hMenu, ID_DRAW_CIRCLES, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMenu, ID_DRAW_RECTS, MF_BYCOMMAND | MF_ENABLED);
            InvalidateRect(hWnd, NULL, true);
            break;
        }

        case ID_DRAW_RECTS: {
            EnableMenuItem(hMenu, ID_DRAW_RECTS, MF_BYCOMMAND | MF_GRAYED);
            EnableMenuItem(hMenu, ID_DRAW_CIRCLES, MF_BYCOMMAND | MF_ENABLED);
            InvalidateRect(hWnd, NULL, true);
            break;
        }
        }
        return 0;
    }

    case WM_SIZE:
    {
        RECT ClientRect;
        GetClientRect(hWnd, &ClientRect);

        SCROLLINFO scrollInfo = {};
        scrollInfo.cbSize = sizeof(scrollInfo);
        scrollInfo.fMask = SIF_ALL;
        scrollInfo.nMin = 0;
        scrollInfo.nPage = (int)PAGE;


        //установка ползунка - лишняя работа
        GetScrollInfo(hWnd, SB_VERT, &scrollInfo);
        scrollInfo.nPos = scrollInfo.nTrackPos;
        scrollInfo.nMax = RANGE - (ClientRect.bottom - ClientRect.top);
        SetScrollInfo(hWnd, SB_VERT, &scrollInfo, TRUE);

        GetScrollInfo(hWnd, SB_HORZ, &scrollInfo);
        scrollInfo.nPos = scrollInfo.nTrackPos;
        scrollInfo.nMax = RANGE - (ClientRect.right - ClientRect.left);
        SetScrollInfo(hWnd, SB_HORZ, &scrollInfo, TRUE);
       

        InvalidateRect(hWnd, NULL, true);
        return 0;
    }

    case WM_PAINT:
    {
        POINT* point = (POINT*)(GetWindowLongPtr(hWnd, GWLP_USERDATA));
        if (!point) {
            return 0;
        }

        PAINTSTRUCT ps;
        HDC hDC = ::BeginPaint(hWnd, &ps);

        //прямоуг. требующий перерисовки 
        RECT paintRect = ps.rcPaint;

        HBRUSH hBrush = ::CreateSolidBrush(RGB(175, 200, 238));
        HBRUSH hBB = ::CreateSolidBrush(RGB(0, 0, 0));

        SCROLLINFO scrollInfo;
        scrollInfo.cbSize = sizeof(scrollInfo);
        scrollInfo.fMask = SIF_POS;

        GetScrollInfo(hWnd, SB_VERT, &scrollInfo);
        int vertScrollPos = scrollInfo.nPos;
        GetScrollInfo(hWnd, SB_HORZ, &scrollInfo);
        int horzScrollPos = scrollInfo.nPos;

        HMENU hMenu = GetMenu(hWnd);

        RECT newRect, frameRect, check;

        if (GetMenuState(hMenu, ID_DRAW_RECTS, MF_GRAYED)) {
            for (int i = 0; i < SIZE; ++i) {

                int left = point[i].x - RADIUS - horzScrollPos;
                int top = point[i].y - RADIUS - vertScrollPos;
                int right = point[i].x + RADIUS - horzScrollPos;
                int bottom = point[i].y + RADIUS - vertScrollPos;

                newRect = { left, top, right, bottom };
                frameRect = { left - 1, top - 1, right + 1, bottom + 1 };

                if (IntersectRect(&check, &paintRect, &newRect)) {
                    if (GetMenuState(hMenu, ID_DRAW_RECTS, MF_GRAYED)) {
                        ::FillRect(hDC, &frameRect, hBB);
                        ::FillRect(hDC, &newRect, hBrush);
                    }
                }
            }
        }
        else {
            for (int i = 0; i < SIZE; ++i) {

                int left = point[i].x - RADIUS - horzScrollPos;
                int top = point[i].y - RADIUS - vertScrollPos;
                int right = point[i].x + RADIUS - horzScrollPos;
                int bottom = point[i].y + RADIUS - vertScrollPos;

                newRect = { left, top, right, bottom };
                frameRect = { left - 1, top - 1, right + 1, bottom + 1 };

                if (IntersectRect(&check, &paintRect, &newRect)) {
                    ::SelectObject(hDC, hBrush);
                    ::Ellipse(hDC, newRect.left, newRect.top,
                        newRect.right, newRect.bottom);
                }
            }
        }

        ::DeleteObject(hBrush);
        ::DeleteObject(hBB);
        ::EndPaint(hWnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        ::PostQuitMessage(0);
        //HMENU hMenu = GetMenu(hWnd);
        //DestroyMenu(hMenu);
        return 0;

    }

    return ::DefWindowProc(hWnd, message, wParam, lParam);
}
