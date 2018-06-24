#include "sailshade.hpp"

//CROSSOVER

using namespace std;

float* SailShade::cross_mono(vector<unsigned int> parents)
{
	float *child;
	int pos;

	pos = rand() % m_genome_size;

	child = new float[m_genome_size];

	for (int i = 0; i <= pos; ++i)
		child[i] = m_pop[parents[0]*m_genome_size+i];

	for (unsigned int i = pos+1; i < m_genome_size; ++i)
		child[i] = m_pop[parents[1]*m_genome_size+i];

	return child;
}

float* SailShade::cross_mono_2(vector<unsigned int> parents)
{
	float *child;
	int pos;
	int k;

	child = new float[m_genome_size];

	for (unsigned int i = 0; i < 3; ++i)
	{
		pos = rand() % 3;

		for (int j = 0; j <= pos; ++j)
		{
			k = i*3+j;
			child[k] = m_pop[parents[0]*m_genome_size+k]; 
		}
		for (int j = pos; j < 3; ++j)
		{
			k = i*3+j;
			child[k] = m_pop[parents[1]*m_genome_size+k]; 
		}
	}

	return child;
}

float* SailShade::cross_uni(vector<unsigned int> parents)
{
	int x;
	float* child;

    child = new float[m_genome_size];

	for (unsigned int i = 0; i < m_genome_size; ++i)
	{
		x = rand() % 2;
		child[i] = m_pop[parents[x]*m_genome_size+i];
	}

	return child;
}

float* SailShade::cross_aver(vector<unsigned int> parents)
{
	float* child;
	float x;

	child = new float[m_genome_size];

	for (unsigned int i = 0; i < m_genome_size; ++i)
	{
		x = 0;
		for (unsigned int j = 0; j < parents.size(); ++j)
			x += m_pop[parents[j]*m_genome_size+i];
		x /= parents.size();
		child[i] = x;
	}
	return child;
}

float* SailShade::cross_best(vector<unsigned int> parents)
{
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};

	float* child;
	float diff;
	int pos = rand() % m_genome_size;
	int x = 0;

	child = new float[m_genome_size];

	if (m_fitness[parents[0]] < m_fitness[parents[1]])
		x = 1;

	for (unsigned int i = 0; i < m_genome_size; ++i)
		child[i] = m_pop[parents[x]*m_genome_size+i];


	diff = (m_pop[parents[x]*m_genome_size+pos] <= m_pop[parents[1-x]*m_genome_size+pos]) ? -1 : 1;
	child[pos] += 0.1*diff;

	child[pos] = (child[pos] <= inf[pos]) ? inf[pos] : child[pos];

	child[pos] = (child[pos] >= inf[pos]) ? sup[pos] : child[pos];

	return child;
}