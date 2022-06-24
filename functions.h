std::vector<double> LinearSpacedArray(double a, double b, std::size_t N) {
	double h = (b - a) / static_cast<double>(N - 1);
	std::vector<double> xs(N);
	std::vector<double>::iterator x;
	double val;
	for (x = xs.begin(), val = a; x != xs.end(); ++x, val += h) {
		*x = val;
	}
	return xs;
};

double MonoElemVolume(char* cell, char* el, double a, double b, double c) {

	if (cell == "FCC") {
		return 0;
	}

};


double Interpd(std::vector<double>& xData, std::vector<double>& yData, double x, bool extrapolate) {
	int size = xData.size();

	int i = 0;
	if (x >= xData[size - 2]) {
		i = size - 2;
	}
	else {
		while (x > xData[i + 1]) i++;
	}
	double xL = xData[i], yL = yData[i], xR = xData[i + 1], yR = yData[i + 1];      // points on either side (unless beyond ends)
	if (!extrapolate) {
		if (x < xL) yR = yL;
		if (x > xR) yL = yR;
	}
	double dydx = (yR - yL) / (xR - xL);                                    // gradient
	return yL + dydx * (x - xL);                                              // linear interpolation
};

std::vector<std::string> splitString(std::string str, char splitter) {
	std::vector<std::string> result;
	std::string current = "";
	for (int i = 0; i < str.size(); i++) {
		if (str[i] == splitter) {
			if (current != "") {
				result.push_back(current);
				current = "";
			}
			continue;
		}
		current += str[i];
	}
	if (current.size() != 0)
		result.push_back(current);
	return result;
};

std::tuple<double, double, double, std::string> ElementPropParser(std::string prop0, std::string prop1, std::string prop2, std::string prop3) {
	std::string t;
	double r, ri, w;
	std::string ss;

	std::istringstream propline0(prop0);
	propline0 >> t >> w;
	std::istringstream propline1(prop1);
	propline1 >> t >> r;
	std::istringstream propline2(prop2);
	propline2 >> t >> ri;
	std::istringstream propline3(prop3);
	propline3 >> t >> ss;
	return std::make_tuple(w, r, ri, ss);
};

 bool in_array(const std::string &value, const std::vector<std::string> &array){
     return std::find(array.begin(), array.end(), value) != array.end();
 };
