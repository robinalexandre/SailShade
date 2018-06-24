#include "Azimuth.hpp"

using namespace std;

Azimuth::Azimuth(struct Location _loc, struct Time _time_beg, struct Time _time_end)
:loc(_loc), time_beg(_time_beg), time_end(_time_end)
{}

double * Azimuth::calc_azimuths(int mod)
{
    int h(1), min(5);
	switch(mod) // Switch on the mod
	{
		case 0 : 
		{
			h = 24;
			min = 60;
			time_beg.hour = 12;
			time_beg.minute = 0;
			break;
		}

		case 1 :
		{
			h = 1;
			min = 60;
			time_beg.minute = 0;
			break;
		}

		case 2 :
		{
			h = 1;
			min = 30;
			if (time_beg.minute >= 30)
				time_beg.minute = 30;
			else
				time_beg.minute = 0;
			break;
		}

		case 3 :
		{
			h = 1;
			min = 1;
			break;
		}
	}

	int y1 = time_beg.year, y2 = time_end.year;
	int m1 = time_beg.month, m2 = time_end.month;
	int d1 = time_beg.day, d2 = time_end.day;
	int h1 = time_beg.hour, h2 = time_end.hour;
	int mi1 = time_beg.minute, mi2 = time_end.minute;
	struct Time time;
	time.second = 0;

	struct Position pos;

	double azimuth, altitude;

	vector<double> vectors;

	double *result;

	while (y1 < y2) // Calc for all years, not the last
	{
		int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
		if (((y1%4 == 0) && (y1%100 != 0)) || (y1%400 == 0))
			days[1] = 29;
		time.year = y1;
		while(m1 <= 12)
		{
			time.month = m1;
			while(d1 <= days[m1-1])
			{
				time.day = d1;
				while (h1 <= 23)
				{
					time.hour = h1;
					while (mi1 <= 59)
					{		
						time.minute = mi1;
						SolTrack(time, loc, &pos, 0, 1, 1, 1);
						azimuth = pos.azimuthRefract;
						altitude = pos.altitudeRefract;
						if (altitude < 0)
						{
							vectors.push_back(sin(azimuth)*cos(altitude));
							vectors.push_back(sin(azimuth)*sin(altitude));
							vectors.push_back(cos(azimuth));
						}
						mi1 += min;
					}
					h1 += h;
					mi1 = 0;
				}
				d1++;
				h1 = 0;
			}
			m1++;
			d1 = 1;
		}
		y1++;
		m1 = 1;
	}


	int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	if (((y2%4 == 0) && (y2%100 != 0)) || (y2%400 == 0))
		days[1] = 29;



	time.year = y2; // Calc for the last year


	while(m1 < m2) // Calc of all the months of the last year
	{
		time.month = m1;
		while(d1 <= days[m1-1])
		{
			time.day = d1;
			while (h1 <= 23)
			{
				time.hour = h1;
				while (mi1 <= 59)
				{		
					time.minute = mi1;
					SolTrack(time, loc, &pos, 0, 1, 1, 1);
					azimuth = pos.azimuthRefract;
					altitude = pos.altitudeRefract;
					if (altitude < 0)
					{
						vectors.push_back(sin(azimuth)*cos(altitude));
						vectors.push_back(sin(azimuth)*sin(altitude));
						vectors.push_back(cos(azimuth));
					}
					mi1 += min;
				}
				h1 += h;
				mi1 = 0;
			}
			d1++;
			h1 = 0;
		}
		m1++;
		d1 = 1;
	}



	time.month = m2; // Calc for the last month of the last year



	while(d1 < d2) // Calc of all the days of the last month
	{
		time.day = d1;
		while (h1 <= 23)
		{
			time.hour = h1;
			while (mi1 <= 59)
			{		
				time.minute = mi1;
				SolTrack(time, loc, &pos, 0, 1, 1, 1);
				azimuth = pos.azimuthRefract;
				altitude = pos.altitudeRefract;
				if (altitude < 0)
				{
					vectors.push_back(sin(azimuth)*cos(altitude));
					vectors.push_back(sin(azimuth)*sin(altitude));
					vectors.push_back(cos(azimuth));
				}
				mi1 += min;
			}
			h1 += h;
			mi1 = 0;
		}
		d1++;
		h1 = 0;
	}



	time.day = d2; // Calc for the last day of the last month of the last year



	while (h1 < h2) // Calc of all the hours of the last day of the last month of the last year
	{
		time.hour = h1;
		while (mi1 <= 59)
		{		
			time.minute = mi1;
			SolTrack(time, loc, &pos, 0, 1, 1, 1);
			azimuth = pos.azimuthRefract;
			altitude = pos.altitudeRefract;
			if (altitude < 0)
			{
				vectors.push_back(sin(azimuth)*cos(altitude));
				vectors.push_back(sin(azimuth)*sin(altitude));
				vectors.push_back(cos(azimuth));
			}
			mi1 += min;
		}
		h1 += h;
		mi1 = 0;
	}



	time.hour = h2; // Calc for the last hour of the last day of the last month of the last year



	while (mi1 <= mi2) // Calc for all the minutes of the last hour of the last day of the last month of the last year
	{		
		time.minute = mi1;
		SolTrack(time, loc, &pos, 0, 1, 1, 1);
		azimuth = pos.azimuthRefract;
		altitude = pos.altitudeRefract;
		if (altitude < 0)
		{
			vectors.push_back(sin(azimuth)*cos(altitude));
			vectors.push_back(sin(azimuth)*sin(altitude));
			vectors.push_back(cos(azimuth));
		}
		mi1 += min;
	}
	
	result = new double[vectors.size()];

    m_nb_azimuth = vectors.size()/3;

	for (unsigned int i = 0; i < vectors.size(); ++i)
	{
		result[i] = vectors[i];
	}
    return result;
}
