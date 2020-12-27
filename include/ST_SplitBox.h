#ifndef SPLIT_BOX_H
#define SPLIT_BOX_H

#include <vector>
#include <ST_Window.h>

namespace shadertest {
	
	enum class Orientation {
		HORIZONTAL, VERTICAL
	};
	
	class SplitBox : public Window {
	public:
		SplitBox(Orientation ori, const std::wstring& name, App& app, bool resizable = false, Window *parent = nullptr);
		
		Orientation orientation() const;
		
		bool resizable() const;
		
		void add_child(Window *child_window, bool h_expand = false, bool v_expand = false);
		
		Window *get_child(size_t index);
		
		void on_resize(WPARAM type, int new_width, int new_height) override;
	private:
		Orientation			m_orient;
		bool				m_resizable;
		std::vector<Child>	m_children;
	};
	
}

#endif