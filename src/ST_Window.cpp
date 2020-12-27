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
		case WM_SIZE:
			{
				Window *self = get_window(hWnd);
				self->on_resize(wParam, static_cast<int>(LOWORD(lParam)), static_cast<int>(HIWORD(lParam)));
			}
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
			throw WinError(L"WindowClass: Failed to register window class '" + m_name + L"'");
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
	
	Child::Child() : win(nullptr), h_expand(false), v_expand(false) {}
	
	Child::Child(Window *child_window, bool h_expand, bool v_expand)
	: win(child_window),
	  h_expand(h_expand),
	  v_expand(v_expand) {}

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
			throw WinError(L"Window: Failed to create new window with name '" + m_name + L"'");
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
	
	std::pair<int, int> Window::size() const {
		RECT sz;
		if(not GetClientRect(m_handle, &sz)) {
			throw WinError(L"Window: Failed to fetch size of the window '" + m_name + L"'");
		}
		return {sz.right - sz.left, sz.bottom - sz.top};
	}
	
	void Window::show(int state) {
		ShowWindow(m_handle, state);
	}
	
	void Window::move(int x, int y, int w, int h, bool repaint) {
		if(not MoveWindow(m_handle, x, y, w, h, repaint)) {
			throw WinError(L"Window: Failed to move the window '" + m_name + L"'");
		}
	}
	
	void Window::set_child(Window *child, bool h_expand, bool v_expand) {
		m_child.win = child;
		m_child.h_expand = h_expand;
		m_child.v_expand = v_expand;
	}
	
	void Window::on_paint(HDC dc, PAINTSTRUCT& ps) {
		FillRect(dc, &ps.rcPaint, reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1));
	}
	
	void Window::on_resize(WPARAM type, int new_width, int new_height) {
		if(m_child.win) {
			int w = 0, h = 0;
			std::tie(w, h) = m_child.win->size();
			w = m_child.h_expand ? std::max(new_width, w) : w;
			h = m_child.v_expand ? std::max(new_height, h) : h;
			m_child.win->move(0, 0, w, h);
		}
	}

}