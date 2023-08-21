#pragma once
#include<memory>
#include<vector>
#include "MapObject.h"
#include"Burrier.h"
#include"Bullet.h"
#include<optional>
class Burrier;
class Tank;
class Bullet;
using MapPtr = std::unique_ptr<struct IMap>;
using FnObject = std::function<void(GeneralMapObjectData)>;
// интерфейс диспетчера, позволяет добавлять слушателя(FnMapModify) и будет уведомлять слушателей обо всех важных изменениях себя
// у нас реализация для карты, поэтому будем сообщать об изменениях данных карты
struct IDispetcher
{
	virtual void AddListener(FnMapModify) = 0;
};

//интерфейс карты/игрового поля
struct IMap: IDispetcher
{
	virtual void UpdateObject(size_t obj, MapObjectData data) = 0;
	virtual std::optional<MapObjectData> GetMapObjectData(size_t id) = 0;
	virtual void AddBullet(MapObjectData data, BulletType type) = 0;
	virtual void AddTank(MapObjectData data, TankType type) = 0;
	virtual void DeleteObject(size_t id)=0;
	virtual Size GetSize() = 0;
	virtual const MapObject* FindObject(Point) const = 0;
	virtual void ForEachObject(FnObject) const = 0;
	virtual std::vector<std::pair<size_t, GeneralMapObjectData>> GetGeneralData() const = 0;
	virtual void SetGeneralData(const std::vector<std::pair< size_t, GeneralMapObjectData>>& objects) = 0;
};



