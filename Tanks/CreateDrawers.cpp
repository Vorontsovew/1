#include "pch.h"
#include "CreateDrawers.h"
#include <variant>
#include "Bitmaps.h"
#include "BitmapDrawer.h"
#include <memory>


std::unique_ptr<IDrawer> Create(const GeneralMapObjectData& data)
{
	auto icon = std::visit(
		[&](auto&& val) -> Bitmaps
		{
			if constexpr (std::is_same_v<std::decay_t<decltype(val)>, TankType>)
			{
				switch (val)
				{
				case TankType::Player: return Bitmaps::PlayerTank;
				case TankType::Enemy: return Bitmaps::EnemyTank;
				}
			}
			else if constexpr (std::is_same_v<std::decay_t<decltype(val)>, BarrierType>)
			{
				switch (val)
				{
				case BarrierType::Wall: return Bitmaps::Wall;
				case BarrierType::Forest: return Bitmaps::Forest;
				case BarrierType::Ice: return Bitmaps::Ice;
				}
			}
			else
			{
				return Bitmaps::Bullet1;
			}
		}
	, data.type);

	return std::make_unique<GeneralDrawer>(icon, data.mo_data);
}