#include <iostream>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include <pthread.h>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkDivideImageFilter.h" 


// Set up types
// If using threads, change to be void * 
void affineRegistration( std::string, std::string, std::string ) ;
int deformableRegistration( std::string, std::string, std::string ) ;

const unsigned int nDims = 3 ;
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
typedef itk::ImageFileWriter < ImageType > WriterType ;  

using namespace std ;


/*
struct thread_data {
	int thread_id ;
	string fixed ;
	string moving ;
	string output ;
}
*/
ImageType::Pointer LoadImage ( string inputFile ) {    
    ReaderType::Pointer reader = ReaderType::New() ;
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    return reader->GetOutput() ;
}

int averageImage( vector < string > files, string outFile ) {
    AddType::Pointer adder = AddType::New() ;
    ImageType::Pointer imgTotal = LoadImage( files[0] ) ;
        
    for ( int i = 1 ; i < files.size() ; i++ ) {
	adder->SetInput1( imgTotal );
	adder->SetInput2( LoadImage( files[i] ) );
	adder->Update() ;
	imgTotal = adder->GetOutput() ;
	cout << files[i] << endl ;
    }

    MultiplyType::Pointer multi = MultiplyType::New() ;
    multi->SetInput( imgTotal ) ;
    multi->SetConstant( (double) 1/21 ) ;
    multi->Update() ;

    WriterType::Pointer writer = WriterType::New() ;
    writer->SetFileName( outFile ) ;
    writer->SetInput( multi->GetOutput() ) ;
    writer->Update() ;

    return 0; 
} 


int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 1 ) {
	cerr << "Usage: " << endl ;
	cerr << argv[0] << "inputImagesList outputImageFile methodChoice" << endl ;
	return -1 ;
    }

    vector < string > files ;
    DIR* dirp = opendir( "./itk-images/" ) ;
    struct dirent * dp ;
    while( ( dp = readdir( dirp ) ) != NULL ) {
	if ( strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name,"..") != 0) {
	    files.push_back( dp->d_name ) ;
	}
    }
    closedir( dirp ) ;
    
    for ( int j = 0 ; j < files.size() ; j++ ) {
	cout << files[j] << endl ;
	files[j] = "./itk-images/" + files[j] ;	
    }

    vector < string > regArray ;
    vector < string > defArray ;
    regArray.push_back( files[0] ) ;
    defArray.push_back ( "./registrations/DR-" + files[0].substr( 21, 2 ) + ".nii.gz" ) ;

    for ( int i = 1 ; i < files.size() ; i++ ) {
 	regArray.push_back ( "./registrations/AR-" + files[i].substr( 21, 2 ) + ".nii.gz" ) ;
        defArray.push_back ( "./registrations/DR-" + files[i].substr( 21, 2 ) + ".nii.gz" ) ;
    }

    if ( strcmp( argv[1], "1" ) == 0 ) {
        cout << "Method 1" << endl ;
	averageImage( files, "initialAverage.nii.gz" ) ;
    } 
    else if ( strcmp( argv[1], "2" ) == 0 ) {
	cout << "Method 2" << endl ;
	// pthread_t threads[21] ;
	// struct thread_data td[21] ;
	for (int i = 1; i < files.size() ; i++ ) {
	    // td[i].thread_id = i ;
	    // td[i].fixed = files[0] ;
	    // td[i].moving = files[i] ;
	    // td[i].output = regArray[i] ;
	    // pthread_create( &threads[i], NULL, affineRegistration, (void *)&td[i] ) ;
	    affineRegistration( files[0], files[i], regArray[i] ) ;
	    cout << regArray[i] << endl ;
	}
	averageImage( regArray, "affineAverage.nii.gz" ) ;
    }
    else if ( strcmp( argv[1], "3" ) == 0 ) {
	cout << "Method 4" << endl ;
        for (int i = 0; i < 21 ; i++ ) {
	    deformableRegistration( "affineAverage.nii.gz", regArray[0], defArray[0] ) ; 
	    cout << defArray[i] << endl ;
	}
	averageImage( defArray, "deformableAverage.nii.gz" ) ;
    }
    // pthread_exit( NULL ) ;
    return 0 ;
}


