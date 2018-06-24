#include "configs.hpp"

void changeConfigsWithOpt(int argc, char **argv, Configs& conf)
{
	for(int i(1); i < argc;)
	{
		if(std::string(argv[i]) == "-o")
		{
			conf.pathOut = argv[i+1];
			i+=2;
		}
		if(std::string(argv[i]) == "-conf")
		{
			conf.pathConstraints = argv[i+1];
			i+=2;
		}
		else if(std::string(argv[i]) == "-seed")
		{
			conf.seed = std::stoi(argv[i+1]);
			i+=2;
		}
		else if(std::string(argv[i]) == "-mono")
		{
			conf.algorithm = 1;
			++i;
		}
		else if(std::string(argv[i]) == "-graphic")
		{
			conf.graphic = true;
			++i;
		}
		else if(std::string(argv[i]) == "-tuning")
		{
			conf.algorithm = 2;
			++i;
		}
		else if(std::string(argv[i]) == "-iteration")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 1)
			{
				conf.ITMAX = tmp;
			}
			else
			{
				std::cerr << "error -iteration : Unvalid argument : \"-iteration " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-iteration must be in [1;inf[" << std::endl; 
			}
			i+=2;
		}
		else if(std::string(argv[i]) == "-probmut")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 0 && tmp <= 100)
			{
				conf.probMutation = tmp;
			}
			else
			{
				std::cerr << "error -probmut : Unvalid argument : \"-probmut " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-probmut must be in [0;100]" << std::endl; 
			}
			i+=2;
		}
		else if(std::string(argv[i]) == "-stat")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 1)
			{
				conf.STAT = tmp;
			}
			else
			{
				std::cerr << "error -stat : Unvalid argument : \"-stat " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-stat must be in [1;inf[" << std::endl; 
			}
			i+=2;
		}
		else if(std::string(argv[i]) == "-select")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 0 && tmp < (int)conf.allSelect.size())
			{
				conf.monoSelect = tmp;
			}
			else
			{
				std::cerr << "error -select : Unvalid argument : \"-select " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-select must be in [0;" << conf.allSelect.size() << "[" << std::endl; 
			}
			i += 2;
		}
		else if(std::string(argv[i]) == "-cross")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 0 && tmp < (int)conf.allCross.size())
			{
				conf.monoCross = tmp;
			}
			else
			{
				std::cerr << "error -cross : Unvalid argument : \"-cross " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-cross must be in [0;" << conf.allCross.size() << "[" << std::endl; 
			}
			i += 2;
		}
		else if(std::string(argv[i]) == "-insert")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 0 && tmp < (int)conf.allInsert.size())
			{
				conf.monoInsert = tmp;
			}
			else
			{
				std::cerr << "error -insert : Unvalid argument : \"-insert " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-insert must be in [0;" << conf.allInsert.size() << "[" << std::endl; 
			}
			i += 2;
		}
		else if(std::string(argv[i]) == "-mutation")
		{
			int tmp = std::stoi(argv[i+1]);
			if(tmp >= 0 && tmp < (int)conf.allMutation.size())
			{
				conf.monoMutation = tmp;
			}
			else
			{
				std::cerr << "error -mutation : Unvalid argument : \"-mutation " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-mutation must be in [0;" << conf.allMutation.size() << "[" << std::endl; 
			}
			i += 2;
		}
		else if(std::string(argv[i]) == "-presets")
		{
			int tmpS = argv[i+1][0] - '0';
			int tmpC = argv[i+1][1] - '0';
			int tmpI = argv[i+1][2] - '0';
			int tmpM = argv[i+1][3] - '0';

			int tmp =tmpS;
			if(tmp >= 0 && tmp < (int)conf.allSelect.size())
			{
				conf.monoSelect = tmp;
			}
			else
			{
				std::cerr << "error -select : Unvalid argument : \"-select " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-select must be in [0;" << conf.allSelect.size() << "[" << std::endl; 
			}
			tmp =tmpC;
			if(tmp >= 0 && tmp < (int)conf.allCross.size())
			{
				conf.monoCross = tmp;
			}
			else
			{
				std::cerr << "error -cross : Unvalid argument : \"-cross " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-cross must be in [0;" << conf.allCross.size() << "[" << std::endl; 
			}
			tmp =tmpI;
			if(tmp >= 0 && tmp < (int)conf.allInsert.size())
			{
				conf.monoInsert = tmp;
			}
			else
			{
				std::cerr << "error -insert : Unvalid argument : \"-insert " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-insert must be in [0;" << conf.allInsert.size() << "[" << std::endl; 
			}
			tmp =tmpM;
			if(tmp >= 0 && tmp < (int)conf.allMutation.size())
			{
				conf.monoMutation = tmp;
			}
			else
			{
				std::cerr << "error -mutation : Unvalid argument : \"-mutation " << argv[i+1] << "\"" << std::endl;
				std::cerr << "-mutation must be in [0;" << conf.allMutation.size() << "[" << std::endl; 
			}
			i += 2;
		}
		else if(std::string(argv[i]) == "--help")
		{
			std::cout << "\"--help\" display all important informations, see -help for more informations." << std::endl;
			std::cout << "\"-o PATH\" create a file with the name PATH, and then store results in it." << std::endl;
			std::cout << "\"-conf PATH\" will take the constraints from PATH (default is constraints.conf)." << std::endl;
			std::cout << "\"-seed INT\" run every tests with the seed INT (to reproduce results)." << std::endl;
			std::cout << "\"-mono\" will run a unique test with the given presets (or defaults ones)." << std::endl;
			std::cout << "\"-graphic\", addition to \"-mono\", launch a graphic interface to present results of mono" << std::endl;
			std::cout << "\"-tuning\" will run every combinaison of the presets (takes a long time)." << std::endl;
			std::cout << "\"-iteration INT\" set the number of iteration that the algorithm will go through to INT." << std::endl;
			std::cout << "\"-probmut INT\" set the probability of mutation in percent to INT (should be in [0;100])." << std::endl;
			std::cout << "\"-stat INT\" set the number of time that the algorithm will be repeated to INT." << std::endl;
			std::cout << "\"-select INT\" change preset select with INT, no need for -tuning (default " << conf.monoSelect << " = " << conf.nameSelect[conf.monoSelect] << ")." << std::endl;
			std::cout << "\"-cross INT\" change preset cross with INT, no need for -tuning (default " << conf.monoCross << " = " << conf.nameCross[conf.monoCross] << ")." << std::endl;
			std::cout << "\"-insert INT\" change preset insert with INT, no need for -tuning (default " << conf.monoInsert << " = " << conf.nameInsert[conf.monoInsert] << ")." << std::endl;
			std::cout << "\"-mutation INT\" change preset mutation with INT, no need for -tuning (default " << conf.monoMutation << " = " << conf.nameMutation[conf.monoMutation] << ")." << std::endl;
			std::cout << "\"-presets ABCD\" change all presets, no need for -tuning." << std::endl;
			std::cout << "\"-help COMMAND\" display further informations about COMMAND." << std::endl;
			++i;
			conf.helped = true;
		}
		else if(std::string(argv[i]) == "-help")
		{
			if(std::string(argv[i+1]) == "-o")
			{
				std::cout << "\"-o PATH\" create a file with the name PATH, and then store results in it." << std::endl;
			}
			if(std::string(argv[i+1]) == "-conf")
			{
				std::cout << "\"-conf PATH\" will take the constraints from PATH (default is constraints.conf)." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-seed")
			{
				std::cout << "\"-seed INT\" run every tests with the seed INT (to reproduce results)." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-mono")
			{
				std::cout << "\"-mono\" will run a unique test with the given presets (or defaults ones)." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-graphic")
			{
				std::cout << "\"-graphic\", addition to \"-mono\", launch a graphic interface to present results of mono" << std::endl;
			}
			else if(std::string(argv[i+1]) == "-tuning")
			{
				std::cout << "\"-tuning\" will run every combinaison of the presets (takes a long time)." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-iteration")
			{
				std::cout << "\"-iteration INT\" set the number of iteration that the algorithm will go through to INT." << std::endl;
				std::cout << "INT should be at least 1." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-probmut")
			{
				std::cout << "\"-probmut INT\" set the probability of mutation in percent to INT (should be in [0;100])." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-stat")
			{
				std::cout << "\"-stat INT\" set the number of time that the algorithm will be repeated to INT." << std::endl;
				std::cout << "It is usefull to be able to make some stats and to be sure that the seed does not affect the efficiency of the algorithm." << std::endl;
				std::cout << "INT should be at least 1." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-select")
			{
				std::cout << "\"-select INT\" change preset select with INT, no need for -tuning (default " << conf.monoSelect << " = " << conf.nameSelect[conf.monoSelect] << ")." << std::endl;
				for(unsigned int j(0); j < conf.nameSelect.size(); j++)
					std::cout << j << " = " << conf.nameSelect[j] << std::endl;
			}
			else if(std::string(argv[i+1]) == "-cross")
			{
				std::cout << "\"-cross INT\" change preset cross with INT, no need for -tuning (default " << conf.monoCross << " = " << conf.nameCross[conf.monoCross] << ")." << std::endl;
				for(unsigned int j(0); j < conf.nameCross.size(); j++)
					std::cout << j << " = " << conf.nameCross[j] << std::endl;
			}
			else if(std::string(argv[i+1]) == "-insert")
			{
				std::cout << "\"-insert INT\" change preset insert with INT, no need for -tuning (default " << conf.monoInsert << " = " << conf.nameInsert[conf.monoInsert] << ")." << std::endl;
				for(unsigned int j(0); j < conf.nameInsert.size(); j++)
					std::cout << j << " = " << conf.nameInsert[j] << std::endl;
			}
			else if(std::string(argv[i+1]) == "-mutation")
			{
				std::cout << "\"-mutation INT\" change preset mutation with INT, no need for -tuning (default " << conf.monoMutation << " = " << conf.nameMutation[conf.monoMutation] << ")." << std::endl;
				for(unsigned int j(0); j < conf.nameMutation.size(); j++)
					std::cout << j << " = " << conf.nameMutation[j] << std::endl;
			}
			else if(std::string(argv[i+1]) == "-presets")
			{
				std::cout << "\"-presets ABCD\" change all presets, no need for -tuning." << std::endl;
				std::cout << "Equivalent to : -select A -cross B -insert C -mutation D" << std::endl;
			}
			else if(std::string(argv[i+1]) == "--help")
			{
				std::cout << "\"--help\" display all important informations about every command." << std::endl;
			}
			else if(std::string(argv[i+1]) == "-help")
			{
				std::cout << "\"-help COMMAND\" display further informations about COMMAND. And can be recursive." << std::endl;
			}
			else
			{
				std::cerr << "error -help : Unvalid command : \"" << argv[i+1] << "\"" << std::endl;
			}
			i += 2;
			conf.helped = true;
		}
		else
		{
			std::cerr << "error : Unvalid command : \"" << argv[i] << "\"" << std::endl;
			++i;
		}
	}
}

void getConstraints(const Configs& conf, Constraints& cons)
{
	std::ifstream in(conf.pathConstraints.c_str());

	long t_begin = time(nullptr);

	while(!in.eof())
	{
		std::string buffer;

		in >> buffer;

		if(buffer == "time_beg:")
		{
			in >> cons.time_beg.year;
			in >> cons.time_beg.month;
			in >> cons.time_beg.day;
			in >> cons.time_beg.hour;
			in >> cons.time_beg.minute;
			in >> cons.time_beg.second;
		}
		else if(buffer == "time_end:")
		{
			in >> cons.time_end.year;
			in >> cons.time_end.month;
			in >> cons.time_end.day;
			in >> cons.time_end.hour;
			in >> cons.time_end.minute;
			in >> cons.time_end.second;
		}
		else if(buffer == "temperature:")
		{
			in >> cons.loc.temperature;
		}
		else if(buffer == "pressure:")
		{
			in >> cons.loc.pressure;
		}
		else if(buffer == "latitude:")
		{
			in >> cons.loc.latitude;
			cons.loc.latitude /= R2D;
		}
		else if(buffer == "longitude:")
		{
			in >> cons.loc.longitude;
			cons.loc.longitude /= R2D;
		}
		else if(buffer == "b1x:")
		{
			in >> cons.b_1.min_x;
			in >> cons.b_1.max_x;
		}
		else if(buffer == "b1y:")
		{
			in >> cons.b_1.min_y;
			in >> cons.b_1.max_y;
		}
		else if(buffer == "b1z:")
		{
			in >> cons.b_1.min_z;
			in >> cons.b_1.max_z;
		}
		else if(buffer == "b2x:")
		{
			in >> cons.b_2.min_x;
			in >> cons.b_2.max_x;
		}
		else if(buffer == "b2y:")
		{
			in >> cons.b_2.min_y;
			in >> cons.b_2.max_y;
		}
		else if(buffer == "b2z:")
		{
			in >> cons.b_2.min_z;
			in >> cons.b_2.max_z;
		}
		else if(buffer == "b3x:")
		{
			in >> cons.b_3.min_x;
			in >> cons.b_3.max_x;
		}
		else if(buffer == "b3y:")
		{
			in >> cons.b_3.min_y;
			in >> cons.b_3.max_y;
		}
		else if(buffer == "b3z:")
		{
			in >> cons.b_3.min_z;
			in >> cons.b_3.max_z;
		}
		else if(buffer == "floor_p1:")
		{
			in >> cons.floor.p1.x;
			in >> cons.floor.p1.y;
			in >> cons.floor.p1.z;
		}
		else if(buffer == "floor_p2:")
		{
			in >> cons.floor.p2.x;
			in >> cons.floor.p2.y;
			in >> cons.floor.p2.z;
		}
		else if(buffer == "floor_p3:")
		{
			in >> cons.floor.p3.x;
			in >> cons.floor.p3.y;
			in >> cons.floor.p3.z;
		}
		else if(buffer == "floor_p4:")
		{
			in >> cons.floor.p4.x;
			in >> cons.floor.p4.y;
			in >> cons.floor.p4.z;
		}

		else if(buffer == "rate_shad:")
		{
			in >> cons.rate_shad;
		}

		if(time(nullptr) - t_begin > 1)
		{
			std::cerr << "Constraint file probably ill-formed." << std::endl;
			std::cerr << "Break" << std::endl;
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& out, const Configs& conf)
{
	out << "# CONFIGS" << std::endl << "#" << std::endl;

	out << "# SELECT :" << std::endl;
	for(unsigned int i(0); i < conf.allSelect.size(); ++i)
		out << "# " << i << " = " << conf.nameSelect[i] << std::endl;
	out << "#" << std::endl;

	out << "# CROSS :" << std::endl;
	for(unsigned int i(0); i < conf.allCross.size(); ++i)
		out << "# " << i << " = " << conf.nameCross[i] << std::endl;
	out << "#" << std::endl;

	out << "# INSERT :" << std::endl;
	for(unsigned int i(0); i < conf.allInsert.size(); ++i)
		out << "# " << i << " = " << conf.nameInsert[i] << std::endl;
	out << "#" << std::endl;

	out << "# MUTATION :" << std::endl;
	for(unsigned int i(0); i < conf.allMutation.size(); ++i)
		out << "# " << i << " = " << conf.nameMutation[i] << std::endl;
	out << "#" << std::endl;

	{
		out << "# EXAMPLE :" << std::endl;
		int s(rand()%conf.allSelect.size());
		int c(rand()%conf.allCross.size());
		int i(rand()%conf.allInsert.size());
		int m(rand()%conf.allMutation.size());

		out << "# " << s << c << i << m << " means :" << std::endl << "#" << std::endl;
		out << "# " << s << " = " << conf.nameSelect[s] << std::endl;
		out << "# " << c << " = " << conf.nameCross[c] << std::endl;
		out << "# " << i << " = " << conf.nameInsert[i] << std::endl;
		out << "# " << m << " = " << conf.nameMutation[m] << std::endl;
	}

	out << "#" << std::endl;
	out << "# STAT = " << conf.STAT << std::endl;
	out << "# ITMAX = " << conf.ITMAX << std::endl;
	out << "# SEED = " << conf.seed << std::endl << std::endl;

	return out;
}

std::ostream& operator<<(std::ostream& out, const Constraints& cons)
{
	out << "# CONSTRAINTS" << std::endl << "#" << std::endl;
	out << "# BEGIN TIME:" << std::endl;
	out << "# YEAR: " << cons.time_beg.year << std::endl;
	out << "# MONTH: " << cons.time_beg.month << std::endl;
	out << "# DAY: " << cons.time_beg.day << std::endl;
	out << "# HOUR: " << cons.time_beg.hour << std::endl;
	out << "# MINUTE: " << cons.time_beg.minute << std::endl;
	out << "# SECOND: " << cons.time_beg.second << std::endl;
	out << "#" << std::endl;
	out << "# END TIME:" << std::endl;
	out << "# YEAR: " << cons.time_end.year << std::endl;
	out << "# MONTH: " << cons.time_end.month << std::endl;
	out << "# DAY: " << cons.time_end.day << std::endl;
	out << "# HOUR: " << cons.time_end.hour << std::endl;
	out << "# MINUTE: " << cons.time_end.minute << std::endl;
	out << "# SECOND: " << cons.time_end.second << std::endl;
	out << "#" << std::endl;
	out << "# TEMPERATURE: " << cons.loc.temperature << std::endl;
	out << "# PRESSURE: " << cons.loc.pressure << std::endl;
	out << "# LATITUDE: " << cons.loc.latitude << std::endl;
	out << "# LONGITUDE: " << cons.loc.longitude << std::endl;
	out << "#" << std::endl;
	out << "# BOUNDARY1: ([" << cons.b_1.min_x << ";" << cons.b_1.max_x << "],[" << cons.b_1.min_y << ";" << cons.b_1.max_y << "],[" << cons.b_1.min_z << ";" << cons.b_1.max_z << "])" << std::endl;
	out << "# BOUNDARY2: ([" << cons.b_2.min_x << ";" << cons.b_2.max_x << "],[" << cons.b_2.min_y << ";" << cons.b_2.max_y << "],[" << cons.b_2.min_z << ";" << cons.b_2.max_z << "])" << std::endl;
	out << "# BOUNDARY3: ([" << cons.b_3.min_x << ";" << cons.b_3.max_x << "],[" << cons.b_3.min_y << ";" << cons.b_3.max_y << "],[" << cons.b_3.min_z << ";" << cons.b_3.max_z << "])" << std::endl;
	out << "#" << std::endl;
	out << "# FLOOR1: (" << cons.floor.p1.x << "," << cons.floor.p1.y << "," << cons.floor.p1.z << ")" << std::endl;
	out << "# FLOOR2: (" << cons.floor.p2.x << "," << cons.floor.p2.y << "," << cons.floor.p2.z << ")" << std::endl;
	out << "# FLOOR3: (" << cons.floor.p3.x << "," << cons.floor.p3.y << "," << cons.floor.p3.z << ")" << std::endl;
	out << "# FLOOR4: (" << cons.floor.p4.x << "," << cons.floor.p4.y << "," << cons.floor.p4.z << ")" << std::endl;
	out << std::endl;

	return out;
}








