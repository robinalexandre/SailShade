#ifndef RESULT_HPP_INCLUDED
#define RESULT_HPP_INCLUDED

#include <sstream>

class Result
{
	public:

	Result();
	Result(const Result& r);
	Result(unsigned int select, unsigned int cross, unsigned int insert, unsigned int mutation, float average, float best, float min, float time);
	~Result();

	friend bool operator<(const Result& r1, const Result& r2)
	{
		return (r1.m_average == r2.m_average) ? r1.m_time < r2.m_time : r1.m_average < r2.m_average;
	}

	friend std::ostream &operator<<(std::ostream &os, const Result &r)
	{ 
		return os << r.m_select << r.m_cross << r.m_insert << r.m_mutation << std::endl 
				<< "average = " << r.m_average << std::endl
				<< "best = " << r.m_best << std::endl
				<< "minimum = " << r.m_min << std::endl
				<< "time = " << r.m_time << std::endl << std::endl;
	}

	private:

	unsigned int m_select;
	unsigned int m_cross;
	unsigned int m_insert;
	unsigned int m_mutation;

	float m_average;
	float m_best;
	float m_min;
	float m_time;
};

#endif