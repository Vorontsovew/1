#include "pch.h"
#include "MapModifier.h"
#include "Map.h"
#include <fstream>
#include <iostream>
#include <variant>
using MapModifierPtr = std::unique_ptr<IMapModifier>;

class MapModifier : public IMapModifier
{

public:
	MapModifier(IMap& map) : m_map(map), m_extraTanks(10) {}
	//перемещение объекта
	virtual void MoveObject(size_t id, Orientation) override;
	virtual void MoveBullets(const std::vector<size_t>& ids) override;
	virtual void Fire(size_t) override;
	virtual void CreateTank() override;
	virtual void Save() override;
	virtual void Load() override;
	// todo : реализовть и поиспользовать в CTanksView::OnMapModify
	virtual GameStatusType GameStatus() override;
private:
	IMap& m_map;
	size_t m_extraTanks; //< кол-во танков которые появятся после смерти соратников
	std::vector<size_t> m_stamps; //< время для появления танков 

};

MapModifierPtr CreateMapModifier(IMap& map)
{
	auto mapModif = std::make_unique<MapModifier>(map);
	return mapModif;
}

//std::vector<std::pair<size_t, GeneralMapObjectData>> GetGeneralData() const;

void MapModifier::Save()
{
	std::ofstream out("output.txt");

	auto Data = m_map.GetGeneralData();
	out <<"Count: " << Data.size();
	for (auto& [id, mapObjData] : Data)
	{
		out << "ID: " << id << std::endl;
		out << "x = " << mapObjData.mo_data.m_position.x << std::endl;
		out << "y = " << mapObjData.mo_data.m_position.y << std::endl;
		out << "Orientation: " << static_cast<int>(mapObjData.mo_data.m_orientation) << std::endl;
		out << "ObjectType: " << mapObjData.type.index();
		std::visit([&out](auto& type) {out << static_cast<int>(type) << "SubType: " << std::endl; }, mapObjData.type);
		/*if (auto* type = std::get_if<TankType>(&mapObjData.type))
		{
			out << "TankType" << std::endl;
			out << static_cast<int>(*type) << std::endl;
		}
		else if (auto* type = std::get_if<BulletType>(&mapObjData.type))
		{
			out << "BulletType" << std::endl;
			out << static_cast<int>(*type) << std::endl;
		}
		else if (auto* type = std::get_if<BarrierType>(&mapObjData.type))
		{
			out << "BarrierType" << std::endl;
			out << static_cast<int>(*type) << std::endl;
		}*/
	
	}
	out.close();

}
void MapModifier::Load()
{
	std::ifstream in("output.txt");
	in >> hile(in)
	in.close();
}

Point Move(Point pt, Orientation orientation)
{
	switch (orientation)
	{
	case Orientation::Bottom:
		if(pt.y<20)
		++pt.y; break;
	case Orientation::Top:
		if (pt.y >0)
		--pt.y; break; 
	case Orientation::Left:
		if (pt.x > 0)
		--pt.x; break; 
	case Orientation::Right:
		if (pt.x < 20)
		++pt.x; break;
	}
	return pt;
}


void MapModifier::MoveObject(size_t id, Orientation orientation)
{
	auto data = m_map.GetMapObjectData(id);
	
	if (data)
	{
		data->m_orientation = orientation;
		data->m_position = Move(data->m_position, orientation);
		auto* obj = m_map.FindObject(data->m_position);

		if (obj && dynamic_cast<const Bullet*>(obj))
		{
			m_map.DeleteObject(id);
			m_map.DeleteObject(obj->GetId());
		}
		else if (!obj)
			m_map.UpdateObject(id, *data);
	}
}

void MapModifier::MoveBullets(const std::vector<size_t>& ids)
{
	for (auto id : ids)
	{
		auto data = m_map.GetMapObjectData(id);
		if (data)
		{
			data->m_position = Move(data->m_position, data->m_orientation);
			auto* obj = m_map.FindObject(data->m_position);
			if (obj)
			{
				m_map.DeleteObject(id);
				m_map.DeleteObject(obj->GetId());
				//if constexpr (std::is_same_v<std::decay_t<decltype(*obj)>, Tank>)
				
			//	if (auto* tank = dynamic_cast<const Tank*>(obj))
			//	{
			//		//AddObject(m_map, { {10,0}, Orientation::Bottom }, TankEnemy);
			//		m_map.CreateTank();
			//		//if (val == TankType::Player)
			//	}
			}
			else m_map.UpdateObject(id, *data);
		}
	}
}

void MapModifier::Fire(size_t idTank)
{
	auto data = m_map.GetMapObjectData(idTank);
	if (data)
	{
		data->m_position = Move(data->m_position, data->m_orientation);
		m_map.AddBullet(*data, BulletType::BulletLight);
	}
}

void MapModifier::CreateTank()
{
	size_t tankCount = 0;
	FnObject fnCalcTank = [&tankCount](auto data)
	{
		if (auto * type = std::get_if<TankType> (&data.type); type && *type == TankType::Enemy)
			++tankCount;
	};
	m_map.ForEachObject(fnCalcTank);

	
	if (tankCount + m_stamps.size() < 5)
	{
		m_stamps.resize(5 - tankCount, 0);
	}

	constexpr size_t WaitSteps = 5;
	for (auto& s : m_stamps)
	{
		if (++s >= WaitSteps && m_extraTanks != 0)
		{
			--m_extraTanks;
			m_map.AddTank({ {10,0}, Orientation::Bottom }, TankType::Enemy);
		}
	}

	std::erase_if(m_stamps, [](auto& stamp) {return stamp > WaitSteps; });
	

}

GameStatusType MapModifier::GameStatus()
{
	return GameStatusType::Continue;
}

