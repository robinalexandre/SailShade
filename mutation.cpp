#include "sailshade.hpp"
#include <cstdlib>

//MUTATION
void SailShade::mutation_1coord(unsigned int ind, int d)
{
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};
	int axe = rand() % m_genome_size;
	float& mut = m_pop[ind*m_genome_size+axe];
	mut += (float)((rand() % (d*2+1) - d))/100;
	mut = (mut >= sup[axe]) ? sup[axe] : mut;
	mut = (mut <= inf[axe]) ? inf[axe] : mut;
}

void SailShade::mutation_2coord(unsigned int ind, int d)
{
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};

	int axe1,axe2;
	axe1 = rand()%m_genome_size;
	axe2 = (axe1+((rand()%(m_genome_size-1))+1))%m_genome_size;
	ind *= m_genome_size;

	float& mut1 = m_pop[ind+axe1];
	float& mut2 = m_pop[ind+axe2];

	mut1 += (float)((rand() % (d*2+1) - d))/100;
	mut1 = (mut1 >= sup[axe1]) ? sup[axe1] : mut1;
	mut1 = (mut1 <= inf[axe1]) ? inf[axe1] : mut1;

	mut2 += (float)((rand() % (d*2+1) - d))/100;
	mut2 = (mut2 >= sup[axe2]) ? sup[axe2] : mut2;
	mut2 = (mut2 <= inf[axe2]) ? inf[axe2] : mut2;
}

void SailShade::mutation_pilar(unsigned int ind, int d)
{
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};

	ind *= m_genome_size;

	int pilar = (rand() % 3)*3;

	float& mut1 = m_pop[ind + pilar];
	float& mut2 = m_pop[ind + pilar+1];
	float& mut3 = m_pop[ind + pilar+2];

	mut1 += (float)((rand() % (d*2+1) - d))/100;
	mut1 = (mut1 >= sup[pilar]) ? sup[pilar] : mut1;
	mut1 = (mut1 <= inf[pilar]) ? inf[pilar] : mut1;

	mut2 += (float)((rand() % (d*2+1) - d))/100;
	mut2 = (mut2 >= sup[pilar+1]) ? sup[pilar+1] : mut2;
	mut2 = (mut2 <= inf[pilar+1]) ? inf[pilar+1] : mut2;

	mut3 += (float)((rand() % (d*2+1) - d))/100;
	mut3 = (mut3 >= sup[pilar+2]) ? sup[pilar+2] : mut3;
	mut3 = (mut3 <= inf[pilar+2]) ? inf[pilar+2] : mut3;
}

void SailShade::exchange_mutation(unsigned int ind, int d)
{
	d = d; //Avoid warning -Wunused-parameter
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};

	int p1, p2;
	float tmp;
	p1 = rand()%3;
	p2 = ((p1+(rand()%2)+1)%3)*3;
	p1 *= 3;
	ind *= m_genome_size;

	tmp = m_pop[ind+p1];
	m_pop[ind+p1] = m_pop[ind+p2];
	m_pop[ind+p2] = tmp;

	m_pop[ind+p1] = (m_pop[ind+p1] >= sup[p1]) ? sup[p1] : m_pop[ind+p1];
	m_pop[ind+p1] = (m_pop[ind+p1] <= inf[p1]) ? inf[p1] : m_pop[ind+p1];
	m_pop[ind+p2] = (m_pop[ind+p2] >= sup[p2]) ? sup[p2] : m_pop[ind+p2];
	m_pop[ind+p2] = (m_pop[ind+p2] <= inf[p2]) ? inf[p2] : m_pop[ind+p2];

	++p1;
	++p2;

	tmp = m_pop[ind+p1];
	m_pop[ind+p1] = m_pop[ind+p2];
	m_pop[ind+p2] = tmp;

	m_pop[ind+p1] = (m_pop[ind+p1] >= sup[p1]) ? sup[p1] : m_pop[ind+p1];
	m_pop[ind+p1] = (m_pop[ind+p1] <= inf[p1]) ? inf[p1] : m_pop[ind+p1];
	m_pop[ind+p2] = (m_pop[ind+p2] >= sup[p2]) ? sup[p2] : m_pop[ind+p2];
	m_pop[ind+p2] = (m_pop[ind+p2] <= inf[p2]) ? inf[p2] : m_pop[ind+p2];

	++p1;
	++p2;

	tmp = m_pop[ind+p1];
	m_pop[ind+p1] = m_pop[ind+p2];
	m_pop[ind+p2] = tmp;

	m_pop[ind+p1] = (m_pop[ind+p1] >= sup[p1]) ? sup[p1] : m_pop[ind+p1];
	m_pop[ind+p1] = (m_pop[ind+p1] <= inf[p1]) ? inf[p1] : m_pop[ind+p1];
	m_pop[ind+p2] = (m_pop[ind+p2] >= sup[p2]) ? sup[p2] : m_pop[ind+p2];
	m_pop[ind+p2] = (m_pop[ind+p2] <= inf[p2]) ? inf[p2] : m_pop[ind+p2];
}

void SailShade::exchange_mutation_v2(unsigned int ind, int d)
{
	d = d; //Avoid warning -Wunused-parameter
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};
	ind *= m_genome_size;

	for(int i(0),p1((rand()%3)*3),p2((((p1/3)+(rand()%2)+1)%3)*3); i < 3; ++i,++p1,++p2)
	{
		float tmp = m_pop[ind+p1];
		m_pop[ind+p1] = m_pop[ind+p2];
		m_pop[ind+p2] = tmp;

		m_pop[ind+p1] = (m_pop[ind+p1] >= sup[p1]) ? sup[p1] : m_pop[ind+p1];
		m_pop[ind+p1] = (m_pop[ind+p1] <= inf[p1]) ? inf[p1] : m_pop[ind+p1];
		m_pop[ind+p2] = (m_pop[ind+p2] >= sup[p2]) ? sup[p2] : m_pop[ind+p2];
		m_pop[ind+p2] = (m_pop[ind+p2] <= inf[p2]) ? inf[p2] : m_pop[ind+p2];
	}
}


void SailShade::replace_mutation(unsigned int ind, int d)
{
	d = d; //Avoid warning -Wunused-parameter
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};

	int i = rand()%9;
	ind *= m_genome_size;

	m_pop[ind+i] = ((rand()%(int)(sup[i]*100-inf[i]*100+1))+inf[i]*100)/(float)100;
}

void SailShade::local_optimum(unsigned int ind, int d)
{
	float sup[] = {m_b_1.max_x,m_b_1.max_y,m_b_1.max_z,m_b_2.max_x,m_b_2.max_y,m_b_2.max_z,m_b_3.max_x,m_b_3.max_y,m_b_3.max_z};
	float inf[] = {m_b_1.min_x,m_b_1.min_y,m_b_1.min_z,m_b_2.min_x,m_b_2.min_y,m_b_2.min_z,m_b_3.min_x,m_b_3.min_y,m_b_3.min_z};

	int point_axe = rand() % 9;
	float* new_ind;
	float origin_fitness = m_fitness[ind];
	float new_fitness = m_fitness[ind];
	ind *= m_genome_size;

	new_ind = new float[m_genome_size];

	for (unsigned int i=0; i<m_genome_size; ++i)
		new_ind[i] = m_pop[ind+i];

	d *= (rand()%2) ? -1 : 1;

	float vec = d/(float)100;

	do{
		origin_fitness = new_fitness;
		for (unsigned int i=0; i<m_genome_size; ++i)
			m_pop[ind+i] = new_ind[i];

		new_ind[point_axe] += vec;

		new_ind[point_axe] = (new_ind[point_axe] >= sup[point_axe]) ? sup[point_axe] : new_ind[point_axe];
		new_ind[point_axe] = (new_ind[point_axe] <= inf[point_axe]) ? inf[point_axe] : new_ind[point_axe];

		new_fitness = eval_one(new_ind);
	} while(new_fitness > origin_fitness);

	delete[] new_ind;
}
