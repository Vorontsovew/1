
// TanksView.h: интерфейс класса CTanksView
//

#pragma once

#include <memory>
#include <vector>

#include <span>
#include <map>

struct IDrawer;
struct GeneralMapObjectData;
class Notifier;
class NotifierBullet;
class NotifierCreateTank;
using NotifierPtr = std::unique_ptr<Notifier>;
using NotifierBulletPtr = std::unique_ptr<NotifierBullet>;

////вью(окно), играет роль вью в паттерне MVC(model/view/controller)
class CTanksView : public CView, public IListener
{
	std::vector<std::unique_ptr<IDrawer>> m_drawers;
	std::map<size_t, NotifierPtr> m_playerNotifier;
	std::map<size_t, NotifierPtr> m_botNotifiers;
	NotifierBulletPtr m_bulletNotifiers;
	std::unique_ptr<NotifierCreateTank> createTank;

	void CreateNotifiers(std::span<std::pair<size_t, GeneralMapObjectData> const>&);
	void CreateDrawers(std::span<std::pair<size_t, GeneralMapObjectData> const>&, bool);
protected: // создать только из сериализации
	CTanksView() noexcept;
	DECLARE_DYNCREATE(CTanksView)

// Атрибуты
public:
	CTanksDoc* GetDocument() const;

// Операции
public:

// Переопределение
public:
	virtual void OnDraw(CDC* pDC);  // переопределено для отрисовки этого представления
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	
//функция-слушатель модели, вызывается при изменении карты

	void OnMapModify(std::span<std::pair<size_t, GeneralMapObjectData> const> objects);
protected:
	virtual void OnInitialUpdate() override;
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	

// Реализация
public:
	virtual ~CTanksView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnKeyDown(UINT, UINT, UINT);
	afx_msg void OnTimer(UINT_PTR);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // версия отладки в TanksView.cpp
inline CTanksDoc* CTanksView::GetDocument() const
   { return reinterpret_cast<CTanksDoc*>(m_pDocument); }
#endif

