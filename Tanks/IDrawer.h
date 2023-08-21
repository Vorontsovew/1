#pragma once

class IDrawContext;
//{
//	CDC* dc;
//	void Line(Point pt1, Point pt2)
//	{
//		CPen pen(0, 0, 0);
//		dc->SelectObject(pen);
//		dc->MoveTo(pt1.x, pt2.y);
//		dc->LineTo(pt1.x, pt2.y);
//		dc->SelectObject(NULL);
//	}
//}

//интерфейс класса, который выводится в окно, или в контекст
struct IDrawer // интерфейс 
{
	virtual void Draw(IDrawContext& dc) const = 0; //
};

