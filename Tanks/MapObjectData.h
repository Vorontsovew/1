#pragma once
#include<string>
#include <functional>
#include <span>
#include <variant>

enum class Orientation
{
  Left, Right, Top, Bottom, Count
};

enum class ObjectTypes
{
  Tank, Barrier, Bullet, Count
};

enum class TankType
{
  Player, Enemy, Count
};


enum class BarrierType
{
  Wall, Forest, Ice, Count
};

enum class BulletType
{
  BulletLight, Count
};

struct Point
{
  int x;
  int y;
  auto operator<=>(const Point&) const = default;

};

struct Size
{
  int cx;
  int cy;
};

//данные объектов карты(позиция, оиент
struct MapObjectData
{
  Point m_position;
  Orientation m_orientation;
};

//полные данные объекта (описывают объект полностью
struct GeneralMapObjectData
{
  MapObjectData mo_data;
  //ObjectTypes type;
  std::variant<TankType, BarrierType, BulletType> type;
};

//слушатель изменений карты - функциональный объект
//using FnMapModify = std::function<void (std::span<GeneralMapObjectData const>)>;

struct IListener
{
  virtual void OnMapModify(std::span<std::pair<size_t, GeneralMapObjectData> const> objects) = 0;
};

using FnMapModify = IListener*;