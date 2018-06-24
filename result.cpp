#include "result.hpp"

Result::Result() : m_select(0), m_cross(0), m_insert(0), m_mutation(0), m_average(0.), m_best(0.), m_min(0.), m_time(0.)
{
	
}

Result::Result(const Result& r) : m_select(r.m_select), m_cross(r.m_cross), m_insert(r.m_insert), m_mutation(r.m_mutation), m_average(r.m_average), m_best(r.m_best), m_min(r.m_min), m_time(r.m_time)
{
	
}

Result::Result(unsigned int select, unsigned int cross, unsigned int insert, unsigned int mutation, float average, float best, float min, float time) : m_select(select), m_cross(cross), m_insert(insert), m_mutation(mutation), m_average(average), m_best(best), m_min(min), m_time(time)
{
	
}

Result::~Result()
{
	
}
