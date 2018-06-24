#ifndef AZIMUTH_HPP
#define AZIMUTH_HPP

extern "C"
{
	#include "SolTrack.h"
}
#include <cstdlib>
#include <vector>
#include <iostream>

class Azimuth
{
private:
	struct Location loc;
	struct Time time_beg;
	struct Time time_end;

public:
	Azimuth(struct Location _loc, struct Time _time_beg, struct Time _time_end);
    long m_nb_azimuth;

	double * calc_azimuths(int mod); /* 0 -> per days
												   1 -> per hours
												   2 -> per 30 minutes
												   3 -> per minutes
												   return a vectors of all the calculate coordonates of the sun on the period*/
};

#endif
