#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <memory>
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

	class Window {
	public:
		Window(const std::wstring& name, App& app, DWORD style = WS_OVERLAPPEDWINDOW, Window *parent = nullptr);

		virtual ~Window();

		const std::wstring& name() const;

		HWND handle() const;

		WindowClass *type();

		const WindowClass *type() const;

		Window *parent() const;
		
		void move(int x, int y, int width, int height, bool repaint = true);
		
		void show(int state = SW_SHOW);
		
		virtual void on_paint(HDC dc, PAINTSTRUCT& ps);
	private:
		std::wstring							m_name;
		HWND									m_handle;
		Window									*m_parent;
		static std::unique_ptr<WindowClass>		s_class;
	};

}

#endif