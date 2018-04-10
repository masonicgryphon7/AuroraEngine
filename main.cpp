#include "CoreEngine.h"

using namespace std;

static std::unique_ptr<CoreEngine> m_coreEngine;

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	CoreEngine engine = CoreEngine();
	MSG msg = engine.Run(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	return (int)msg.wParam;
}
