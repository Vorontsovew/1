#pragma once
#include "Bitmaps.h"
//#include "BitmapDrawer.h"
#include "MapObjectData.h"
#include<memory>




//��������� ��� ����������� ������, � ������� ������� ����� �������� ������ � ���������� ����� ������ - ���� MFC

class IDrawContext
{
public:
  virtual void DrawBitmap(Bitmaps, Point, Orientation)=0;
  virtual void DrawText(const std::wstring& text, Point ptLT, Point ptRB)  = 0 ;
};

std::unique_ptr<IDrawContext> CreateCDCContext(CDC&);



