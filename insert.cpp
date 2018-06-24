#include "sailshade.hpp"

//INSERT


void SailShade::insert_replace_worst(const std::vector<float*>& children, unsigned int it)
{
	it = it; //Avoid warning -Wunused-parameter
    for(unsigned int i(0); i < children.size(); i++)
    {
        for(unsigned int j(0); j < m_genome_size-1; j++)
        {
            m_pop[m_indexSorted[m_size-i-1]*m_genome_size+j] = children[i][j];
        }
        delete[] children[i];
    }
}

void SailShade::insert_alea(const std::vector<float*>& children, unsigned int it)
{
	it = it; //Avoid warning -Wunused-parameter
    for(unsigned int i(0); i < children.size(); i++)
    {
        int index = rand()%m_size;
        
        for(unsigned int j(0); i < m_genome_size; i++)
        {
            m_pop[index*m_genome_size+j] = children[j][i];
        }
        delete[] children[i];
    }
}

void SailShade::insert_age(const std::vector<float*>& children, unsigned int it)
{
	for(unsigned int i(0); i < children.size(); i++)
	{
		unsigned int oldest(0);
		for(unsigned int j(0); j < m_size; j++)
		{
			oldest = (m_born[oldest] > m_born[j]) ? j : oldest;
		}

		for(unsigned int j(0); j < m_genome_size; j++)
		{
			m_pop[oldest*m_genome_size+j] = children[i][j];
		}
		m_born[oldest] = it;
		delete[] children[i];
	}
}















