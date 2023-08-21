#pragma once

#include <memory>
struct IMap;
using MapPtr = std::unique_ptr<IMap>;
class IMapModifier;

using MapModifierPtr = std::unique_ptr<IMapModifier>;

MapPtr GenerateMap(int level);
MapModifierPtr CreateMapModifier(IMap& map);
