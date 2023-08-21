#include "pch.h"
#include "Bullet.h"
#include "MapObjectData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Bullet::Bullet(MapObjectData& data, BulletType type, IdGenerator& generator) : MapObject(data, generator), m_type(type)
{
}

BulletType Bullet::GetType()
{
    return m_type;
}


void Bullet::Accept(MapObjectVisitor& visitor)
{
    visitor.Visit(*this);
}

void Bullet::SetData(MapObjectData data)
{
    SetMapObjectData(data);
    
};
