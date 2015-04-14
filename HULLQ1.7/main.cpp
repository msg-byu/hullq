#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include "hull.h"
#include "proto_data.h"
using namespace std;

map <string, string> ArgParse(int argc, char* argv[]){
	//This function builds a map of the arguments from the command line.
	map <string, string> options;
	for (int arg = 0; arg < argc; arg++){
		string currentArg = argv[arg];
		if (currentArg.front() == '-'){
			try{
				if (arg + 1 >= argc){
					throw - 1; // if it would produce a null pointer throw;
				}
				string nextArg = argv[arg + 1];
				if (nextArg.front() != '-'){
					options.insert(pair<string, string>(currentArg, nextArg));
				}
				else {
					options.insert(pair<string, string>(currentArg, "TRUE"));
				}
			}
			catch (int e){
				options.insert(pair<string, string>(currentArg, "TRUE"));
			}
		}
	}
	return options;
}

string outputProto(proto p){
	string output;
	stringstream out;
	out.setf(ios::scientific);
	out.precision(10);
	out <<  "Proto: " <<  p.getProto() <<  "  Name: " <<  p.getName() <<  "  BT: " << p.outputVals(true) <<  "  AT: " <<  p.outputVals(false) << "  Distance: " << p.getDistance() << "\n";
	return out.str();
}



int main(int argc, char* argv[]){
	map <string, string> options = ArgParse(argc, argv);
	/*	cout << "-------------------------\n"
		 << "     HULLQ\n"
		 << "-------------------------\n"
		 << "    Version 1.2\n"	
		 << "    Created by BYU MSG\n"
	         << "    Authors: Jacob Hansen, Chandramouli Nyshadham"
	         << "\n-------------------------\n";
	*/
	
	/*  
	(1)--------------------------
	Duke Code (copy data from aflow library) actually we need to rewrite this...
	-> produces ABC_data.txt where A,B,C are elements in alphabetical order
	--------------------------

	j	(2)Then parse the ABC_data.txt to look like this:
	Poto|name|last n-1 points from concentration|enthralpy of formation|entropic temp
	-also keep track of the total number of lines

	(3)Generate n-Dimentional simplex coordinates. Then change them to concentration space. Collect coordinate data from
	step (2) and do matrix multiplication with the simplex matrix in concentration space.
	between step (2) and the beginning of step (3)
		->outputs concentration_coordinates.in and std.in for qhull.

	(4) Run qhull
	-> output from qhull using the n option in qhull
	-> this makes normals.in
	-> if dimentions = 2 or 3 allow mathematic option and store mathematica file for gnuplot.

	(5) Create the convex hull(c++ object) with normals.in
	if option -gnu and 2 or 3 dimentions plot using gnuplot.

	(6) Calculate the distance of each structure in concentration_coordinates.in
	if option -p only calculate the distance to [proto]

	(7) output distances
	if -o save this output as [ABC_dist.out]

	final output is ABC_dist.out and gnuplot if asked.

	*/

	if (options.count("-h") || options.count("--help")){
		string helpText = "Usage: hullq -in [infile] [options]"
			"\n\nElements for the [infile] should only be given in alphabetical order."
			"\n\nOptions:"
			"\n-in [infile], used to specify a particular infile."
			"\n-p [proto], calculate only the distance of proto."
			"\n-o Output to file, file will be named [infile].out."
		        "\n-v Outputs the distances of the vertices of the convex hull to the file VD_[infile].out."
			"\n-t [tolerance], option to specify tolerance of the distance of a point to the convex hull. If none, gives all points."
			"\n-gnu, Make the gnu plot of the convex hull."
			"\n-mat, Gives the mathematica output of the convex hull."
		        "\n-gnu, Makes a gnuplot of ternary system. The output will be given as gnu_[infile].pdf"
			"\n-h or --help, display these options.\n\n";
		cout << helpText;
	}
	else if (options.count("-in"))  {
	  //	cout << "working...\n";

		
		string infile = options.at("-in");
		string outfile = (infile.substr(0,infile.size()-3) + ".out");
 
		double tolerance = -1;
		if (options.count("-t")){
			tolerance = stod(options.at("-t"));
		}

		proto_data database = proto_data(infile);
		cout << "Database created.\n";
		vector <proto> myProtos = database.GetProtos();
	
		if (options.count("-p")){
			string proto = options.at("-p");
			for (auto p : myProtos){
				if (p.getProto() == options.at("-p") || p.getName() == options.at("-p")){
					cout << outputProto(p);
				}
			}
		}

		if (options.count("-mat")){
		  string temp = ("cat stdin | ./qhull m >> M_" + outfile);
		  system(temp.c_str());
		}		

		if (options.count("-gnu")){
		  bool fixedTolerance = false;
		  if(tolerance == -1){
		    tolerance = 0.00005;
		    fixedTolerance = true;
		    cout << tolerance ;
		  }
		  ofstream gnuOut;
		  gnuOut.open("gnu_"+outfile);
		  for (auto p : myProtos){
		    if (p.getDistance() <= tolerance){
		      gnuOut << p.getProto() + " " + p.getName() + " " + p.outputVals(false) + " " + to_string(p.getDistance()) + "\n";
		    }
		  }
		  gnuOut.close();
		  tolerance = fixedTolerance ? -1 : tolerance;
		}

		if (options.count("-v")){
		  ofstream VOut;
		  string temp;
 		  temp=("VD_" + outfile);
		  VOut.open(temp);
		  VOut << database.vertexOut();
		  VOut.close();
		}

		if (options.count("-o")){
			ofstream OutFile;
			OutFile.open(outfile);
			for (auto p : myProtos){
				if (tolerance != -1){ // if tolerance is specified only output the the ones within the tolernce. 
					if (p.getDistance() <= tolerance){
						OutFile << outputProto(p);
					}
				}else{
				  OutFile << outputProto(p);
				}
			}
			OutFile.close();
			//	cout << "Successfully created output at: " << options.at("-o") << "\n";
		} else {
			vector <proto> myProtos = database.GetProtos();
			for (auto p : myProtos){
			  cout << outputProto(p);
			}
		} 
		
		ofstream allPointsOut;
		string temp = ("AP_" + outfile);
	    	allPointsOut.open(temp);
		for(auto p : myProtos){
		  allPointsOut << p.outputVals(false) << "\n";
		}
		allPointsOut.close();
		//	system("rm stdin");
	} else {
		cout << "Please specify an input file.\n";
	}

	
	//system("pause");
}


