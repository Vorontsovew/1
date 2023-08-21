#pragma once
#include "MapObject.h"
struct IMap;

enum class GameStatusType
{
	Continue,
	PlayerWin,
	PlayerLose,
	Count
};

//контроллер карты
class IMapModifier
{
//	virtual void SetObjectData(const MapObject& obj, MapObjectData data)=0;
public:
	virtual void MoveObject(size_t id, Orientation) = 0;
	virtual void MoveBullets(const std::vector<size_t>& ids) = 0;
	virtual void Fire(size_t) = 0;
	virtual void CreateTank() = 0;
	virtual void Save() = 0;
	virtual void Load() = 0;
	virtual GameStatusType GameStatus() = 0;
//	virtual void KillAll();
	virtual ~IMapModifier()=default;
};
