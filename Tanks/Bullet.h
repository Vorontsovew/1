#pragma once
#include "MapObject.h"
#include "MapObjectVisitor.h"

class Bullet : public MapObject
{
public:
	using MapObject::MapObject;
	Bullet(MapObjectData& data, BulletType, IdGenerator& generator);
//	Bullet(Bullet&& other) = default;
//	Bullet Bullet(Bullet&& other) = default;
//	Orientation BulletOrient();
	BulletType GetType();
	virtual void Accept(MapObjectVisitor&) override;
	void SetData(MapObjectData data);
private:
	BulletType m_type;
};

