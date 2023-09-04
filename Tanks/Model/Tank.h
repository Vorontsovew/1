#pragma once
#include "MapObject.h"
#include "MapObjectVisitor.h"

class Tank : public MapObject
{
public:
  Tank(MapObjectData& data, TankType, IdGenerator& generator);
  TankType GetType();
  virtual void Accept(MapObjectVisitor&) override;
  void SetData(MapObjectData data);
  
private:
  TankType m_type;
};


