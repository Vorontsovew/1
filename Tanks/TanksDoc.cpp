
// TanksDoc.cpp: реализация класса CTanksDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "Tanks.h"
#endif

#include "MapModifier.h"
#include "TanksDoc.h"
#include "Map.h"
#include "MapGenerator.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTanksDoc

IMPLEMENT_DYNCREATE(CTanksDoc, CDocument)

BEGIN_MESSAGE_MAP(CTanksDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CTanksDoc

CTanksDoc::CTanksDoc() noexcept
{
  // TODO: добавьте код для одноразового вызова конструктора

}

CTanksDoc::~CTanksDoc()
{
}

BOOL CTanksDoc::OnNewDocument()
{
  if (!CDocument::OnNewDocument())
    return FALSE;

  m_map = GenerateMap(5);
  m_controller = CreateMapModifier(*m_map);

  // TODO: добавьте код повторной инициализации
  // (Документы SDI будут повторно использовать этот документ)

  return TRUE;
}

void CTanksDoc::AddListener(FnMapModify fn)
{
  if (m_map)
    m_map->AddListener(fn);
}


IMapModifier* CTanksDoc::Contoller()
{
  return m_controller.get();
}



// Сериализация CTanksDoc

void CTanksDoc::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    // TODO: добавьте код сохранения
  }
  else
  {
    // TODO: добавьте код загрузки
  }
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CTanksDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
  // Измените этот код для отображения данных документа
  dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

  CString strText = _T("TODO: implement thumbnail drawing here");
  LOGFONT lf;

  CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
  pDefaultGUIFont->GetLogFont(&lf);
  lf.lfHeight = 36;

  CFont fontDraw;
  fontDraw.CreateFontIndirect(&lf);

  CFont* pOldFont = dc.SelectObject(&fontDraw);
  dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
  dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CTanksDoc::InitializeSearchContent()
{
  CString strSearchContent;
  // Задание содержимого поиска из данных документа.
  // Части содержимого должны разделяться точкой с запятой ";"

  // Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
  SetSearchContent(strSearchContent);
}

void CTanksDoc::SetSearchContent(const CString& value)
{
  if (value.IsEmpty())
  {
    RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
  }
  else
  {
    CMFCFilterChunkValueImpl *pChunk = nullptr;
    ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
    if (pChunk != nullptr)
    {
      pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
      SetChunkValue(pChunk);
    }
  }
}

#endif // SHARED_HANDLERS

// Диагностика CTanksDoc

#ifdef _DEBUG
void CTanksDoc::AssertValid() const
{
  CDocument::AssertValid();
}

void CTanksDoc::Dump(CDumpContext& dc) const
{
  CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CTanksDoc
