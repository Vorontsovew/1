
// TanksView.cpp: реализация класса CTanksView
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Tanks.h"
#endif

#include <variant>
#include <cstdlib>
#include "DrawContext.h"
#include "IDrawer.h"
#include "TanksDoc.h"
#include "TanksView.h"
#include "Notifier.h"

#include "BitmapDrawer.h"
#include "CreateDrawers.h"

#include<ranges>
#include<array>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTanksView

IMPLEMENT_DYNCREATE(CTanksView, CView)

BEGIN_MESSAGE_MAP(CTanksView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTanksView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// Создание или уничтожение CTanksView

CTanksView::CTanksView() noexcept
{
	// TODO: добавьте код создания

}

CTanksView::~CTanksView()
{
}

BOOL CTanksView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

//	std::array objects{ GeneralMapObjectData{{{35,20}, Orientation::Top}, TankType::Player},
//	GeneralMapObjectData{{{5,25}, Orientation::Bottom}, TankType::Enemy},
//	GeneralMapObjectData{{{5,55}, Orientation::Left}, TankType::Enemy},
//	GeneralMapObjectData{{{5,40}, Orientation::Right}, TankType::Enemy},
//	GeneralMapObjectData{{{25,20}, Orientation::Top}, BarrierType::Wall},
//	GeneralMapObjectData{{{30,15}, Orientation::Left}, BarrierType::Forest},
//	GeneralMapObjectData{{{30,25}, Orientation::Right}, BarrierType::Ice},
//	GeneralMapObjectData{{{7,65}, Orientation::Left}, BarrierType::Wall},
//	GeneralMapObjectData{{{16,20}, Orientation::Left}, BarrierType::Forest},
//	GeneralMapObjectData{{{22,60}, Orientation::Bottom}, BarrierType::Ice}
//	};
//
//	for (auto& data : objects)
//		if (auto&& drawer = ::Create(data))
//			m_drawers.emplace_back(std::move(drawer));

	return CView::PreCreateWindow(cs);
}

void ClearNotifier(std::map<size_t, NotifierPtr>& m_playerNotifier, std::map<size_t, NotifierPtr>& m_botNotifiers)
{
	m_botNotifiers.clear();
	for (auto& [id, data] : m_playerNotifier)
	{
		data.reset();
	}
}

bool ExistPlayerTank(std::span<std::pair<size_t, GeneralMapObjectData> const>& objects)
{
	for (auto& [id, data] : objects)
		if (auto* type = std::get_if<TankType>(&data.type); type && *type == TankType::Player)
			return true;
	return false;
}
void CTanksView::CreateNotifiers(std::span<std::pair<size_t, GeneralMapObjectData> const>& objects)
{
	auto* contoller = GetDocument()->Contoller();
	std::vector<size_t> id_bullets;
	for (auto& [id, data] : objects)
	{
		if (auto* type = std::get_if<TankType>(&data.type))
		{
			if (*type == TankType::Player)
			{
				m_playerNotifier[id] = std::make_unique<Notifier>(*contoller, id);
			}
			else
			{
				m_botNotifiers[id] = std::make_unique<Notifier>(*contoller, id);
			}
		}
		else if (auto* type = std::get_if<BulletType>(&data.type))

		{
			id_bullets.emplace_back(id);

		}
	}

	m_bulletNotifiers = std::make_unique<NotifierBullet>(*contoller, id_bullets);
	createTank = std::make_unique<NotifierCreateTank>(*contoller);
}

void CTanksView::CreateDrawers(std::span<std::pair<size_t, GeneralMapObjectData> const>& objects, bool playerTankExist)
{
	if (!playerTankExist)
	{
		m_drawers.emplace_back(CreateTextDrawer(L"GAME OVER", { 8, 9 }, { 20, 12 }));
	}
	else
	{

		for (auto& [id, data] : objects)
			if (auto&& drawer = ::Create(data))
				m_drawers.emplace_back(std::move(drawer));
		//	if(m_botNotifiers.empty())
		m_drawers.emplace_back(CreateTextDrawer(L"WIN!!!!!!!!!!!!!!!!!!!!!!", { 8, 9 }, { 20, 12 }));

		/*bool playerTankExist = false;
		for (auto&& [id, data] : m_playerNotifier)
			if (data)
				playerTankExist = true;*/


				//GameOver();
			//обновить окно(вызовется OnDraw)
		Invalidate();
	}
}

// При изменении на карте обновить перерисовщики
void CTanksView::OnMapModify(std::span<std::pair<size_t, GeneralMapObjectData> const> objects)
{
	m_drawers.clear();
	ClearNotifier(m_playerNotifier, m_botNotifiers);
	bool playerTankExist = ExistPlayerTank(objects);
	CreateDrawers(objects,playerTankExist);
	CreateNotifiers(objects);
		//m_playerNotifier.clear();
}

// Рисование CTanksView

void CTanksView::OnDraw(CDC* pDC)
{
	CTanksDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	//Size screenSize = { 800, 400 };
	//Size mapSize = { 80, 40 };
	//int scale = int(screenSize.cx / mapSize.cx);

	//auto dc = CreateCDCContext(*pDC);
	std::unique_ptr<IDrawContext> dc= CreateCDCContext(*pDC);
	// TODO: добавьте здесь код отрисовки для собственных данных
	
	for (auto& drawer : m_drawers)
	{
		drawer->Draw(*dc);
	}
}

// todo: 
// 1. Реализовать  функции Save  и Load у контроллера(создать новые методы)
// 2. Внутри функции записи: открыть файл, получить список всех данных карты (GEneralMapObjectDAta), записать в поток
// 3. Внутри чтения записи: открыть файл, прочитать список всех данных карты (GEneralMapObjectDAta) из потока, Переложить объекты в карту(новая функция у карты)


// std::ofstream out("output.txt");
// out << input;
// out.close();

void CTanksView::OnKeyDown(UINT nchar, UINT, UINT)
{
	if (nchar == '1')
		GetDocument()->Contoller()->Save();
	//	mapmodif->Save();
	//else if (nchar == '2')
	//	mapmodif->Load();

	Events ev = Events::Stop;
	Events ev2 = Events::Stop;
	switch (nchar)
	{
	case VK_DOWN:
		ev = Events::Down;
		break;
	case VK_UP:
		ev = Events::Up;
		break;
	case VK_LEFT:
		ev = Events::Left;
		break;
	case VK_RIGHT:
		ev = Events::Right;
		break;
	case VK_SPACE:
		ev = Events::Fire;
		break;
	}
	
	switch (nchar)
	{
	case 'S':
		ev2 = Events::Down;
		break;
	case 'W':
		ev2 = Events::Up;
		break;
	case 'A':
		ev2 = Events::Left;
		break;
	case 'D':
		ev2 = Events::Right;
		break;
	case 'K':
		ev2 = Events::Fire;
		break;
	}

	std::vector<Events> Evs = { ev,ev2 };
	//for (auto&& [notifier, ev] : std::views::zip(m_playerNotifier, Evs))
	//{
	//	auto& [id, data] = notifier;
	//		if(data)
	//			data->Send(ev);
	//}

	auto it=Evs.begin();
	for (auto& [id, data] : m_playerNotifier)
	{
		if (it == Evs.end())
			break;
		if (data)
			data->Send(*it);
		it++;
	}
	//(*(m_playerNotifier.begin())).second->Send(ev);
	//(*(++m_playerNotifier.begin())).second->Send(ev2);
}

void CTanksView::OnTimer(UINT_PTR)
{
	std::vector<Notifier> bots, players;
	for (auto& [id, bot] : m_botNotifiers)
	{
		if(bot)
		bots.push_back(*bot);
	}

	for (auto& [id, player] : m_playerNotifier)
	{
		if (player)
			players.push_back(*player);
	}
	auto bullets = *m_bulletNotifiers;

	/*for (auto&& pl : m_playerNotifier)
	{
		pl.second->Execute();
		//break;
	}*/

	//(*(m_playerNotifier.begin())).second->Execute();
	//(*(++m_playerNotifier.begin())).second->Execute();
	for (auto& bot : bots)
	{
		Events ev = (Events)(std::rand() % (int)Events::Count);
		bot.Send(ev);
		bot.Execute();
	}

	for (auto& player : players)
	{
		player.Execute();
	}
	bullets.Execute();
	//static int k =0;
	//if (bots.size() < 5)
	//	k++;
	//if (k > 5)
	//{
	//	createTank->Execute();
	//	k = 0;
	//}
	createTank->Execute();
}



// Печать CTanksView


void CTanksView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CTanksView::OnInitialUpdate()
{
	CView::OnInitialUpdate();;
	GetDocument()->AddListener(this);// [&](auto&& data) {OnMapModify(data); });
	SetTimer(1, 1000, nullptr);
}

BOOL CTanksView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CTanksView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CTanksView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}

void CTanksView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTanksView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Диагностика CTanksView

#ifdef _DEBUG
void CTanksView::AssertValid() const
{
	CView::AssertValid();
}

void CTanksView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTanksDoc* CTanksView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTanksDoc)));
	return (CTanksDoc*)m_pDocument;
}
#endif //_DEBUG


// Обработчики сообщений CTanksView
