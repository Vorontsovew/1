#pragma once
#include <memory>
#include "MapObjectData.h"

class IdGenerator;
struct MapObjectVisitor;

class Bullet;
class Tank;
class Burrier;

// базовый класс объектов карты
class MapObject
{
public:
	virtual ~MapObject() = default;
	const MapObjectData& GetMapObjectData()  const;
	size_t GetId() const { return m_id; }

	virtual void Accept(MapObjectVisitor&) = 0;
protected:
	void SetMapObjectData(MapObjectData);
	MapObject(const MapObjectData&, IdGenerator&);

private:
	MapObjectData m_mapObjectData;
	std::size_t m_id;
};

//либо паттерн визитёр, либо static_cast либо dinamic_cast

/*void SetMapObjectData(MapObject& mapObject, MapObjectData data)
{
	if (mapObject.GetType() == Tank)
	{
		Tank& tank = static_cast<Tank&>( mapObject);
		tank.SetData(data);
	}

	if (auto* tank = dynamic_cast<Tank*>(&mapObject))
	  tank->SetData(data);
	else if (auto* barrier = dynamic_cast<Barrier*>(&mapObject))
	  barrier.Destroy();
}
*/
