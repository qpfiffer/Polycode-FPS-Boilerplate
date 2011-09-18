#include <Polycode.h>
#include "PolycodeView.h"
#include "World3D.h"
#include "Windows.h"

// enable Visual C++ memory leak checking
#ifdef _DEBUG
#include <ostream>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace Polycode;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    PolycodeView *view = new PolycodeView(hInstance, nCmdShow, L"---");
    World3D *main = new World3D(view);

    MSG msg;
    do {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    } while (main->update());

    delete main;
    delete view;

    return msg.wParam;
}