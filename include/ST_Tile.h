#ifndef TILE_H
#define TILE_H

#include <ST_Window.h>

namespace shadertest {
	
	class Tile : public Window {
	public:
		Tile(COLORREF color, const std::wstring& name, App& app, Window *parent = nullptr);
		
		~Tile() override;
	
		COLORREF color() const;
		
		void on_paint(HDC hdc, PAINTSTRUCT& ps) override;
	private:
		LOGBRUSH	m_brush_settings;
		HBRUSH		m_brush;
	};

}

#endif