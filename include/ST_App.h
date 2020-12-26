#ifndef APP_H
#define APP_H

#include <ST_common.h>

namespace shadertest {
	
	class App {
	public:
		App(HINSTANCE hInstance);
		
		HINSTANCE module() const;
	
		void start();
	private:
		HINSTANCE	m_module;
	
		void start_msg_loop();
	};
	
}

#endif