#include "simplex.h"

double *Simplex_Cart_Coord ( int n )
{
  // These generate the simplex coordinates in cartesian space.
  int row,col;
  int var1,var2,var3;
  double sum,dot_sum;
  double *Matrix;
 
  Matrix=Initialise(n);
  
  for(col=0;col<n;col++)
    { 
      sum=0;
      
      for(var1=0;var1<col;var1++)
	sum=sum+Matrix[var1+col*n]*Matrix[var1+col*n];
      
      Matrix[col+col*n]= sqrt(1.0-sum);
      
      for(var2=col+1;var2<n+1;var2++)
	{
	  dot_sum=0;
	  
	  for(var3=0;var3<col;var3++)
	    dot_sum=dot_sum+Matrix[var3+col*n]*Matrix[var3+var2*n];

	  Matrix[col+var2*n]=(-1.0/(double)(n)-dot_sum)/Matrix[col+col*n];
	 
	}
	  
    }


  return Matrix;
}



double *Simplex_Conc_Coord (double *Mat_Sim_Cart,int n)

{

  // Converts the simplex coordinates from cartesian to concentration space.

  int row,col;
  double sum_squared;
  double *Mat_Conc;

  Mat_Conc=Mat_Sim_Cart;

  //Subracting first vector from all other vectors. i.e. Transforming one vector to origin.
  // The first vector is 1,0,0...bunch of zeros.

  for(col=0;col<n+1;col++)
    Mat_Conc[col*n]=Mat_Conc[col*n]-1;  // I wrote 1 because, I know first vector has only one and all zeros.
  
  //Normalising all the vectors.

  for (col=1;col<n+1;col++)    //col =0 has all zeros; so normalising doesnt work, hence col =1 to n+1 is given.
    {
      sum_squared=0;
      
      for(row=0;row<n;row++)
	sum_squared=sum_squared+Mat_Conc[row+col*n]*Mat_Conc[row+col*n];

      for(row=0;row<n;row++)
	Mat_Conc[row+col*n]=Mat_Conc[row+col*n]/ sqrt (sum_squared);
    }

   // I'm adding few lines to solve this gnuplot problems. WE can change it later.                                                                     



  /*
  if(n==2)
    {
      Mat_Conc[0]=0;
      Mat_Conc[1]=0;
      Mat_Conc[2]=0.5;
      Mat_Conc[3]=sqrt(3)/2;
      Mat_Conc[4]=1;
      Mat_Conc[5]=0;
    }
  */

  return Mat_Conc;
  
}


double *Transformation_Matrix (double *Simp_Conc_Mat, int n)
{
  int row,col;
  double *SC_Mat;  //simplex_conc_matrix
  double *T_Mat;  // transformed new matrix;

// Initialising T_Mat

  T_Mat= new double[(n+1)*(n+1)];   
   for ( col = 0; col < n+1; col++ )
    for ( row = 0; row < n+1; row++ )
      T_Mat[row+col*(n+1)] = 0.0;
   
// Initialising done..

  SC_Mat=Simp_Conc_Mat;

  /* Leaving the last row and Last coloumn fill all the n*n matrix part of T_Mat with all the vectors from Simp_conc_mat..
     of course we leave the first coloumn with all zeros... so total n coloumns and n rows. */

  for(col=0;col<n;col++)
     for(row=0;row<n;row++)
	 T_Mat[row+col*(n+1)]=SC_Mat[row+(col+1)*n];

  /*Now fill the last row and last coloumn with all zeros except the (n,n) element being 1, so that when we multiply 
    the transformation matrix with the alloy data enthalpy is not changed.  One good thing is as already each element of T_Mat is initialised to zeros, we can just make the (n,n) element to be 1. That's enough. */

  T_Mat[(n+1)*(n+1)-1]=1.0;

  return T_Mat;
}


double  *Initialise ( int n )    // This function initializes only simplex coordinate matrices. 
{

  int row,col;
  double *Matrix;

  Matrix = new double[n*(n+1)];

  for ( col = 0; col < n+1; col++ )
    for ( row = 0; row < n; row++ ) 
   	  Matrix[row+col*n] = 0.0;

  return Matrix;
}



