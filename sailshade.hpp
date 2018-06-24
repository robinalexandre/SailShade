#ifndef SAILSHADE_HPP_INCLUDED
#define SAILSHADE_HPP_INCLUDED

#include "Azimuth.hpp"
#include <set>
#include <stdlib.h>
#include <random>
#include <vector>

class SailShade;

//Definition of pointers of methods
typedef unsigned int (SailShade::*SELECT) ();
typedef float* (SailShade::*CROSS) (std::vector<unsigned int> parents);
typedef void (SailShade::*MUTATION) (unsigned int ind, int d);
typedef void (SailShade::*INSERT) (const std::vector<float*>& children, unsigned int it);

typedef struct Borne{
    float min_x;
    float max_x;
    float min_y;
    float max_y;
    float min_z;
    float max_z;
} Borne;

class SailShade
{
	public:
    
    struct Point {
        float x;
        float y;
        float z;
    };
    
    struct Triangle {
        Point p1;
        Point p2;
        Point p3;
    };
    
    struct Rectangle {
        Point p1;
        Point p2;
        Point p3;
        Point p4;
    };
    
    struct BestInd {
        Triangle triangle;
        float fitness;
        float shadow;
        float area;
        float rate_area;
    };
    
    BestInd m_best;

    SailShade(unsigned int sizePop, int modAzimut, Location loc, Time time_beg, Time time_end, float step_points_floor, Rectangle floor, int prob_mut, Borne b_1, Borne b_2, Borne b_3, float _rate_shad);
    ~SailShade();

	//SELECT
	unsigned int select_alea();
    unsigned int select_best();
	template<int T>
	unsigned int select_tourn()
	{
		unsigned int best(rand()%m_size);
		for(unsigned int i(0); i < m_size; i++)
		{
			best = (m_fitness[best] < (m_fitness[i]*(rand()%100 < T))) ? i : best;
		}
		return best;
	}
	template<int T>
	unsigned int select_tourn_v2()
	{
		for(unsigned int i(0); i < m_size; i++)
		{
			if(rand()%100 < T)
				return m_indexSorted[i];
		}
		return m_indexSorted[0];
	}

	//CROSSOVER

	float* cross_mono(std::vector<unsigned int> parents); // mono_point crossover
    float* cross_mono_2(std::vector<unsigned int> parents); // mono_point on each point of a genome
	float* cross_uni(std::vector<unsigned int> parents); // uniforme crossover
    float* cross_aver(std::vector<unsigned int> parents); // children is the average of the two parents
    float* cross_best(std::vector<unsigned int> parents); // children influenced by one axis of one point of the best parent compared with the other

	//MUTATION
	void mutation_1coord(unsigned int ind, int d); //mute 1coord au hasard du pilier 3 entre -d et d cm
	void mutation_2coord(unsigned int ind, int d); //mute 2coord au hasard du pilier 3 entre -d et d cm
    void mutation_pilar(unsigned int ind, int d); //mute 3coord au hasard du pilier 3 entre -d et d cm
    //quand on modifiera pilier 1 et 2 aussi:
	void exchange_mutation(unsigned int ind, int d); //échange 2 pieds
	void exchange_mutation_v2(unsigned int ind, int d); //échange 2 pieds
	void replace_mutation(unsigned int ind, int d); //remplace une valeur d'un pied
    void local_optimum(unsigned int ind, int d); // recherche d'un optimum local sur une coordonées dans une direction d ou -d
    template<MUTATION M1, MUTATION M2>
    void composition_mutation(unsigned ind, int d)
    {
    	(this->*M1)(ind,d);
    	(this->*M2)(ind,d);
    }

	//INSERT
	void insert_replace_worst(const std::vector<float*>& children, unsigned int it);
    void insert_alea(const std::vector<float*>& children, unsigned int it);
    void insert_age(const std::vector<float*>& children, unsigned int it);
    template<INSERT I = &SailShade::insert_replace_worst>
	void insert_up(const std::vector<float*>& children, unsigned int it)
	{
		float scoreMin(m_fitness[m_indexSorted[m_size-1]]);
		std::vector<float*> up;
		for(unsigned int i(0); i < children.size(); i++)
		{
			if(eval_one(children[i]) > scoreMin)
			{
				up.push_back(children[i]);
			}
			else
			{
				delete[] children[i];
			}
		}
		(this->*I)(up,it);
	}

    //EVALUATION
    Triangle getProjectTriangle(Triangle sail, double *director) const;
    void eval_all();
    float eval_one(float *ind);
    
	//ALGO
	float algo(	SELECT sel,
				CROSS cro,
				MUTATION mut,
				INSERT ins,
				unsigned int itMAx,
				std::vector<std::ofstream>& output
				);

	float algo(	const std::vector<SELECT>& sel,
				const std::vector<CROSS>& cro,
				const std::vector<MUTATION>& mut,
				const std::vector<INSERT>& ins,
				unsigned int itMAx,
				std::vector<std::ofstream>& output
				);

	//AUTRE

    void graphic(struct Rectangle rect, Location loc, Time time_beg, Time time_end);

    void instanciation();

	
	/* Update m_indexSorted
	 *		m_fitness[m_indexSorted[i]] > m_fitness[m_indexSorted[i+1]]
	 *		for 0 <= i <= m_size-1
	 */
	void sort();

    private:
    
    unsigned int m_genome_size;
    unsigned int m_size;
    float *m_pop;
    float *m_fitness;
    float *m_area;
    float *m_shadow;
    float m_max_area;
    float m_average_fitness;
    unsigned int *m_indexSorted;
    unsigned int *m_born;
    double *m_director_ray;
    long m_nb_azimuth;
    float m_step;
    struct Rectangle m_floor;
    unsigned int m_nb_cross;
    int m_prob_mut;
    float m_d_mut_max = 50.; //en cm
    struct Borne m_b_1, m_b_2, m_b_3;
    float rate_shad;
    
    void checkBest();

    //Generation for selection and insertion

	std::default_random_engine m_generator;
	std::piecewise_linear_distribution<double> m_distribution;
    
    //EVALUATION
    double computeCoverPercentage(Triangle sail, double *director, Rectangle floor) const;
    float area(Triangle projection) const;
    float area(float x1, float y1, float x2, float y2, float x3, float y3) const;
    bool isInside(struct Point p, Triangle projection) const;
    float getCoverPercentage(Triangle sail, double *director, Rectangle floor) const;
};

#endif
