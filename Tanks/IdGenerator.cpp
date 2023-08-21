#include "pch.h"
#include "IdGenerator.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

std::size_t IdGenerator::generate()
{
	return ++m_globalId;
}
