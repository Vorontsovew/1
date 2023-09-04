#pragma once
class Bullet;
class Tank;
class Burrier;

struct MapObjectVisitor
{
  virtual ~MapObjectVisitor() = default;

  virtual void Visit(Bullet&) = 0;
  virtual void Visit(Tank&) = 0;
  virtual void Visit(Burrier&) = 0;
};