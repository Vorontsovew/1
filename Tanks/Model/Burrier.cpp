#include "pch.h"
#include "Burrier.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Burrier::Burrier(MapObjectData& data, BarrierType type, IdGenerator& generator) 
  : MapObject(data, generator)
  , m_type(type) 
{

}


BarrierType Burrier::GetType()
{
  return m_type;
}

void Burrier::Accept(MapObjectVisitor& visitor)
{
  visitor.Visit(*this);
}

//void Burrier::SetData(MapObjectData& data)
//{
  //m_mapObjectData = data;
//}       