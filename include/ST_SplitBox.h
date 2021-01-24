#ifndef SPLIT_BOX_H
#define SPLIT_BOX_H

#include <vector>
#include <ST_Window.h>
#include <ST_Tile.h>

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
		
		void remove_child(size_t index);
		
		int spacing() const;
		
		void set_spacing(int s);
		
		std::pair<int, int> min_size() const override;
		
		void show(int state) override;
		
		void on_resize(WPARAM type, int new_width, int new_height) override;
	private:
		class Spacer : public Window {
		public:
			Spacer(size_t index, SplitBox *parent, Window *left, Window *right, App& app);
			
			Window *left() const;
			
			Window *right() const;
			
			void on_drag(int x, int y) override;
		private:
			Window		*m_left;
			Window		*m_right;
		};
	
		Orientation								m_orient;
		int										m_spacing;
		bool									m_resizable;
		std::vector<Child>						m_children;
		std::vector<std::unique_ptr<Spacer>>	m_spacers;
		App										*m_app_ptr;
	};
	
}

#endif