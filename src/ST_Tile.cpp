#include <ST_Tile.h>
#include <ST_WinError.h>

namespace shadertest {
	
	Tile::Tile(COLORREF color, const std::wstring& name, App& app, Window *parent)
	: Window(name, app, WS_CHILD, parent), m_brush_settings() {
		m_brush_settings.lbStyle = BS_SOLID;
		m_brush_settings.lbColor = color;
		m_brush = CreateBrushIndirect(&m_brush_settings);
		if(m_brush == nullptr) {
			throw WinError(L"Tile: Failed to create solid color brush");
		}
	}
	
	Tile::~Tile() {
		if(m_brush != nullptr) {
			DeleteObject(m_brush);
		}
	}
	
	COLORREF Tile::color() const {
		return m_brush_settings.lbColor;
	}
	
	void Tile::on_paint(HDC dc, PAINTSTRUCT& ps) {
		FillRect(dc, &ps.rcPaint, m_brush);
	}

}