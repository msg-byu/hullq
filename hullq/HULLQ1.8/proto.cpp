#include "proto.h"


proto::proto(string in)
{
  //  Its the object that holds data for each prototype read from the input file.
	string rawdata = in;
	vector <string> data;

	regex e("[^:, ]*");  

	regex_iterator<string::iterator> rit(rawdata.begin(), rawdata.end(), e);
	regex_iterator<string::iterator> rend;

	while (rit != rend) {
		if (!rit->str().empty()){
			data.push_back(rit->str());
		}
		++rit;
	}
	// This is how it expects to see input data.
	//Proto|name|last n-1 points from concentration|enthalpy of formation|entropic temp

	vector<string>::iterator d;
	int n = data.size() - 9; // this will give you the number of dimensions, i.e. how  many points to look for. It can break if extra delimters are found.
	//This 'n' declared above works good for the RESTAPI input format. 
	for (d = data.begin(); d != data.end(); ++d){
		if (*d == "Proto"){
			prototype = *(d + 1);
		}
		else if (*d == "Name"){
			name = *(d + 1);
		}
		else if (*d == "Stoichiometry"){
		  for (int i = 1; *(d + i) != "FormEnthalpyAtom"; i++){ 
			 concentration_coordinates.push_back(stod(*(d + i))*100);
			 n=i;   // This assures it n-nary case.     
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
	
		// We can add more depending on the type of errors we encounter in generating input file.

	}
	orig_coor = proto_val();
	
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
  // To string method to convert protos to strings. set 'bt' to true if you want to display before transformation results else it displays after transformation coordinates. 
  string result;
	if (bt)
	  {
	    result=  to_string(orig_coor.at(0)) + " " + to_string(orig_coor.at(1)) + " " + to_string(orig_coor.at(2));
	    return result;
	  }
	else
	  {
	    result = to_string(concentration_coordinates.at(1)) + " " + to_string(concentration_coordinates.at(2))  + " " +  to_string(enthalpy);
	    return result;
	  }
}



/*
string proto::outputVals(bool bt){
  // To string method to convert protos to strings. set 'bt' to true if you want to display before transformation results else it displays after transformation coordinates.                                                                  string result;
  if (bt)
    {
      // return vectordouble2string(orig_coor);                                                                                                                                                                                         
      //for (auto elem : orig_coor){                                                                                                                                                                                                  
      // result += to_string(elem);                                                                                                                                                                                                   
      // if (elem != orig_coor.back()){                                                                                                                                                                                               
      //  result += " ";                                                                                                                                                                                                              
      // }                                                                                                                                                                                                                            
      //}                                                                                                                                                                                                                             
      result=  to_string(orig_coor.at(0)) + " " + to_string(orig_coor.at(1)) + " " + to_string(orig_coor.at(2));
      return result;
    }
  else
    {
      //    return vectordouble2string(concentration_coordinates);                                                                                                                                                                    
      //    for (auto elem : concentration_coordinates)                                                                                                                                                                               
      // {                                                                                                                                                                                                                            
      //result += to_string(elem);                                                                                                                                                                                                      
      //cout << elem << " " ;                                                                                                                                                                                                           
      //if (elem != concentration_coordinates.back()){                                                                                                                                                                                  
      //  result += " ";                                                                                                                                                                                                                
      //}                                                                                                                                                                                                                               
      //    if (elem != concentration_coordinates.front()){                                                                                                                                                                           
      //result += to_string(elem) + " ";                                                                                                                                                                                              
      //}                                                                                                                                                                                                                             
      //}                                                                                                                                                                                                                             
      //return result;                                                                                                                                                                                                                  

      result = to_string(concentration_coordinates.at(1)) + " " + to_string(concentration_coordinates.at(2))  + " " +  to_string(enthalpy);

      //cout << "Orig" <<  " " <<  orig_coor.at(0) << " " << orig_coor.at(1) << " " << orig_coor.at(2) << " " << "Conc" << concentration_coordinates.at(0) << " " << concentration_coordinates.at(1) << " " << concentration_coordinates.at(2)  << " " <<  to_string(enthalpy) << "\n";                                                                                                                                                                                           //return result + to_string(enthalpy);                                                                                                                                                                                            
      return result;
    }
}
*/

double proto::getDistance(){
	return distance_to_hull;
}


double proto::getEnthalpy(){
  return enthalpy;
}



proto::~proto()
{
}
