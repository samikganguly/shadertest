#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>
#include <utility>
#include <ST_common.h>
#include <ST_App.h>

namespace shadertest {

	class WindowClass {
	public:
		WindowClass(HINSTANCE hInstance, const std::wstring& name = L"ShaderWindowClass");
	
		WindowClass(HINSTANCE hInstance, WNDPROC proc, const std::wstring& name = L"ShaderWindowClass");
		
		virtual ~WindowClass();
		
		const std::wstring& name() const;
		
		ATOM handle() const;

		HINSTANCE module() const;
	private:
		WNDCLASS		m_class;
		HINSTANCE		m_module;
		std::wstring	m_name;
		ATOM			m_class_handle;
	};


	class Window;
	
	struct Child {
		Window	*win;
		bool	h_expand;
		bool	v_expand;

		Child();

		Child(Window *win, bool h_expand, bool v_expand);
	};

	class Window {
	public:
		Window(const std::wstring& name, App& app, DWORD style = WS_OVERLAPPEDWINDOW, Window *parent = nullptr);

		virtual ~Window();

		const std::wstring& name() const;

		HWND handle() const;

		WindowClass *type();

		const WindowClass *type() const;

		Window *parent() const;
		
		std::pair<int, int> size() const;
		
		void move(int x, int y, int width, int height, bool repaint = true);
		
		void show(int state = SW_SHOW);
		
		void set_child(Window *child, bool h_expand, bool v_expand);
		
		virtual void on_paint(HDC dc, PAINTSTRUCT& ps);
		
		virtual void on_resize(WPARAM type, int new_width, int new_height);
	private:
		std::wstring							m_name;
		HWND									m_handle;
		Window									*m_parent;
		Child									m_child;
		static std::unique_ptr<WindowClass>		s_class;
	};

}

#endif