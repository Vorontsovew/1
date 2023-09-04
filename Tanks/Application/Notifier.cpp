#include "pch.h"
#include "Notifier.h"

Notifier::Notifier(IMapModifier& ctrl, size_t id) :
  m_controller(ctrl),
  m_id(id),
  m_lastEvent(Events::Stop)
{
}

void Notifier::Send(Events ev) 
{ 
  m_lastEvent = ev; 
}

void Notifier::Execute()
{
  switch (m_lastEvent)
  {
  case Events::Left: m_controller.MoveObject(m_id, Orientation::Left);
    break;
  case Events::Right: m_controller.MoveObject(m_id, Orientation::Right);
    break;
  case Events::Up:m_controller.MoveObject(m_id, Orientation::Top);
    break;
  case Events::Down: m_controller.MoveObject(m_id, Orientation::Bottom);
    break;
  case Events::Fire: m_controller.Fire(m_id);
    break;
  };
}

NotifierBullet::NotifierBullet(IMapModifier& ctrl, const std::vector<size_t>& ids) : m_controller(ctrl), m_ids(ids) {};

void NotifierBullet::Execute()
{
  m_controller.MoveBullets(m_ids);
}
 
NotifierCreateTank::NotifierCreateTank(IMapModifier& ctrl) :
  m_controller(ctrl)
{
}

void NotifierCreateTank::Execute()
{
  m_controller.CreateTank();
}
