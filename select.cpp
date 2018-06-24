#include "sailshade.hpp"

//SELECT

unsigned int SailShade::select_alea()
{
	return rand() % m_size;	
}

unsigned int SailShade::select_best()
{
    return m_indexSorted[(int)m_distribution(m_generator)];
}


