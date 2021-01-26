#include <ST_App.h>

namespace shadertest {
	
	App::App(HINSTANCE hInstance) : m_module(hInstance) {}
	
	void App::start() {
		start_msg_loop();
	}
	
	HINSTANCE App::module() const {
		return m_module;
	}
	
	void App::register_exception(const std::exception_ptr& ex) {
		m_err_ptr = ex;
	}

	void App::start_msg_loop() {
		for(MSG msg{}; GetMessage(&msg, nullptr, 0, 0); ) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			if(m_err_ptr) {
				std::exception_ptr err_ptr_copy;
				std::swap(err_ptr_copy, m_err_ptr);
				std::rethrow_exception(err_ptr_copy);
			}
		}
	}

}