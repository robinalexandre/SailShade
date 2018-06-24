#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <irrlicht.h>
#include "sailshade.hpp"
#include "result.hpp"
#include "configs.hpp"

#ifdef WINDOWS
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

int main(int argc, char **argv)
{
	//SET UP
/*
	Configs conf =	{5,1000,(int)time(nullptr),0,0,0,0,0,75,"test.out","constraints.conf",false,
						{&SailShade::select_alea,&SailShade::select_best,&SailShade::select_tourn<30>,&SailShade::select_tourn_v2<30>,&SailShade::select_tourn_v2<50>}, //allSelect
						{&SailShade::cross_mono,&SailShade::cross_mono_2,&SailShade::cross_uni,&SailShade::cross_aver,&SailShade::cross_best}, //allCross
						{&SailShade::insert_replace_worst,&SailShade::insert_alea,&SailShade::insert_age,&SailShade::insert_up<>}, //allInsert
						{&SailShade::mutation_1coord,&SailShade::mutation_2coord,&SailShade::mutation_pilar,&SailShade::exchange_mutation,&SailShade::replace_mutation,&SailShade::local_optimum}, //allMutation

						{"select_alea","select_best","select_tourn_30","select_tourn_v2_30","select_tourn_v2_50"}, //nameSelect
						{"cross_mono","cross_mono_2","cross_uni","cross_aver","cross_best"}, //nameCross
						{"insert_replace_worst","insert_alea","insert_age","insert_up"}, //nameInsert
						{"mutation_1coord","mutation_2coord","mutation_pilar","exchange_mutation","replace_mutation","local_optimum"} //nameMutation
					};*/

	Configs conf =	{5,100,(int)time(nullptr),0,0,0,0,0,75,"test.out","constraints.conf",false,false,
						{&SailShade::select_best}, //allSelect
						{&SailShade::cross_best}, //allCross
						{&SailShade::insert_replace_worst}, //allInsert
						{&SailShade::mutation_1coord,&SailShade::mutation_2coord,&SailShade::mutation_pilar,&SailShade::exchange_mutation,&SailShade::replace_mutation,&SailShade::local_optimum,&SailShade::composition_mutation<&SailShade::replace_mutation,&SailShade::local_optimum>,&SailShade::composition_mutation<&SailShade::mutation_pilar,&SailShade::local_optimum>}, //allMutation

						{"select_best"}, //nameSelect
						{"cross_best"}, //nameCross
						{"insert_replace_worst"}, //nameInsert
						{"mutation_1coord","mutation_2coord","mutation_pilar","exchange_mutation","replace_mutation","local_optimum","local_optimum+replace","local_optimum+pilar"} //nameMutation
					};

	//ARGS

	changeConfigsWithOpt(argc,argv,conf);

	srand(conf.seed);

	//CONFIGS

	std::ofstream out(conf.pathOut);

	out << conf;

	//CONSTRAINTS

	Constraints cons;
	getConstraints(conf,cons);

	out << cons;
    
    //Get working directory for file path

	std::string results_working_dir;
	{
		char buff[1024];
		results_working_dir = GetCurrentDir( buff, 1024 );
	}
	results_working_dir += "/Results/";


	//ALGORITHM

	if(conf.algorithm)
		std::cerr << "begin" << std::endl << std::endl;
    

    if(conf.algorithm == 1)
    {
        SailShade sailshade(40,2,cons.loc,cons.time_beg,cons.time_end,0.3,cons.floor,conf.probMutation,cons.b_1,cons.b_2,cons.b_3,cons.rate_shad);
        
        // Name of the output file for gnuplot
        std::vector<std::ofstream> output;
        output.push_back(std::ofstream(results_working_dir+conf.nameSelect[conf.monoSelect]+"-"+conf.nameCross[conf.monoCross]+"-"+conf.nameInsert[conf.monoInsert]+"-"+conf.nameMutation[conf.monoMutation]+".gpd",std::ofstream::out | std::ofstream::app));

    	std::cerr << "Unique test with the following presets :" << std::endl;
    	std::cerr << "Select : " << conf.monoSelect << " = " << conf.nameSelect[conf.monoSelect] << std::endl;
    	std::cerr << "Cross : " << conf.monoCross << " = " << conf.nameCross[conf.monoCross] << std::endl;
    	std::cerr << "Insert : " << conf.monoInsert << " = " << conf.nameInsert[conf.monoInsert] << std::endl;
    	std::cerr << "Mutation : " << conf.monoMutation << " = " << conf.nameMutation[conf.monoMutation] << std::endl << std::endl;

		long t_begin = time(nullptr);

		float score = sailshade.algo(conf.allSelect[conf.monoSelect], conf.allCross[conf.monoCross], conf.allMutation[conf.monoMutation], conf.allInsert[conf.monoInsert], conf.ITMAX, output);

        Result r(conf.monoSelect,conf.monoCross,conf.monoMutation,conf.monoInsert,score,score,score,time(nullptr) - t_begin);
        out << r;

        if(conf.graphic == true)
        	sailshade.graphic(cons.floor, cons.loc, cons.time_beg, cons.time_end);
	}
	else if(conf.algorithm == 2)
	{
		std::vector<Result> allResults;

		#pragma omp parallel for collapse(4)
		for(unsigned int i = 0; i < conf.allSelect.size(); ++i)						//SELECT
		{
			for(unsigned int j = 0; j < conf.allCross.size(); ++j)					//CROSS
			{
				for(unsigned int k = 0; k < conf.allInsert.size(); ++k)				//INSERT
				{
					for(unsigned int l = 0; l < conf.allMutation.size(); ++l)		//MUTATION
					{
						SailShade sailshade(40,0,cons.loc,cons.time_beg,cons.time_end,0.3,cons.floor,conf.probMutation,cons.b_1,cons.b_2,cons.b_3,cons.rate_shad);

                        // Name of the output file for gnuplot
                        std::cerr << results_working_dir << std::endl;
                        //sailshade.m_file_out.append(results_working_dir+conf.nameSelect[i]+"-"+conf.nameCross[j]+"-"+conf.nameInsert[k]+"-"+conf.nameMutation[l]+".gpd");
                        //Files
                        std::vector<std::ofstream> output;
                        output.push_back(std::ofstream(results_working_dir+conf.nameSelect[i]+"-"+conf.nameCross[j]+"-"+conf.nameInsert[k]+"-"+conf.nameMutation[l]+".gpd",std::ofstream::out | std::ofstream::app));
                        output.push_back(std::ofstream(results_working_dir+conf.nameSelect[i]+".gpd",std::ofstream::out | std::ofstream::app));
                        output.push_back(std::ofstream(results_working_dir+conf.nameCross[j]+".gpd",std::ofstream::out | std::ofstream::app));
                        output.push_back(std::ofstream(results_working_dir+conf.nameInsert[k]+".gpd",std::ofstream::out | std::ofstream::app));
                        output.push_back(std::ofstream(results_working_dir+conf.nameMutation[l]+".gpd",std::ofstream::out | std::ofstream::app));
                        
						float averageScore(0.);
						float bestScore(0.);
						float minScore(1.);
						long t_begin = time(nullptr);
						for(unsigned int m(0); m < conf.STAT; ++m)
						{
							float tmp = sailshade.algo(conf.allSelect[i], conf.allCross[j], conf.allMutation[l], conf.allInsert[k], conf.ITMAX, output);

							averageScore += tmp;
							bestScore = (bestScore < tmp) ? tmp : bestScore;
							minScore = (minScore > tmp) ? tmp : minScore;
						}
                        
						allResults.push_back(Result(i,j,k,l,averageScore/conf.STAT,bestScore,minScore,(time(nullptr) - t_begin)/(float)conf.STAT));
					}
				}
			}
		}

		std::sort(allResults.begin(),allResults.end());
		for(auto& r : allResults)
			out << r;
	}

	/*SailShade sailshade(40,3,cons.loc,cons.time_beg,cons.time_end,0.3,cons.floor,conf.probMutation,cons.b_1,cons.b_2,cons.b_3,cons.rate_shad);
    std::vector<std::ofstream> output;
    output.push_back(std::ofstream(results_working_dir+"fixed.gpd",std::ofstream::out | std::ofstream::app));

    sailshade.algo({&SailShade::select_alea},{&SailShade::cross_best},{&SailShade::local_optimum},{&SailShade::insert_replace_worst}, 100, output);

	if(conf.graphic == true)
    	sailshade.graphic(cons.floor, cons.loc, cons.time_beg, cons.time_end);*/


	if(conf.algorithm)
		std::cerr << "end" << std::endl;
	else if(!conf.helped)
		std::cerr << "Nothing happened, try ./sailshade --help" << std::endl;


    return 0;
}


