#include "pch.h"
#include "Tank.h"
#include "MapObjectData.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Tank::Tank(const MapObjectData& data, TankType type, IdGenerator& generator) : MapObject(data, generator), m_type(type) 
{
}

TankType Tank::GetType()
{
    return m_type;
}

void Tank::Accept(MapObjectVisitor& visitor)
{
    visitor.Visit(*this);
}

void Tank::SetData(MapObjectData data)
{
    SetMapObjectData(data);
}