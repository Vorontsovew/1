#pragma once

#include "MapObjectData.h"
#include <memory>
#include "IDrawer.h"

// ������� ���������� ��� ������� �����

std::unique_ptr<IDrawer> Create(const GeneralMapObjectData& data);

