#pragma once
#include "IDrawer.h"
#include "MapObjectData.h"
#include "Bitmaps.h"

#include<memory>

//реализация интерфейса idrawer для отрисовки объектов карты с помощью иконки
class GeneralDrawer : public IDrawer
{
public:
	GeneralDrawer(Bitmaps icon, MapObjectData data) :m_icon(icon), m_data(data)
	{};

	virtual void Draw(IDrawContext&) const;
private:
	MapObjectData m_data;
	Bitmaps m_icon;

	
};


std::unique_ptr<IDrawer> CreateTextDrawer(const std::wstring&, Point ptLT, Point ptRB);
