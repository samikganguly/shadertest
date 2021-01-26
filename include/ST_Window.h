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
		WindowClass(App& app, const std::wstring& name = L"ShaderWindowClass");
	
		WindowClass(App& app, WNDPROC proc, const std::wstring& name = L"ShaderWindowClass");
		
		virtual ~WindowClass();
		
		const std::wstring& name() const;
		
		ATOM handle() const;

		App& app();
		
		void register_exception(const std::exception_ptr& ex);
	private:
		WNDCLASS		m_class;
		App				*m_app;
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
		
		Window(Window&& other);
		
		Window& operator =(Window&& other);

		virtual ~Window();
		
		void register_exception(const std::exception_ptr& ex);

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
		
		void set_cursor(LPCTSTR system_cursor_code);
		
		void assign_cursor();
		
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
		HCURSOR									m_cursor;
		static std::unique_ptr<WindowClass>		s_class;
		
		Window() noexcept;
	};

}

#endif