#include <Polycode.h>
#include "PolycodeView.h"
#include "World3D.h"
#include "Windows.h"

using namespace Polycode;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PolycodeView *view = new PolycodeView(hInstance, nCmdShow, L"---");
    //PolycodeView *view = (PolycodeView *)malloc(sizeof(PolycodeView));
    World3D *main = new World3D(view);

    HWND hWnd = *((HWND*)view->windowData);
    RECT rcClip;           // new area for ClipCursor
    RECT rcOldClip;        // previous area for ClipCursor
    // Record the area in which the cursor can move. 
    GetClipCursor(&rcOldClip); 
    // Get the dimensions of the application's window. 
    GetWindowRect(hWnd, &rcClip); 
    // Confine the cursor to the application's window. 
    if (ClipCursor(&rcClip) == 0) {
        DWORD error = GetLastError();
    }

    MSG msg;
    do {
        //SetCapture(hWnd);
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE | PM_QS_INPUT)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } while (main->update());

    //ClipCursor(&rcOldClip); 

    delete main;
    delete view;

    return msg.wParam;
}