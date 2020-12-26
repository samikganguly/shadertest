#include <ST_App.h>

namespace shadertest {
	
	App::App(HINSTANCE hInstance) : m_module(hInstance) {}
	
	void App::start() {
		start_msg_loop();
	}
	
	HINSTANCE App::module() const {
		return m_module;
	}

	void App::start_msg_loop() {
		for(MSG msg{}; GetMessage(&msg, nullptr, 0, 0); ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

}