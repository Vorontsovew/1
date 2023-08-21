#pragma once
#include "MapModifier.h"

enum class Events
{
	Down,
	Up,
	Left,
	Right,
	Fire,
	Stop,
	Count
};
//обрабатывет событие от окна Events(транслирует событие из окна в действие контроллера
class Notifier
{
private:
	Events m_lastEvent;
	IMapModifier& m_controller;
	size_t m_id;
	
public:
	Notifier(IMapModifier&  ctrl, size_t id);
	
	//запоминает последнее событие
	void Send(Events ev);
	
	void Execute(); 
	
};

class NotifierBullet
{
private:
//	Events m_lastEvent;
	IMapModifier& m_controller;
	std::vector<size_t> m_ids;


public:
	NotifierBullet(IMapModifier& ctrl, const std::vector<size_t>& ids);

	void Execute();

};

class NotifierCreateTank
{
private:
	IMapModifier& m_controller;
	//size_t m_id;

public:
	NotifierCreateTank(IMapModifier& ctrl);
	void Execute();

};