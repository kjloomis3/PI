#include <iostream> 
#include <cstdlib> 
#include <cmath> 
#include <iomanip> 
#include <gmpxx.h> 
#include <pthread.h> 

using namespace std; 

const int BIT_SIZE = 500000; 
mpf_class *RIEMANN; 
mpf_class RADIUS("1.0", BIT_SIZE); 

struct ThreadType 
{ 
 int iter; 
 mpf_class* start; 
 mpf_class* end; 
 mpf_class* rtnVal; 
 mpf_class* sum; 
}; 

ThreadType* ThreadData; 

void* GetArea( void* args ) 
{ 
 ThreadType *data = ( ThreadType* ) args; 

 mpf_class width( 0,BIT_SIZE ); 
 mpf_class f_of_x( 0, BIT_SIZE ); 
 mpf_class f_of_x2( 0, BIT_SIZE ); 
 mpf_class trapizoid( 0, BIT_SIZE ); 
 mpf_class i(*data->start, BIT_SIZE ); 
 mpf_class sum( 0, BIT_SIZE ); 

 int iteration = data->iter; 
 width = ( i * RADIUS / *RIEMANN ); 
 f_of_x = sqrt( RADIUS * RADIUS - width * width); 
 for ( i=i+1; cmp( i, *data->end )<=0; i= i+1 ) 
 { 
	width = ( i * RADIUS / *RIEMANN ); 
	f_of_x2 = f_of_x; 
	f_of_x = sqrt( RADIUS * RADIUS - width * width); 
	trapizoid = RADIUS / *RIEMANN * ( f_of_x + f_of_x2 ) / 2; 
	sum = sum + trapizoid; 
 } 
 *data->rtnVal = sum; 
 pthread_exit(0); 
 return (void*) 0; 
} 

int main( int argc, char *argv[] ) 
{ 
 int PARTS = 1; 
 switch (argc) 
 { 
   case (1):{ 	break;	} 
   case (2):{ 	RIEMANN = new mpf_class( argv[1], BIT_SIZE ); break;	} 
   case (3):{ 	RIEMANN = new mpf_class( argv[1], BIT_SIZE ); 
		PARTS = atoi( argv[2] ); break; } 
   default:{	cout << "Incorrect usage!" << endl; 
		exit ( 1 ); break; } 
 } 
  
 ThreadData = new ThreadType[PARTS]; 
 mpf_class sum( 0, BIT_SIZE ); 
 mpf_class i( 0, BIT_SIZE ); 
 mpf_class pi( "3.1415926535897932384626433832795028841971693993751", BIT_SIZE ); 
 pthread_t thID[PARTS]; 
 for ( int iter=0; iter<PARTS; iter++ ) 
 { 
	ThreadData[iter].start = new mpf_class( 0, BIT_SIZE ); 
	ThreadData[iter].end = new mpf_class( 0, BIT_SIZE ); 
	ThreadData[iter].rtnVal = new mpf_class( 0, BIT_SIZE ); 
	ThreadData[iter].sum = new mpf_class( 0, BIT_SIZE ); 
 } 
 for ( int iter=0; iter<PARTS; iter++ ) 
 { 
	ThreadData[iter].iter = iter; 
	*ThreadData[iter].start = i; 
	i = i + ( *RIEMANN / PARTS ); 
	*ThreadData[iter].end = i; 
	pthread_create (&thID[iter], NULL, GetArea, (void*) &ThreadData[iter]); 
 } 
 for (int iter=0; iter<PARTS; iter++) 
 { 
   pthread_join ( thID[iter], NULL ); 
 } 
 for ( int iter=0; iter<PARTS; iter++ ) 
 { 
   sum = sum + *ThreadData[iter].rtnVal; 
 } 
 sum = (sum * 4)/(RADIUS * RADIUS); 
 cout << fixed << setprecision(50); 
 cout << "**************************************************************" << endl; 
 cout << "real pi = " << pi << endl; 
 cout << "calc pi = " << sum << endl; 
 cout << "**************************************************************" << endl; 
 cout << "diff pi "; 
 if ( cmp( pi, sum )<0 ) 
	cout << "+ " << sum-pi << endl; 
 else 
	cout << "- " << pi-sum << endl; 
 cout << "**************************************************************" << endl; 

 return 0; 
} 
