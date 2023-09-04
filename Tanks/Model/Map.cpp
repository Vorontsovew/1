#include "pch.h"
#include "Map.h"

#include <array>
#include <tuple>
#include <span>

#include "Burrier.h"
#include "Tank.h"
#include "Bullet.h"
#include "IdGenerator.h"
#include <algorithm>
#include <ranges>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


class Map : public IMap
{
  //Map(Size a, std::vector<std::unique_ptr<Burrier>>& b, std::vector<std::unique_ptr<Tank>>& c, std::vector<std::unique_ptr<Tank>>& d,
  //  std::vector<std::unique_ptr<Bullet>>& e) : m_size(a),m_burriers(b), m_EnemyTanks(c), m_PlayerTanks(d), m_bullets(e) 
  //{
  //};

public:
  static MapPtr Create(Size szMap, std::span<GeneralMapObjectData> data);

  Map(Size sz) : m_size(sz) {}
  ~Map();
  virtual const MapObject* FindObject(Point) const override;
  std::vector<std::pair<size_t, GeneralMapObjectData>> GetGeneralData() const override;
  virtual void SetGeneralData(const std::vector<std::pair< size_t, GeneralMapObjectData>>& objects) override;
  virtual void UpdateObject(size_t id, MapObjectData data) override;
  
  virtual void AddBullet(MapObjectData data, BulletType type) override
  {
    m_bullets.push_back(std::make_unique<Bullet>(data,type,generator));
    OnModify();
  }
private:
  virtual void AddListener(FnMapModify fn) override
  {
    m_listeners.push_back(fn);
  
  fn->OnMapModify(GetGeneralData());
  }
  void OnModify()
  {
    auto datas = GetGeneralData();
    for (auto& fn : m_listeners)
      fn->OnMapModify(datas);

  }
  virtual std::optional<MapObjectData> GetMapObjectData(size_t id) override;
  MapObject* FindObject(size_t id);

  virtual Size GetSize() override { return m_size; }
  //virtual const Bullet& GetBullets() {}

  void AddMapObject(const GeneralMapObjectData& data)
  {
    std::visit(
          [&](auto&& val) 
        {
        if constexpr (std::is_same_v<std::decay_t<decltype(val)>, TankType>)
          {
          if (val == TankType::Player)
            m_PlayerTanks.emplace_back(std::make_unique<Tank>(data.mo_data, val, generator));
          else
            m_EnemyTanks.emplace_back(std::make_unique<Tank>(data.mo_data, val, generator));
        }
        else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, BarrierType>)
          {
            m_burriers.emplace_back(std::make_unique<Burrier>(data.mo_data, val, generator));
          }
        else
          {
            static_assert (std::is_same_v< std::decay_t<decltype(val)>, BulletType>);
            //m_bullets.emplace_back(std::make_unique<Bullet>(data.mo_data));
          }
        }
        , data.type);

  }
    
  virtual void DeleteObject(size_t id) override;
  virtual void AddTank(MapObjectData data, TankType type) override;

  virtual void ForEachObject(FnObject) const override;
private:
  const Size m_size;
  std::vector<FnMapModify> m_listeners;
  IdGenerator generator;
  std::vector<std::unique_ptr<Burrier>> m_burriers;
  std::vector<std::unique_ptr<Tank>> m_EnemyTanks;
  std::vector<std::unique_ptr<Tank>> m_PlayerTanks;
  std::vector<std::unique_ptr<Bullet>> m_bullets;
};

void Map::ForEachObject(FnObject fn) const
{
  for (auto& obj : m_burriers)
    fn({ obj->GetMapObjectData(), obj->GetType() });
  for (auto& obj : m_EnemyTanks)
    fn({ obj->GetMapObjectData(), obj->GetType() });
  for (auto& obj : m_PlayerTanks)
    fn({ obj->GetMapObjectData(), obj->GetType() });
  for (auto& obj : m_bullets)
    fn({ obj->GetMapObjectData(), obj->GetType() });


}
void Map::DeleteObject(size_t id)
{
  auto func = [id](auto&& val) {return val->GetId() == id;};
  std::erase_if(m_burriers, func);
  std::erase_if(m_EnemyTanks, func);
  std::erase_if(m_PlayerTanks, func);
  std::erase_if(m_bullets, func);
  OnModify();
}
void Map::AddTank(MapObjectData data, TankType type)
{
  switch (type)
  {
  case TankType::Enemy:
    m_EnemyTanks.push_back(std::make_unique<Tank>(data, type, generator));
    break;
  case TankType::Player:
    m_PlayerTanks.push_back(std::make_unique<Tank>(data, type, generator));
    break;
  }
  OnModify();
}
class UpdateObjectVisitor : public MapObjectVisitor
{
  MapObjectData m_data;
public:
  UpdateObjectVisitor( MapObjectData data):
    MapObjectVisitor(),
    m_data(data)
  {  }

  virtual void Visit(Bullet& bullet) override
  {
    bullet.SetData(m_data);
  }
  virtual void Visit(Tank& tank) override
  {
    tank.SetData(m_data);
  }
  virtual void Visit(Burrier&) override
  {
    ASSERT(0);
  }
};
//обновление данных объекта карты
void Map::UpdateObject(size_t id, MapObjectData data)
{
  UpdateObjectVisitor visitor(data);
  if (auto* obj = FindObject(id))
    obj->Accept(visitor);
  OnModify();
/*
  switch (obj.GetType())
  {
  case Tank:
    auto tank = m_PlayerTanks.find(obj.getId());
    tank.SetMapObjectData(data);
  }
*/
  
}

#define FIND_OBEJECT(objs) for (auto&& a : objs) \
if (a->GetId() == id) \
return  a.get();

template<class Objects, class Value>
MapObject* FindObject(const Objects& objs, Value val)
{
  for (auto&& a : objs)
  {
    if constexpr (std::is_same_v<Value, size_t>)
    {
      if (a->GetId() == val)
        return  a.get();
    }
    else if constexpr (std::is_same_v<Value, Point>)
    {
      if (a->GetMapObjectData().m_position == val)
        return  a.get();
    }
    else static_assert(std::is_same_v<Value, size_t> || std::is_same_v<Value, Point>);
  }
  return nullptr;
}

MapObject* Map::FindObject(size_t id)
{
  auto* res = ::FindObject(m_bullets, id);
  res = res? res: ::FindObject(m_burriers, id);
  if (!res) res = ::FindObject(m_EnemyTanks, id);
  if (!res) res = ::FindObject(m_PlayerTanks, id);
  return res;

  /*  FIND_OBEJECT(m_bullets);
  FIND_OBEJECT(m_burriers);
  FIND_OBEJECT(m_EnemyTanks);
  FIND_OBEJECT(m_PlayerTanks);

  ASSERT(0);
  return nullptr;
*/
  }

const MapObject* Map::FindObject(Point pt) const
{
  auto* res = ::FindObject(m_bullets, pt);
  res = res ? res : ::FindObject(m_burriers, pt);
  if (!res) res = ::FindObject(m_EnemyTanks, pt);
  if (!res) res = ::FindObject(m_PlayerTanks, pt);
  return res;
}


std::optional<MapObjectData> Map::GetMapObjectData(size_t id)
{
  std::optional<MapObjectData> result;
  if (auto* obj = FindObject(id))
    result = obj->GetMapObjectData();
  return result; // obj ? obj->GetMapObjectData() : MapObjectData{};

}
void Map::SetGeneralData(const std::vector<std::pair< size_t, GeneralMapObjectData>>& objects)
{
  m_burriers.clear();
  m_EnemyTanks.clear();
  m_PlayerTanks.clear();
  m_bullets.clear();
  for (auto& [id,data] : objects)
    AddMapObject(data);
}
std::vector<std::pair< size_t, GeneralMapObjectData>> Map::GetGeneralData() const
{
  std::vector<std::pair< size_t, GeneralMapObjectData>> data_get;
  
  
  for (auto&& a : m_burriers)
  {
    data_get.emplace_back(a->GetId(), GeneralMapObjectData{ a->GetMapObjectData(), a->GetType() });
  }
  for (auto&& a : m_bullets)
  {
    data_get.emplace_back(a->GetId(), GeneralMapObjectData{ a->GetMapObjectData(), a->GetType() });
  }
  for (auto&& a : m_EnemyTanks)
  {
    data_get.emplace_back(a->GetId(), GeneralMapObjectData{ a->GetMapObjectData(), a->GetType() });
  }
  for (auto&& a : m_PlayerTanks)
  {
    data_get.emplace_back(a->GetId(), GeneralMapObjectData{ a->GetMapObjectData(), a->GetType() });
  }
  return data_get;
}

MapPtr Map::Create(Size szMap, std::span<GeneralMapObjectData> objects)
{
  constexpr Size minSize = { 4, 4 };
  if (szMap.cx <= minSize.cx || szMap.cy < minSize.cy)
    return {};
  auto map = std::make_unique<Map>(szMap);
  for (auto& data : objects)
    map->AddMapObject(data);

  return map;

}

Map::~Map() = default;

MapPtr GenerateMap(int level)
{
  Size size_map{ 40,80 };
  std::array<std::array<bool, 20>, 20> checkPosition{};

  std::vector<GeneralMapObjectData> objects
  { 
  //  GeneralMapObjectData{{{1,1}, Orientation::Top}, TankType::Player},
  //  GeneralMapObjectData{{{3,10}, Orientation::Top}, TankType::Player},
  //  GeneralMapObjectData{{{5,15}, Orientation::Bottom}, TankType::Enemy},
  //  GeneralMapObjectData{{{7,10}, Orientation::Left}, TankType::Enemy},
  //  GeneralMapObjectData{{{2,12}, Orientation::Right}, TankType::Enemy},
  //  GeneralMapObjectData{{{4,18}, Orientation::Top}, BarrierType::Wall},
  //  GeneralMapObjectData{{{1,7}, Orientation::Left}, BarrierType::Forest},
  //  GeneralMapObjectData{{{15,5}, Orientation::Right}, BarrierType::Ice},
  //  GeneralMapObjectData{{{12,10}, Orientation::Left}, BarrierType::Wall},
  //  GeneralMapObjectData{{{17,3}, Orientation::Left}, BarrierType::Forest},
  //  GeneralMapObjectData{{{12,6}, Orientation::Bottom}, BarrierType::Ice}
  }; 
  objects.push_back({ { {7,19}, Orientation::Top}, TankType::Player });
  objects.push_back({ { {13,19}, Orientation::Top}, TankType::Player });
  for (auto i = 0; i < level; i++)
    objects.push_back({{{i*int(20/level),0 }, Orientation::Bottom}, TankType::Enemy });
  for (auto i = 0; i < 10*level; i++)
    objects.push_back({ {{rand()%20,rand() % 20}, static_cast<Orientation>(rand() % int(Orientation::Count))}, static_cast<BarrierType>(rand() % int(BarrierType::Count))});
  std::erase_if(objects, [&checkPosition](GeneralMapObjectData obj)
    {
      auto& val = checkPosition[obj.mo_data.m_position.x][obj.mo_data.m_position.y];
  if (val == false)
  {
    val = true;
    return false;
  }
  else
    return true;
    }
  
  );
  //objects.push_back(GeneralMapObjectData{ {{1,1}, Orientation::Left}, TankType::Enemy });

  return Map::Create(size_map, objects);


  //  std::unique_ptr<Tank> tank_enemy_1 = std::make_unique<Tank>();
  //  std::unique_ptr<Tank> tank_enemy_2 = std::make_unique<Tank>();
  //  std::unique_ptr<Tank> tank_player_1 = std::make_unique<Tank>();
  //  std::unique_ptr<Tank> tank_player_2 = std::make_unique<Tank>();
  //  std::unique_ptr<Burrier> burrier_1 = std::make_unique<Burrier>();
  //  std::unique_ptr<Burrier> burrier_2 = std::make_unique<Burrier>();
  //  std::unique_ptr<Bullet> bullet_1 = std::make_unique<Bullet>();
  //  std::unique_ptr<Bullet> bullet_2 = std::make_unique<Bullet>();
  //
  //  std::vector<std::unique_ptr<Burrier>> burriers;
  //  burriers.push_back(burrier_1);
  //  burriers.push_back(burrier_2);
  //
  //  std::vector<std::unique_ptr<Tank>> EnemyTanks;
  //  EnemyTanks.push_back(tank_enemy_1);
  //  EnemyTanks.push_back(tank_enemy_2);
  //
  //  std::vector<std::unique_ptr<Tank>> PlayerTanks;
  //  PlayerTanks.push_back(tank_player_1);
  //  PlayerTanks.push_back(tank_player_2);
  //
  //return {};//  Map::Create(size_map, std::move(burriers), std::move(EnemyTanks), std::move(PlayerTanks));
}