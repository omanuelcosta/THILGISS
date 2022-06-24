#include <string>
#include "gnuplot-iostream.h"
/**
* @brief Class vect_film(n, th), vectorizes all film properties
* @return instance with the read props in input file (
	cs <string> | elist() <vector<strings>> | th <double> => thickness | ctype <string> | cparam () <vector<double>> | steps <int> | gangles() <vector<double>>)
**/
class vect_film {
public:

	void display() {
		std::cout << "This is film class" << std::endl;
	}
};

/**
* @brief Class InputReader (Input reader), reads the inputs of a file
* @return instance with the read props in input file (
	cs <string> | elist() <vector<strings>> | th <double> => thickness | ctype <string> | cparam () <vector<double>> | steps <int> | gangles() <vector<double>>)
**/
class InputReader {
private:
	// filename
	std::string f;
	std::vector<std::string> fn_splt;

	std::vector<std::string> ext_props() {
		/*
		** @brief list of strings containing file inputs
		*/
		std::vector<std::string> props;
		// iterator
		int i = 0;
		// filename line
		static std::string fileline;
		// file object
		std::ifstream input_file;
		// variables storage
		std::vector<std::string> fcontents;
		// open file
		input_file.open(f);

		if (input_file.is_open()) {

			while (!input_file.eof()) {

				std::getline(input_file, fileline);

				if (fileline.find('#', 0) == 0) {
					continue;
				}
				props.push_back(fileline);
				// cout << fileline << endl;
				i += 1;
			}
			input_file.close(); //closing the file

		}
		else {
			std::cout << "Unable to open file " << f;
		}; //if the file is not open output

		return props;
	};
public:
	// constructor
	InputReader(std::string f) : f(f), fn_splt() {
		fn_splt = splitString(f, '.');
	}
	/*
	** @brief displays name of file
	* @returns cout << running job _filename_ << endl
	*/
	void display_fn() {
		std::cout << "running job " << fn_splt[0] << "." << fn_splt[1] << "..." << std::endl;
	};

	// crystal structure
	std::string cs = ext_props()[0];

	// element list
	std::vector<std::string> elist = elister();
	std::vector<std::string> elister() {
		std::string list = ext_props()[1];
		std::vector<std::string> a;

		a = splitString(list, ' ');
		return a;
	};
	// film thickness
	double th = std::stod(ext_props()[2]);

	// concentration type def
	std::string ctype = ext_props()[3];

	// ctype parameter list
	std::vector<double> cparams = cparamtr();
	std::vector<double> cparamtr() {
		int i;
		std::vector<double> cparams;
		std::vector<std::string> list = splitString(ext_props()[4], ' ');
		for (i = 0; i < list.size(); ++i) {
			cparams.push_back(std::stod(list[i]));
		};
		return cparams;
	}

	// steps
	double steps = std::stod(ext_props()[5]);

	// GIXRD angles
	std::vector<double> gangles() {
		int i;
		std::vector<std::string> langles = splitString(ext_props()[6], ' ');
		std::vector<double> a;

		for (i = 0; i < langles.size(); ++i) {
			a.push_back(std::stod(langles[i]));
		}
		return a;
	}
};

class CompositionVectorizer {
private:
	std::vector<std::string> elist;
	int steps;
	std::string ctype;
	std::vector<double> cparams;
	double th;

public:
	// constructor
	CompositionVectorizer(const InputReader& T) {
		elist = T.elist;
		steps = T.steps;
		ctype = T.ctype;
		cparams = T.cparams;
		th = T.th;
	};

	void display() {
		int i;
		for (i = 0; i < compvec().size(); ++i) {
			std::cout << " " << i << " " << compvec()[i] << std::endl;
		}
	};

	std::vector<double> ycoord() {
		double pos = 0;
		int i;
		std::vector<double> yposvec;
		for (i = 0; i < steps; ++i) {
			pos += th / steps;
			// std::cout << pos << " " << th/steps <<  " " << i << std::endl;
			yposvec.push_back(pos);
		};
		return yposvec;
	}

	/**
	* @brief function compvec (Input reader), parse input ctype and cparams
	* @return vector with the given %at fraction for the first, second and n elements
	**/

	int display_req(std::string ctype, int req, std::vector<double> cparams) {
		if (cparams.size() != req) {
			std::cout << "required inputs for " << ctype << ": " << req << ", input: " << cparams.size() << std::endl;
			throw std::invalid_argument("not the same ammount of needed parameters");
		}
		else {
			return 0;
		}
	};

	void plot(std::string gnupath){
		int i;
		Gnuplot gp(gnupath);
		std::cout << gnupath << std::endl;

		std::string outputdir = ".temp";
		std::string file = "compvec.dat";
		std::ofstream composition(".temp/compvec.dat");

		std::vector<double> ycoord = CompositionVectorizer::ycoord();
		std::vector<double> compvec = CompositionVectorizer::compvec();
		std::vector< std::vector<double> > pts;

		if (CreateDirectory(outputdir.c_str(), NULL)|| ERROR_ALREADY_EXISTS == GetLastError())
		{
			if (composition.is_open()) {
				std::vector< double > xy;
				for (i = 0; i < ycoord.size(); i++) {
					switch (i) {
					case 0:
						composition << "depth" << "\t" << "ratio" << "(" << elist[0] << "/" << elist[1] << ")" << std::endl;
						composition << ycoord[i] << "\t" << compvec[i] << std::endl;
						xy = { ycoord[i], compvec[i] };
						break;
					default:
						composition << ycoord[i] << "\t" << compvec[i] << std::endl;
						xy = { ycoord[i], compvec[i] };
					};

					pts.push_back(xy);
				}
			}
			else { std::cout << "can't output to " << outputdir << std::endl; }
			composition.close();
		}
		else
		{
			std::cout << "failed to create dir. Check for admin rights" << std::endl;
		};
		gp << "set ylabel \"" << elist[0] << "/" << elist[1] << "\"\n";
		//gp.send(elist[0]).send(elist[1]);
		gp << "set xlabel \"depth\" \n";
		gp << "plot '-' with lines \n";
		
		gp.send1d(pts);

		//gp.send(compvec);

		std::cout << "Plotting composition gradient, press Enter to continue" << std::endl;
		std::cin.get();
	}
	
	std::vector<double> compvec() {
		try {
			if (elist.size() == 2) {
				if (ctype == "BOLTZMANN") {
					int req = 4;
					display_req(ctype, req, cparams);
					return BOLTZMAN(steps, ycoord(), cparams[0], cparams[1], cparams[2], cparams[3]);
				};
				if (ctype == "LINEAR") {
					int req = 2;
					display_req(ctype, req, cparams);
					return LINEAR(steps, ycoord(), cparams[0], cparams[1]);
				}
				else {
					throw InputNoNameType(ctype);
					std::cout << ctype << std::endl;
					exit(0);
				}
			}
		}
		catch (InputNoNameType& ctype) {
			std::cout << "Input error: No composition module named " << ctype.what();
			exit(0);
		}
	};

	std::vector<std::vector<double>> compvecat() {
		std::vector<std::vector<double>> compvecat(elist.size(), std::vector<double>(steps,0));

		std::vector<double> a;
		int i;

		try{
			if (elist.size() == 2) {
				if (ctype == "BOLTZMANN") {
					int req = 4;
					display_req(ctype, req, cparams);
					a = BOLTZMAN(steps, ycoord(), cparams[0], cparams[1], cparams[2], cparams[3]);
					for (i = 0; i < a.size(); ++i) {
						compvecat[0][i] = a[i] / (a[i] + 1.0);
						compvecat[1][i] = (1. - (a[i] / (a[i] + 1.0)));
					};
					return compvecat;
				};
				if (ctype == "LINEAR") {
					int req = 4;
					display_req(ctype, req, cparams);
					a = LINEAR(steps, ycoord(), cparams[0], cparams[1]);
					for (i = 0; i < a.size(); ++i) {
						compvecat[0][i] = a[i] / (a[i] + 1.0);
						compvecat[1][i] = (1. - (a[i] / (a[i] + 1.0)));
					};
					return compvecat;
				}
				else {
					throw InputNoNameType(ctype);
				}
			}
			else {
				int err = elist.size();
				throw InputNotSupported(err);
			}
		}
		catch (InputNotSupported& err){
			std::cout << "Input error: not supported for " << err.what() << " elements" ;
			exit(0);
		}
		catch (InputNoNameType& ctype) {
			std::cout << "Input error: No composition module named " << ctype.what();
			exit(0);
		}
	};

	std::vector<double> BOLTZMAN(int steps, std::vector<double> X, double A1, double A2, double X0, double DX) {
		std::vector<double> comp_vec;
		int i;
		double c;
		for (i = 0; i < steps; i++) {
			c = (A2 + (A1 - A2)) / (1. + exp((X[i] - X0) / DX));
			comp_vec.push_back(c);
		}
		return comp_vec;
	};

	std::vector<double> LINEAR(int steps, std::vector<double> X, double Y0, double A) {
		std::vector<double> comp_vec;
		int i;
		double c;
		for (i = 0; i < steps; i++) {
			c = Y0 + (A * X[i]);
			comp_vec.push_back(c);
		}
		return comp_vec;
	}
};

class DensityVectorizer {
private:
	std::vector<std::string> elist;
	int steps;
	std::string ctype;
	std::vector<double> cparams;
	double th;
public:
	DensityVectorizer(const InputReader& T) {
		elist = T.elist;
		steps = T.steps;
		ctype = T.cs;
		cparams = T.cparams;
		th = T.th;
	};

	std::vector<double> rho() {
		
		return { 0 };
	}

	std::vector<double> cellvol() {
		return { 0 };
	}

	std::vector<double> cellwt(){
		return { 0 };
	}

};

class Element{
private:
	int i;
	std::string fileline;
	std::vector<std::string> all,props;
	std::tuple<double, double, double, std::string> proptuple;
	std::string yml_path = "data/elements.yaml";
public:
	double r, ri, w;
	std::string elsymb, ss;

	Element(std::string Symb) : elsymb(Symb) {

		std::ifstream data;

		data.open(yml_path);
		if (data.is_open()) {
			while (!data.eof()) {
				std::getline(data, fileline);
				all.push_back(fileline);
				//std::cout << el << std::endl;
			};
			data.close();
		}
		else {
			std::cout << "Unable to open file " << yml_path;
		};

		for (i = 0; i < all.size(); ++i) {
			if (all[i] == "  symbol: " + elsymb) {
				props.push_back(all[i + 2]); // w
				props.push_back(all[i + 6]); // r
				props.push_back(all[i + 7]); // ri
				props.push_back(all[i + 11]); // ss
			};
		}

		proptuple = ElementPropParser(props[0], props[1], props[2], props[3]);

		w = std::get<0>(proptuple);
		r = std::get<1>(proptuple);
		ri = std::get<2>(proptuple);
		ss = std::get<3>(proptuple);

		std::cout << "w=" << w << " r=" << r << " ri=" << ri << " ss=" << ss << std::endl;
	}
};