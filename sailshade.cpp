#include <iostream>
#include <algorithm>
#include <iomanip>
#include <fstream>
#include "sailshade.hpp"
#include "Azimuth.hpp"

using namespace std;

SailShade::SailShade(unsigned int sizePop, int modAzimut, Location loc, Time time_beg, Time time_end, float step_points_floor, Rectangle floor, int prob_mut, Borne b_1, Borne b_2, Borne b_3, float _rate_shad)
: m_genome_size(9), m_size(sizePop), m_pop(new float[sizePop*m_genome_size]), m_fitness(new float[sizePop]), m_area(new float[sizePop]), m_shadow(new float[sizePop]), m_indexSorted(new unsigned int[sizePop]), m_born(new unsigned int[sizePop]), m_step(step_points_floor), m_floor(floor), m_nb_cross(sizePop/2), m_prob_mut(prob_mut), m_b_1(b_1), m_b_2(b_2), m_b_3(b_3), rate_shad(_rate_shad)
{
    for(unsigned int i(0); i < m_size*m_genome_size; i++)
    {
        m_pop[i] = 0.;
    }
    for(unsigned int i(0); i < m_size; i++)
    {
        m_fitness[i] = 0.;
        m_area[i] = 0.;
        m_shadow[i] = 0.;
        m_indexSorted[i] = i;
    }
    Azimuth azimuth(loc, time_beg, time_end);
    this->m_director_ray = azimuth.calc_azimuths(modAzimut);
    this->m_nb_azimuth = azimuth.m_nb_azimuth;

    m_max_area = 0.;
    
    //Generation for selection and insertion

    int intervals[] = {0, (int)m_size/2, (int)m_size};
    double weights[] = {10.0,1.0,0.0};

    std::piecewise_linear_distribution<double> distribution (intervals,intervals+3,weights);
    m_distribution = distribution;
    srand((unsigned int)time(nullptr));
}

SailShade::~SailShade()
{
    delete[] m_pop;
    delete[] m_fitness;
    delete[] m_indexSorted;
	delete[] m_born;
    delete[] m_director_ray;
    delete[] m_area;
    delete[] m_shadow;
}

void SailShade::instanciation()
{
    for (unsigned int i = 0; i < m_size*m_genome_size; i+=9)
    {
        m_pop[i] = (float)((rand() % ((int)(m_b_1.max_x*100) - (int)(m_b_1.min_x*100) + 1)) + m_b_1.min_x*100)/100;
        m_pop[i+1] = (float)((rand() % ((int)(m_b_1.max_y*100) - (int)(m_b_1.min_y*100) + 1)) + m_b_1.min_y*100)/100;
        m_pop[i+2] = (float)((rand() % ((int)(m_b_1.max_z*100) - (int)(m_b_1.min_z*100) + 1)) + m_b_1.min_z*100)/100;
        m_pop[i+3] = (float)((rand() % ((int)(m_b_2.max_x*100) - (int)(m_b_2.min_x*100) + 1)) + m_b_2.min_x*100)/100;
        m_pop[i+4] = (float)((rand() % ((int)(m_b_2.max_y*100) - (int)(m_b_2.min_y*100) + 1)) + m_b_2.min_y*100)/100;
        m_pop[i+5] = (float)((rand() % ((int)(m_b_2.max_z*100) - (int)(m_b_2.min_z*100) + 1)) + m_b_2.min_z*100)/100;
        m_pop[i+6] = (float)((rand() % ((int)(m_b_3.max_x*100) - (int)(m_b_3.min_x*100) + 1)) + m_b_3.min_x*100)/100;
        m_pop[i+7] = (float)((rand() % ((int)(m_b_3.max_y*100) - (int)(m_b_3.min_y*100) + 1)) + m_b_3.min_y*100)/100;
        m_pop[i+8] = (float)((rand() % ((int)(m_b_3.max_z*100) - (int)(m_b_3.min_z*100) + 1)) + m_b_3.min_z*100)/100;
    }
    
    this->eval_all();
    sort();
    
    m_best.triangle.p1.x = m_pop[m_indexSorted[0]*m_genome_size];
    m_best.triangle.p1.y = m_pop[m_indexSorted[0]*m_genome_size+1];
    m_best.triangle.p1.z = m_pop[m_indexSorted[0]*m_genome_size+2];
    m_best.triangle.p2.x = m_pop[m_indexSorted[0]*m_genome_size+3];
    m_best.triangle.p2.y = m_pop[m_indexSorted[0]*m_genome_size+4];
    m_best.triangle.p2.z = m_pop[m_indexSorted[0]*m_genome_size+5];
    m_best.triangle.p3.x = m_pop[m_indexSorted[0]*m_genome_size+6];
    m_best.triangle.p3.y = m_pop[m_indexSorted[0]*m_genome_size+7];
    m_best.triangle.p3.z = m_pop[m_indexSorted[0]*m_genome_size+8];
    
    m_best.fitness = m_fitness[m_indexSorted[0]];
    m_best.shadow = m_shadow[m_indexSorted[0]];
    m_best.area = m_area[m_indexSorted[0]];
}

float SailShade::algo(SELECT sel, CROSS cro, MUTATION mut, INSERT ins, unsigned int itMax, std::vector<std::ofstream>& output)
{
    int x;
    unsigned int par_1, par_2;
    unsigned int nb_it = 0;
    unsigned int i;
    vector<unsigned int> parents;
    vector<float*> children;
    
    long t_begin = time(nullptr);
    
    std::cout << "Iteration" << setw(10);
    std::cout << "Best" << setw(15);
    std::cout << "Average" << setw(15);
    std::cout << "Worst" << std::endl;
    
    for(auto& out : output)
    {
        out << "#Iteration" << setw(10);
        out << "#Best" << setw(15);
        out << "#Average" << setw(15);
        out << "#Worst" << std::endl;
    }

    //////////// INSTANCIATION POPULATION //////////////


    instanciation();


    //////////// SELECTION & CROSSOVER    //////////////
    
    while (nb_it < itMax) {
        children.clear();
        for(i = 0; i < m_nb_cross; ++i)
        {
            parents.clear();
            par_1 = (this->*sel)();
            parents.push_back(par_1);
            do
            {
                par_2 = (this->*sel)();
            } while(par_1 == par_2);
            parents.push_back(par_2);

            children.push_back((this->*cro)(parents));
        }


    //////////// INSERTION CHILDREN     //////////////


        (this->*ins)(children, nb_it);


    //////////// MUTATION POPULATION    //////////////


        for(i = 0; i < m_size; ++i)
        {
            x = rand()%100+1;
            if (x <= m_prob_mut) {
                (this->*mut)(i,(1-m_fitness[i])*m_d_mut_max);
            }
        }


    //////////// EVALUATION POPULATION   /////////////
        
        this->eval_all();
        sort();
        this->checkBest();
    
        std::cout << setw(5) << nb_it << setw(15);
        std::cout << m_fitness[m_indexSorted[0]] << setw(15);
        std::cout << m_average_fitness << setw(15);
        std::cout << m_fitness[m_indexSorted[m_size-1]] << std::endl;
        
        for(auto& out : output)
        {
            out << setw(5) << nb_it << setw(15);
            out << m_fitness[m_indexSorted[0]] << setw(15);
            out << m_average_fitness << setw(15);
            out << m_fitness[m_indexSorted[m_size-1]] << std::endl;
        }

        ++nb_it;
    }


    for(auto& out : output)
    {
        out << "#P1 sail (x, y, z): " << m_best.triangle.p1.x << ", " << m_best.triangle.p1.y << ", " << m_best.triangle.p1.z << std::endl;
        out << "#P2 sail (x, y, z): " << m_best.triangle.p2.x << ", " << m_best.triangle.p2.y << ", " << m_best.triangle.p2.z << std::endl;
        out << "#P3 sail (x, y, z): " << m_best.triangle.p3.x << ", " << m_best.triangle.p3.y << ", " << m_best.triangle.p3.z << std::endl;
        out << "Fitness (percent): " << m_best.fitness << std::endl;
        out << "#Shadow (percent): " << m_best.shadow << std::endl;
        out << "#Area sail (m2): " << m_best.area << " (percent): " << m_best.rate_area << std::endl;
    }

    std::cout << "Best: P1 sail (x, y, z) :" << m_best.triangle.p1.x << ", " << m_best.triangle.p1.y << ", " << m_best.triangle.p1.z << std::endl;
    std::cout << "Best: P2 sail (x, y, z) :" << m_best.triangle.p2.x << ", " << m_best.triangle.p2.y << ", " << m_best.triangle.p2.z << std::endl;
    std::cout << "Best: P3 sail (x, y, z) :" << m_best.triangle.p3.x << ", " << m_best.triangle.p3.y << ", " << m_best.triangle.p3.z << std::endl;
    std::cout << "Fitness (percent):" << m_best.fitness << std::endl;
    std::cout << "Shadow (percent):" << m_best.shadow << std::endl;
    std::cout << "Area sail (m2): " << m_best.area << " (percent): " << m_best.rate_area << std::endl;
    std::cout << "TIME: " << time(nullptr) - t_begin << std::endl;

    return this->m_best.fitness;
}

float SailShade::algo(const std::vector<SELECT>& sel, const std::vector<CROSS>& cro, const std::vector<MUTATION>& mut, const std::vector<INSERT>& ins, unsigned int itMax, std::vector<std::ofstream>& output)
{
    int x;
    unsigned int par_1, par_2;
    unsigned int nb_it = 0;
    vector<unsigned int> parents;
    vector<float*> children;
    
    long t_begin = time(nullptr);
    
    std::cout << "Iteration" << setw(10);
    std::cout << "Best" << setw(15);
    std::cout << "Average" << setw(15);
    std::cout << "Worst" << std::endl;
    
    for(auto& out : output)
    {
        out << "#Iteration" << setw(10);
        out << "#Best" << setw(15);
        out << "#Average" << setw(15);
        out << "#Worst" << std::endl;
    }

    //////////// INSTANCIATION POPULATION //////////////


    instanciation();

    for(unsigned int i(0); i < sel.size(); ++i)
    {
        for(unsigned int j(0); j < cro.size(); ++j)
        {
            for(unsigned int k(0); k < mut.size(); ++k)
            {
                for(unsigned int l(0); l < ins.size(); ++l)
                {
                    //////////// SELECTION & CROSSOVER    //////////////

                    for(unsigned int it(0); it < itMax; ++it,++nb_it)
                    {
                        children.clear();
                        for(unsigned int c(0); c < m_nb_cross; ++c)
                        {
                            parents.clear();
                            par_1 = (this->*sel[i])();
                            parents.push_back(par_1);
                            do
                            {
                                par_2 = (this->*sel[i])();
                            } while(par_1 == par_2);
                            parents.push_back(par_2);

                            children.push_back((this->*cro[j])(parents));
                        }


                    //////////// INSERTION CHILDREN     //////////////


                        (this->*ins[l])(children, nb_it);


                    //////////// MUTATION POPULATION    //////////////


                        for(unsigned int m(0); m < m_size; ++m)
                        {
                            x = rand()%100+1;
                            if (x <= m_prob_mut) {
                                (this->*mut[k])(m,(1-m_fitness[m])*m_d_mut_max);
                            }
                        }


                    //////////// EVALUATION POPULATION   /////////////
                        
                        this->eval_all();
                        sort();
                        this->checkBest();
                    
                        std::cout << setw(5) << nb_it << setw(15);
                        std::cout << m_fitness[m_indexSorted[0]] << setw(15);
                        std::cout << m_average_fitness << setw(15);
                        std::cout << m_fitness[m_indexSorted[m_size-1]] << std::endl;
                        
                        for(auto& out : output)
                        {
                            out << setw(5) << nb_it << setw(15);
                            out << m_fitness[m_indexSorted[0]] << setw(15);
                            out << m_average_fitness << setw(15);
                            out << m_fitness[m_indexSorted[m_size-1]] << std::endl;
                        }
                    }
                }
            }
        }
    }


    for(auto& out : output)
    {
        out << "#P1 sail (x, y, z): " << m_best.triangle.p1.x << ", " << m_best.triangle.p1.y << ", " << m_best.triangle.p1.z << std::endl;
        out << "#P2 sail (x, y, z): " << m_best.triangle.p2.x << ", " << m_best.triangle.p2.y << ", " << m_best.triangle.p2.z << std::endl;
        out << "#P3 sail (x, y, z): " << m_best.triangle.p3.x << ", " << m_best.triangle.p3.y << ", " << m_best.triangle.p3.z << std::endl;
        out << "Fitness (percent): " << m_best.fitness << std::endl;
        out << "#Shadow (percent): " << m_best.shadow << std::endl;
        out << "#Area sail (m2): " << m_best.area << " (percent): " << m_best.rate_area << std::endl;
    }

    std::cout << "Best: P1 sail (x, y, z) :" << m_best.triangle.p1.x << ", " << m_best.triangle.p1.y << ", " << m_best.triangle.p1.z << std::endl;
    std::cout << "Best: P2 sail (x, y, z) :" << m_best.triangle.p2.x << ", " << m_best.triangle.p2.y << ", " << m_best.triangle.p2.z << std::endl;
    std::cout << "Best: P3 sail (x, y, z) :" << m_best.triangle.p3.x << ", " << m_best.triangle.p3.y << ", " << m_best.triangle.p3.z << std::endl;
    std::cout << "Fitness (percent):" << m_best.fitness << std::endl;
    std::cout << "Shadow (percent):" << m_best.shadow << std::endl;
    std::cout << "Area sail (m2): " << m_best.area << " (percent): " << m_best.rate_area << std::endl;
    std::cout << "TIME: " << time(nullptr) - t_begin << std::endl;

    return this->m_best.fitness;
}

void SailShade::checkBest()
{
    if(m_fitness[m_indexSorted[0]] > m_best.fitness) {
        m_best.triangle.p1.x = m_pop[m_indexSorted[0]*m_genome_size];
        m_best.triangle.p1.y = m_pop[m_indexSorted[0]*m_genome_size+1];
        m_best.triangle.p1.z = m_pop[m_indexSorted[0]*m_genome_size+2];
        m_best.triangle.p2.x = m_pop[m_indexSorted[0]*m_genome_size+3];
        m_best.triangle.p2.y = m_pop[m_indexSorted[0]*m_genome_size+4];
        m_best.triangle.p2.z = m_pop[m_indexSorted[0]*m_genome_size+5];
        m_best.triangle.p3.x = m_pop[m_indexSorted[0]*m_genome_size+6];
        m_best.triangle.p3.y = m_pop[m_indexSorted[0]*m_genome_size+7];
        m_best.triangle.p3.z = m_pop[m_indexSorted[0]*m_genome_size+8];
        
        m_best.fitness = m_fitness[m_indexSorted[0]];
        m_best.shadow = m_shadow[m_indexSorted[0]];
        m_best.area = m_area[m_indexSorted[0]];
        m_best.rate_area = (1-(m_area[m_indexSorted[0]] / m_max_area));
    }
}

void SailShade::sort()
{
    bool tmp(true);

    while(tmp)
    {
        tmp = false;

        for(unsigned int i(1); i < m_size; i++)
        {
            if(m_fitness[m_indexSorted[i-1]] < m_fitness[m_indexSorted[i]])
            {
                unsigned int tmpI(m_indexSorted[i]);
                m_indexSorted[i] = m_indexSorted[i-1];
                m_indexSorted[i-1] = tmpI;

                tmp = true;
            }
        }
    }
}
