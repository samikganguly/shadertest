#include <ST_Window.h>
#include <ST_WinError.h>

namespace shadertest {
	
	static void store_window(HWND hWnd, LPARAM lParam) {
		CREATESTRUCT *cStruct = reinterpret_cast<CREATESTRUCT *>(lParam);
		Window *self = reinterpret_cast<Window *>(cStruct->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(self));
	}
	
	static Window *get_window(HWND hWnd) {
		LONG_PTR self = GetWindowLongPtr(hWnd, GWLP_USERDATA);
		return reinterpret_cast<Window *>(self);
	}
	
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		switch(uMsg) {
		case WM_CREATE:
			store_window(hWnd, lParam);
			return TRUE;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd, &ps);
				if(hdc == nullptr) {
					throw WinError(L"WindowClass: Failed to obtain a device context to paint on");
				}
				Window *self = get_window(hWnd);
				self->on_paint(hdc, ps);
				EndPaint(hWnd, &ps);
			}
			return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	WindowClass::WindowClass(HINSTANCE hInstance, WNDPROC proc, const std::wstring& n) 
	: m_class{},
	  m_module(hInstance),
	  m_name(n),
	  m_class_handle(0) {
		m_class.lpfnWndProc = proc;
		m_class.hInstance = m_module;
		m_class.lpszClassName = m_name.c_str();
		m_class_handle = RegisterClass(&m_class);
		if(m_class_handle == 0) {
			throw WinError(L"GLWindowClass: Failed to register window class '" + m_name + L"'");
		}
	}
	
	WindowClass::WindowClass(HINSTANCE hInstance, const std::wstring& n) : WindowClass(hInstance, WindowProc, n) {}

	WindowClass::~WindowClass() {
		if(m_class_handle != 0) {
			UnregisterClass(reinterpret_cast<LPCWSTR>(m_class_handle), m_module);
		}
	}

	const std::wstring& WindowClass::name() const {
		return m_name;
	}

	ATOM WindowClass::handle() const {
		return m_class_handle;
	}

	HINSTANCE WindowClass::module() const {
		return m_module;
	}

	std::unique_ptr<WindowClass> Window::s_class = nullptr;

	Window::Window(const std::wstring& n, App& app, DWORD style, Window *parent)
	: m_name(n),
	  m_parent(parent) {
		if(s_class == nullptr) {
			s_class = std::make_unique<WindowClass>(app.module());
		}
		m_handle = CreateWindowEx(
			0,
			reinterpret_cast<LPCWSTR>(s_class->handle()),
			m_name.c_str(),
			style,
			0, 0,
			10, 10,
			(m_parent ? m_parent->handle() : nullptr),
			nullptr,
			s_class->module(),
			this
		);
		if(m_handle == nullptr) {
			throw WinError(L"GLWindow: Failed to create new window with name '" + m_name + L"'");
		}
	}

	Window::~Window() {
		if(m_handle != nullptr) {
			DestroyWindow(m_handle);
		}
	}

	const std::wstring& Window::name() const {
		return m_name;
	}

	HWND Window::handle() const {
		return m_handle;
	}

	WindowClass *Window::type() {
		return s_class.get();
	}

	const WindowClass *Window::type() const {
		return s_class.get();
	}

	Window *Window::parent() const {
		return m_parent;
	}
	
	void Window::show(int state) {
		ShowWindow(m_handle, state);
	}
	
	void Window::move(int x, int y, int w, int h, bool repaint) {
		MoveWindow(m_handle, x, y, w, h, repaint);
	}
	
	void Window::on_paint(HDC dc, PAINTSTRUCT& ps) {
		FillRect(dc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
	}

}