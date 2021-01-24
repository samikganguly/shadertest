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
		
		RECT bound() const;
		
		void set_min_size(int width, int height);
		
		void enable_drag();
		
		void disable_drag();
		
		bool drag_enabled() const;
		
		void grab_focus();
		
		void release_focus();
		
		bool has_focus() const;
		
		void move(int x, int y, int width, int height, bool repaint = true);
		
		void set_child(Window *child, bool h_expand, bool v_expand);
		
		virtual std::pair<int, int> min_size() const;
		
		virtual void show(int state = SW_SHOW);
		
		virtual void on_paint(HDC dc, PAINTSTRUCT& ps);
		
		virtual void on_resize(WPARAM type, int new_width, int new_height);
		
		virtual void on_mouse_down(int mouse_button, int x, int y);
		
		virtual void on_mouse_up(int mouse_button, int x, int y);
		
		virtual void on_mouse_move(int x, int y);
		
		virtual void on_drag(int x, int y);
	private:
		std::wstring							m_name;
		HWND									m_handle;
		int										m_min_width;
		int										m_min_height;
		bool									m_drag_enabled;
		bool									m_has_focus;
		Window									*m_parent;
		Child									m_child;
		static std::unique_ptr<WindowClass>		s_class;
	};

}

#endif