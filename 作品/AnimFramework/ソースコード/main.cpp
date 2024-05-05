#define _CRTDBG_MAP_ALLOC
#include "App.h"
#include <crtdbg.h>

int wmain(int argc, wchar_t** argv, wchar_t** envp) {

#if ( defined(DEBUG) | defined(_DEBUG))
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	App app(1280, 720);
	app.Run();

	return 0;
};