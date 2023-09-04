#include "pch.h"
#include "DrawContext.h"
#include "WinUser.h"
#include "resource.h"
#include <memory>

//uint32_t  
UINT GetBitmap(Bitmaps icon)
{
  switch (icon)
  {
  case Bitmaps::PlayerTank:
    return IDB_PLAYER_TANK;
  case Bitmaps::EnemyTank:
    return IDB_ENEMY_TANK;
  case Bitmaps::Wall:
    return IDB_WALL;
  case Bitmaps::Forest:
    return IDB_FOREST;
  case Bitmaps::Ice:
    return IDB_ICE;
  case Bitmaps::Bullet1:
    return IDB_BULLET;
  }
  return 0;
}

class DrawContext : public IDrawContext
{
  CDC& dc; //CDC это mfc объект, позвол€ющий выводить данные в окно
  int scale;
public:
  DrawContext(CDC& dc) : dc(dc), scale(32) {}
  virtual void DrawBitmap(Bitmaps, Point, Orientation) override;
  virtual void DrawText(const std::wstring& text, Point ptLT, Point ptRB) override;
  // вывод иконки на экран(окно)

};

void DrawContext::DrawBitmap(Bitmaps icon, Point pt, Orientation o)
{
  CBitmap bitmap;
  CBitmap bitmap_;
  bitmap.LoadBitmap(GetBitmap(icon));

  CDC memDC;
  memDC.CreateCompatibleDC(&dc);
  CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
  BITMAP bmp;
  bitmap.GetBitmap(&bmp);
  //dc.BitBlt(pt.x * scale, pt.y * scale, bmp.bmWidth, bmp.bmHeight, &memDC, 0, 0, SRCCOPY);

  POINT base = { pt.x * scale, pt.y * scale };


  POINT PTS[3] = { base, base, base };
  switch(o)
  {
  case Orientation::Left:
    PTS[0].x += scale;
    PTS[2].x += scale;
    PTS[2].y += scale;
    break;
  case Orientation::Right:
    PTS[1].x += scale;
    PTS[2].y += scale;
    break;
  case Orientation::Bottom:
    PTS[0].x += scale;
    PTS[1].x += scale;
    PTS[1].y += scale;
    //PTS[2].y += scale;
    break;
  case Orientation::Top:
    PTS[0].y += scale;
    PTS[2].x += scale;
    PTS[2].y += scale;
    break;

  };
  dc.PlgBlt(PTS, &memDC, 0, 0, bmp.bmWidth, bmp.bmHeight, bitmap_, 0, 0);
  memDC.SelectObject(pOldBitmap);

}

void DrawContext::DrawText(const std::wstring& text, Point ptLT, Point ptRB)
{
  RECT rc = { ptLT.x * scale, ptLT.y * scale, ptRB.x * scale, ptRB.y * scale };
  dc.DrawText(text.data(), text.size(), &rc, 0);
}

std::unique_ptr<IDrawContext> CreateCDCContext(CDC& dc)
{
  return std::make_unique<DrawContext>(dc);
}
