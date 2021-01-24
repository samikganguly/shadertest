#include <ST_SplitBox.h>

namespace shadertest {
	
	SplitBox::Spacer::Spacer(size_t index, SplitBox *parent, Window *left, Window *right, App& app)
	: Window(L"Spacer_" + std::to_wstring(index), app, WS_CHILD, parent),
	  m_left(left),
	  m_right(right) {
		if(parent->resizable()) {
			set_cursor(parent->orientation() == Orientation::HORIZONTAL ? IDC_SIZEWE : IDC_SIZENS);
		}
		enable_drag();
	}
	  
	Window *SplitBox::Spacer::left() const {
		return m_left;
	}
	
	Window *SplitBox::Spacer::right() const {
		return m_right;
	}
	
	void SplitBox::Spacer::on_drag(int x, int y) {
		SplitBox *p = dynamic_cast<SplitBox *>(parent());
		if(p && p->resizable()) {
			RECT s = bound();
			RECT l = left()->bound();
			RECT r = right()->bound();
			x += s.left;
			y += s.top;
			int lw = 0, lh = 0, rw = 0, rh = 0;
			std::tie(lw, lh) = left()->min_size();
			std::tie(rw, rh) = right()->min_size();
			if(p->orientation() == Orientation::HORIZONTAL) {
				move(x, s.top, s.right - s.left, s.bottom - s.top);
				left()->move(l.left, l.top, x - l.left, l.bottom - l.top);
				right()->move(x + p->spacing(), r.top, r.right - x - p->spacing(), r.bottom - r.top);
			} else if(p->orientation() == Orientation::VERTICAL) {
				move(s.left, y, s.right - s.left, s.bottom - s.top);
				left()->move(l.left, l.top, l.right - l.left, y - l.top);
				right()->move(r.left, y + p->spacing(), r.right - r.left, r.bottom - y - p->spacing());
			}
		}
	}
	
	SplitBox::SplitBox(Orientation ori, const std::wstring& name, App& app, bool resizable, Window *parent)
	: Window(name, app, WS_CHILD, parent), 
	  m_orient(ori),
	  m_spacing(5),
	  m_resizable(resizable),
	  m_app_ptr(&app) {}
	
	Orientation SplitBox::orientation() const {
		return m_orient;
	}
		
	bool SplitBox::resizable() const {
		return m_resizable;
	}
	
	int SplitBox::spacing() const {
		return m_spacing;
	}
	
	void SplitBox::set_spacing(int s) {
		m_spacing = s;
	}
	
	void SplitBox::add_child(Window *child_window, bool h_expand, bool v_expand) {
		if(child_window) {
			m_children.emplace_back(child_window, h_expand, v_expand);
			if(m_children.size() > 1) {
				m_spacers.emplace_back(new Spacer(m_children.size() - 1, this, (m_children.rbegin() + 1)->win, m_children.rbegin()->win, *m_app_ptr));
			}
		}
	}
	
	Window *SplitBox::get_child(size_t index) {
		return m_children.at(index).win;
	}
	
	void SplitBox::remove_child(size_t index) {
		if(m_children.size() > 1) {
			m_spacers.erase(m_spacers.begin() + (index > 0 ? index - 1 : 0));
		}
		m_children.erase(m_children.begin() + index);
	}
	
	std::pair<int, int> SplitBox::min_size() const {
		// get children area width and height according to orientation
		int margin = std::max(static_cast<int>(m_children.size()) - 1, 0) * m_spacing;
		int cw = 0, ch = 0;
		if(orientation() == Orientation::HORIZONTAL) {
			cw += margin;
		} else {
			ch += margin;
		}
		for(const Child& child : m_children) {
			if(child.win) {
				int w = 0, h = 0;
				std::tie(w, h) = child.win->min_size();
				if(orientation() == Orientation::HORIZONTAL) {
					cw += w;
					ch = ch < h ? h : ch;
				} else {
					cw = cw < w ? w : cw;
					ch += h;
				}
			}
		}
		return {cw, ch};
	}
	
	void SplitBox::show(int state) {
		for(Child& child : m_children) {
			if(child.win) {
				child.win->show();
			}
		}
		for(auto& sp : m_spacers) {
			sp->show();
		}
		Window::show(state);
	}
	
	void SplitBox::on_resize(WPARAM type, int new_width, int new_height) {
		int cw = 0, ch = 0, hc_expand = 0, vc_expand = 0;
		std::tie(cw, ch) = min_size();
		
		for(const Child& child : m_children) {
			if(child.win) {
				hc_expand += (child.h_expand ? 1 : 0);
				vc_expand += (child.v_expand ? 1 : 0);
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
		size_t ci = 0;
		for(Child& child : m_children) {
			if(child.win) {
				int w = 0, h = 0;
				std::tie(w, h) = child.win->min_size();
				if(orientation() == Orientation::HORIZONTAL) {
					w += (child.h_expand ? extra_w : 0);
					h = (child.v_expand ? new_height : h);
					child.win->move(l, t, w, h);
					l += w;
					if(ci < m_spacers.size()) {
						m_spacers.at(ci)->move(l, t, m_spacing, h);
					}
					l += m_spacing;
				} else {
					w = (child.h_expand ? new_width : w);
					h += (child.v_expand ? extra_h : 0);
					child.win->move(l, t, w, h);
					t += h;
					if(ci < m_spacers.size()) {
						m_spacers.at(ci)->move(l, t, w, m_spacing);
					}
					t += m_spacing;
				}
				++ci;
			}
		}
	}
}