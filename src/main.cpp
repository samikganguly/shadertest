#include <ST_common.h>
#include <ST_WinError.h>
#include <ST_App.h>
#include <ST_Tile.h>
#include <ST_SplitBox.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
	try {
		
		shadertest::App app(hInstance);
		shadertest::Window w(L"Tile Test", app);
		shadertest::SplitBox sp1(shadertest::Orientation::HORIZONTAL, L"HBox1", app, false, &w);
		shadertest::SplitBox sp2(shadertest::Orientation::VERTICAL, L"HBox2", app, false, &sp1);
		shadertest::Tile t1(RGB(255, 0, 0), L"t1", app, &sp1);
		shadertest::Tile t2(RGB(0, 255, 0), L"t2", app, &sp2);
		shadertest::Tile t3(RGB(128, 255, 0), L"t3", app, &sp2);
		shadertest::Tile t4(RGB(0, 255, 128), L"t4", app, &sp2);
		shadertest::Tile t5(RGB(0, 0, 255), L"t5", app, &sp1);
		t1.set_min_size(50, 20);
		t2.set_min_size(50, 50);
		t3.set_min_size(50, 50);
		t4.set_min_size(50, 50);
		t5.set_min_size(50, 50);
		w.set_child(&sp1, true, true);
		sp1.add_child(&t1, false, true);
		sp1.add_child(&sp2, true, true);
		sp1.add_child(&t5, false, true);
		sp2.add_child(&t2, true, false);
		sp2.add_child(&t3, true, true);
		sp2.add_child(&t4, true, false);
		w.move(50, 50, 200, 200);
		w.show(nCmdShow);
		app.start();
		
	} catch(const shadertest::WError& err) {
		shadertest::wlog_file << err.wwhat() << std::endl;
	} catch(const std::exception& err1) {
		shadertest::log_file << err1.what() << std::endl;
	}
	
	return 0;
}