#include "pch.h"
#include "MapObject.h"

#include "IdGenerator.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void MapObject::SetMapObjectData(MapObjectData data)
{
	m_mapObjectData = data;
}

MapObject::MapObject(const MapObjectData& data, IdGenerator& generator):
	m_mapObjectData(data),
	m_id(generator.generate())
{}

const MapObjectData& MapObject::GetMapObjectData() const
{
	return m_mapObjectData;
}

/*void MapObject::SetMapObjectData(MapObjectData& data)
{
	m_mapObjectData=data;
}*/
