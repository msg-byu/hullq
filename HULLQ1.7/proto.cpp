#include "proto.h"


proto::proto(string in)
{
	string rawdata = in;
	vector <string> data;

	//POSSIBLE FIX IF WE CANT USE REGEX
	/*stringstream rawdata, temp;
	string split1, split2;
	rawdata << in;

	while (getline(rawdata, split1, ':')){
		temp << split1;
		while (getline(temp, split2, ',')){
			data.push_back(split2);
		}
	}*/

	regex e("[^:, ]*");  

	regex_iterator<string::iterator> rit(rawdata.begin(), rawdata.end(), e);
	regex_iterator<string::iterator> rend;

	while (rit != rend) {
		if (!rit->str().empty()){
			data.push_back(rit->str());
		}
		++rit;
	}
	//Proto|name|last n-1 points from concentration|enthalpy of formation|entropic temp

	vector<string>::iterator d;
	int n = data.size() - 9; // this will give you the number of dimensions, i.e. how  many points to look for. It can break if extra delimters are found.
	for (d = data.begin(); d != data.end(); ++d){
		if (*d == "Proto"){
			prototype = *(d + 1);
		}
		else if (*d == "Name"){
			name = *(d + 1);
		}
		else if (*d == "Stoichiometry"){
			for (int i = 1; i <= n; i++){ 
			  //	string currentstr = *(d + i);
			  //	double currentnum = stod(*(d + i));
				concentration_coordinates.push_back(stod(*(d + i))*100);
			}
		}
		else if (*d == "FormEnthalpyAtom"){
		  enthalpy = stod(*(d + 1))*1000;
		}
		else if (*d == "EntropicTemp"){
			ent_temp = stod(*(d + 1));
		}
		else if (*d == "WARNING"){
			throw invalid_argument("Bad input at line ");
		}
		else if (*d == "LDAU2"){
		  throw invalid_argument("Bad input at line ");
                }


	}
	orig_coor = proto_val();
	
	/*
	for(auto p: orig_coor)
	  {cout << p << " " ;
	  }
	  cout << "\n";*/

}

vector <double> proto::proto_val(){
	vector <double> result = concentration_coordinates;
	result.erase(result.begin());
	result.push_back(enthalpy);
	return result;
}

void proto::setVals(vector <double> newVals){
	vector <double> vals = newVals;
	enthalpy = newVals.back();
	for (unsigned int i = 1; i < concentration_coordinates.size(); i++){
		concentration_coordinates.at(i) = vals.at(i - 1);
	}
}

void proto::setDistance(double d){
	distance_to_hull = d;
}

string proto::getProto(){
	return prototype;
}

string proto::getName(){
	return name;
}

string proto::outputVals(bool bt){
  string result;
  // = "{";
	if (bt){
		for (auto elem : orig_coor){
		  result += to_string(elem);

		  if (elem != orig_coor.back()){
		    result += " ";
		  }

		}
		return result;
	}
	else{
		for (auto elem : concentration_coordinates){
			if (elem != concentration_coordinates.front()){
				result += to_string(elem) + " ";
			}
		}
		return result + to_string(enthalpy);
	}
}

double proto::getDistance(){
	return distance_to_hull;
}

proto::~proto()
{
}
