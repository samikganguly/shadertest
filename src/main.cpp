#include <ST_common.h>
#include <ST_WinError.h>
#include <ST_App.h>
#include <ST_Tile.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	try {
		
		shadertest::App app(hInstance);
		shadertest::Window w(L"Tile Test", app);
		shadertest::Tile t1(RGB(255, 0, 0), L"t1", app, &w);
		shadertest::Tile t2(RGB(0, 255, 0), L"t2", app, &w);
		t1.move(10, 10, 50, 50);
		t1.show();
		t2.move(70, 10, 50, 50);
		t2.show();
		w.move(50, 50, 200, 200);
		w.show(nCmdShow);
		app.start();
		
	} catch(const shadertest::WError& err) {
		MessageBox(
			nullptr,
			err.wwhat(),
			L"Error",
			MB_OK | MB_ICONERROR
		);
	}
	
	return 0;
}