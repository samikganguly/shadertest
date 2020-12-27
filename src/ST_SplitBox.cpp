#include <ST_SplitBox.h>

namespace shadertest {
	
	SplitBox::SplitBox(Orientation ori, const std::wstring& name, App& app, bool resizable, Window *parent)
	: Window(name, app, WS_CHILD, parent), 
	  m_orient(ori),
	  m_resizable(resizable) {}
	
	Orientation SplitBox::orientation() const {
		return m_orient;
	}
		
	bool SplitBox::resizable() const {
		return m_resizable;
	}
	
	void SplitBox::add_child(Window *child_window, bool h_expand, bool v_expand) {
		m_children.emplace_back(child_window, h_expand, v_expand);
	}
	
	Window *SplitBox::get_child(size_t index) {
		return m_children.at(index).win;
	}
	
	void SplitBox::on_resize(WPARAM type, int new_width, int new_height) {
		int m = 10;
		int margin = std::max(static_cast<int>(m_children.size()) - 1, 0) * m;
		
		// get children area width and height according to orientation
		int cw = 0, ch = 0, hc_expand = 0, vc_expand = 0;
		if(orientation() == Orientation::HORIZONTAL) {
			cw += margin;
		} else {
			ch += margin;
		}
		for(const Child& child : m_children) {
			if(child.win) {
				hc_expand += (child.h_expand ? 1 : 0);
				vc_expand += (child.v_expand ? 1 : 0);
				int w = 0, h = 0;
				std::tie(w, h) = child.win->size();
				if(orientation() == Orientation::HORIZONTAL) {
					cw += w;
					ch = ch < h ? h : ch;
				} else {
					cw = cw < w ? w : cw;
					ch += h;
				}
			}
		}
		
		int extra_w = 0, extra_h = 0;
		if(new_width > cw) {
			extra_w = (new_width - cw) / std::max(hc_expand, 1);
		}
		if(new_height > ch) {
			extra_h = (new_height - ch) / std::max(vc_expand, 1);
		}
		int l = 0, t = 0;
		for(Child& child : m_children) {
			if(child.win) {
				int w = 0, h = 0;
				std::tie(w, h) = child.win->size();
				if(orientation() == Orientation::HORIZONTAL) {
					w += (child.h_expand ? extra_w : 0);
					child.win->move(l, t, w, (child.v_expand ? ch : h));
					l += w + m;
				} else {
					h += (child.v_expand ? extra_h : 0);
					child.win->move(l, t, (child.h_expand ? cw : w), h);
					t += h + m;
				}
			}
		}
	}
}