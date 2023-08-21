#pragma once


class IdGenerator
{
public:
	std::size_t generate();
private:
	std::size_t m_globalId = 0;
};

