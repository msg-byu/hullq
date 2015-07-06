#include "proto_data.h"

proto_data::proto_data(string filename)
{
  // Its running all the logic here. 

	int nProtos = createProtos(filename);
	//	cout << "\nInput file read.\n" << nProtos << " protos were successfully added.\n";

	simplex_transform();
	//	cout << "Simplex transformation completed.\n";

	createQhullInput();
	//	cout << "qhull input created.\n";
	
	system("rm normals.in");
	// This is where we want to integrate qhull code to our code. 

	 createQhullInput();
	string sysCommand = "echo \"" + createQhullInput() + "\" >> stdin";
	system(sysCommand.c_str()); 
	
	
	system("cat stdin | ./qhull n >> normals.in"); 


	myHull.buildHull("normals.in");
	//	cout << "Convex hull built.\n";

	find_dist_to_hull();
	find_vertex_to_hull();
	//	cout << "Distances calculated.\n";
}

int proto_data::createProtos(string filename){
	//function: opens and reads the file from which hullq then adds protos to the database. 
	//returns: the number of protos  in the database after the read.
	ifstream myFile;
	myFile.open(filename);
	string line;
	int nline = 0;
	int nerrors = 0;
	while (getline(myFile, line, '\n')){
		try {
		  
		  proto new_proto = proto(line);
		  
		  if(new_proto.getEnthalpy() > -1000.0){
		    database.push_back(new_proto);
		  }
		}
		catch (invalid_argument& e){
		  cout << "\n" << e.what() << nline;
		  nerrors++;
		}
		nline++;
	}
	myFile.close();
	return database.size();
}

string proto_data::createQhullInput(){
	//function: creates the stdin file that qhull uses to calculate the normals for the planes in the convex hull.
	stringstream temp;
	temp.precision(15);  // Always need 15 digit precision to avoid conflicts with qhull. 
	temp.setf(ios::fixed);
	int n = data_conc_at.at(0).size();
	int linesKept = 0;
	for (unsigned int p = 0; p < data_conc_at.size(); p++){
		vector<double> current = data_conc_at.at(p);
		database.at(p).setVals(current);
		if (current.back() <= 0){    // We want only negative enthalpies in stdin file. 
			vector<double>::iterator elem;
			for (elem = current.begin(); elem != current.end(); elem++){
				double currentnum = *elem;
				temp << *elem << " ";
				string currentstring = temp.str();

			}
			temp << "\n";
			linesKept++;   // this keeps track of number of lines in stdin file
		}
	}
	return to_string(n) + "\n" + to_string(linesKept + n) + "\n" + firstPlane(n-1) + temp.str(); 
}

void proto_data::simplex_transform(){
   // This generates simplex coordinates as per the wikipedia article : http://en.wikipedia.org/wiki/Simplex 
  // Go to section : Cartesian coordinates for regular n-dimensional simplex in Rn in wiki page. The same logic is implemented here

	double *Simplex_cartesian;
	double *Simplex_concentration;
	double *Trans_Mat; // (n+1,n+1) matrix 
	vector <vector <double> > data_conc_bt = dataout(); //list of (n+1,1) matricies
	vector <vector <double> > transpose_matrix; // this is the transpose of Trans_Mat
	int n = data_conc_bt.at(0).size() - 1;

	Simplex_cartesian = Simplex_Cart_Coord(n);
	Simplex_concentration = Simplex_Conc_Coord(Simplex_cartesian, n);
	Trans_Mat = Transformation_Matrix(Simplex_concentration, n);

	vector <double> temp; // transpose the simplex matrix
	for (int row = 0; row < n + 1; row++){
		for (int col = 0; col < n + 1; col++){
			temp.push_back(Trans_Mat[row + col*(n + 1)]);
			//cout << Trans_Mat[row + col*(n + 1)] << " ";
		}
		//cout << "\n";
		transpose_matrix.push_back(temp);
		temp.clear();
	}

	for (auto p : data_conc_bt){ //do the matrix multiplication between each point in data_conc_bt and the transpose_matrix
		for (auto row : transpose_matrix){
			int i = 0;
			double result = 0;
			for (auto elem : row){
				result += elem*p.at(i);
				i++;
			}
			temp.push_back(result);
		}
		data_conc_at.push_back(temp);
		temp.clear();
	}

}

void proto_data::find_dist_to_hull(){
	double distance;
	for (unsigned int p = 0; p < database.size(); p++){
		distance = myHull.findDistance(database.at(p).proto_val());
		database.at(p).setDistance(distance);
	}
}

void proto_data::find_vertex_to_hull(){
	// Test function that allowed us to check the distances from the qhull verticies to the qhull hull. Answer should be 0.
  system("cat stdin | ./qhull p >> vertices.in");  
  stringstream  myVertices;
  double distance;
  int count = 0;
  vertexCheck("vertices.in");
  /*
	for (unsigned int p = 0; p < vertexPoints.size(); p++){
		distance = myHull.findDistance(vertexPoints.at(p));
		//	myVertices << "Point: ";
		for (unsigned int elem = 0; elem < vertexPoints.at(p).size(); elem++){
		  if (elem != vertexPoints.at(p).size()){
		    myVertices << vertexPoints.at(p).at(elem) << " ";
			}
			else {
			  myVertices << vertexPoints.at(p).at(elem);
			}
		}
		//	myVertices << " Distance: " << distance << "\n";
		count++;
		}   */

	//myVertices << "Found " << count << " point(s) on the convex hull.\n";
	system("rm vertices.in");
	cout<<myVertices.str();
	VDoutput = myVertices.str();
}

void proto_data::vertexCheck(string filename){
	// part of the find_vertex_to_hull() test function. Purpose is to read in and store the verticies. 
	ifstream myFile;
	myFile.open(filename);
	string token, line;
	stringstream temp;
	vector <double> rawPoints;

	while (getline(myFile, line, '\n')){
		//line = line.substr(line.find_first_not_of(" "), line.size());
		temp << line;
		while (getline(temp, token, ' ')){
			if (!token.empty()){
				rawPoints.push_back(stod(token));
			}
		}
		temp.clear();
	}

	int n = rawPoints[0];
	vector <double> tempPoints;
	for (unsigned int i = 2; i < rawPoints.size(); i += n){
		for (int j = 0; j < n; j++){
			tempPoints.push_back(rawPoints.at(i + j));
		}
		vertexPoints.push_back(tempPoints);
		tempPoints.clear();
	}
}


vector <proto> proto_data::GetProtos(){
	return database;
}

string proto_data::firstPlane(int n){
	// appends the top plane of the convex hull.
	stringstream result;
	//result.setf(ios::scientific);
	result.setf(ios::fixed);
	result.precision(15);
	double *Simplex_cartesian;
	double *Simplex_concentration;
	int col, row;
	Simplex_cartesian = Simplex_Cart_Coord(n);
	Simplex_concentration = Simplex_Conc_Coord(Simplex_cartesian, n);
	for (col = 0; col < n + 1; col++)
	{
		for (row = 0; row < n; row++)
		{
			result << Simplex_concentration[row + col*n]*100;
			if(row<n - 1){
			  result << " ";
			}
		}
		result << " 0\n";
	}
	return result.str();
}


string proto_data::removeTrailingZeros(string text){
	string myText = text;
	while (myText.back() == '0'&& myText.size() > 1){
		myText.erase(myText.end() - 1);
	}
	return myText;
}

string proto_data::vertexOut(){
  return VDoutput;
}

vector <vector <double> > proto_data::dataout(){
	//function: iterates through the database of protos.
	//returns: a list of the concentration space coordinates. (for n dimentions this would be the last n-1 points & the enthalpy)
	vector <vector <double> > result;
	for (auto p : database){
		result.push_back(p.proto_val());
	}
	return result;
}

proto_data::~proto_data()
{
}
