#ifndef CONFIGS_HPP_INCLUDED
#define CONFIGS_HPP_INCLUDED

#include <iostream>
#include <sstream>
#include <fstream>
#include "sailshade.hpp"

struct Configs{
	unsigned int STAT;
	unsigned int ITMAX;
	
	int seed;
	int algorithm;
	int monoSelect;
	int monoCross;
	int monoInsert;
	int monoMutation;
	int probMutation;

	std::string pathOut;
	std::string pathConstraints;

	bool helped;
	bool graphic;

	std::vector<SELECT> allSelect;
	std::vector<CROSS> allCross;
	std::vector<INSERT> allInsert;
	std::vector<MUTATION> allMutation;

	std::vector<std::string> nameSelect;
	std::vector<std::string> nameCross;
	std::vector<std::string> nameInsert;
	std::vector<std::string> nameMutation;
};

struct Constraints{
    Time time_beg;
    Time time_end;
    Location loc;
    Borne b_1;
    Borne b_2;
    Borne b_3;
    SailShade::Rectangle floor;
    float rate_shad;
};

void changeConfigsWithOpt(int argc, char **argv, Configs& conf);
void getConstraints(const Configs& conf, Constraints& cons);
std::ostream& operator<<(std::ostream& out, const Configs& conf);
std::ostream& operator<<(std::ostream& out, const Constraints& cons);

#endif