#include <fstream>
#include <ST_common.h>
#include <ST_WinError.h>
#include <ST_App.h>
#include <ST_Tile.h>
#include <ST_SplitBox.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	try {
		
		shadertest::App app(hInstance);
		shadertest::Window w(L"Tile Test", app);
		shadertest::SplitBox sp(shadertest::Orientation::VERTICAL, L"HBox", app, false, &w);
		shadertest::Tile t1(RGB(255, 0, 0), L"t1", app, &sp);
		shadertest::Tile t2(RGB(0, 255, 0), L"t2", app, &sp);
		shadertest::Tile t3(RGB(0, 0, 255), L"t3", app, &sp);
		t1.move(0, 0, 50, 20);
		t2.move(0, 0, 50, 50);
		t3.move(0, 0, 50, 50);
		w.set_child(&sp, true, true);
		sp.add_child(&t1);
		sp.add_child(&t2, true, true);
		sp.add_child(&t3, false, true);
		w.move(50, 50, 200, 200);
		t1.show();
		t2.show();
		t3.show();
		sp.show();
		w.show(nCmdShow);
		app.start();
		
	} catch(const shadertest::WError& err) {
		std::wofstream log("Log.txt");
		log << err.wwhat() << std::endl;
	} catch(const std::exception& err1) {
		std::ofstream log("Log.txt");
		log << err1.what() << std::endl;
	}
	
	return 0;
}