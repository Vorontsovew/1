#pragma once

#include "MapObjectData.h"
#include <memory>
#include "IDrawer.h"

// создает отрисовщик для объекта карты

std::unique_ptr<IDrawer> Create(const GeneralMapObjectData& data);

