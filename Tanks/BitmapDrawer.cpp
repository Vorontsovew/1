#include "pch.h"
#include "BitmapDrawer.h"
#include "DrawContext.h"



void GeneralDrawer::Draw(IDrawContext& dc) const
{
	dc.DrawBitmap(m_icon, m_data.m_position, m_data.m_orientation);
}


namespace
{
	class TextDrawer: public IDrawer
	{
		std::wstring m_text;
		Point m_ptLT;
		Point m_ptRB;
	public:
		TextDrawer(const std::wstring& text, Point ptLT, Point ptRB) :m_text(text), m_ptLT(ptLT), m_ptRB(ptRB)
		{};

		virtual void Draw(IDrawContext&) const override;
	};

	void TextDrawer::Draw(IDrawContext& context) const
	{
		context.DrawText(m_text, m_ptLT, m_ptRB);
	}
}

std::unique_ptr<IDrawer> CreateTextDrawer(const std::wstring& text, Point ptLT, Point ptRB)
{
	return std::make_unique<TextDrawer>(text, ptLT, ptRB);
}