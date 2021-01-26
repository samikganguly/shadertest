#ifndef APP_H
#define APP_H

#include <ST_common.h>
#include <exception>

namespace shadertest {
	
	class App {
	public:
		App(HINSTANCE hInstance);
		
		HINSTANCE module() const;
	
		void start();
		
		void register_exception(const std::exception_ptr& ex);
	private:
		HINSTANCE			m_module;
		std::exception_ptr	m_err_ptr;
	
		void start_msg_loop();
	};
	
}

#endif