#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"

// Set up types
void affineRegistration( int, std::string, std::string, std::string ) ;
void deformableRegistration( std::string, std::string, std::string ) ;

const unsigned int nDims = 3 ;
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
typedef itk::ImageFileWriter < ImageType > WriterType ;  

ReaderType::Pointer reader = ReaderType::New() ;

using namespace std ;

// Loading Images
ImageType::Pointer LoadImage ( string inputFile ) {    
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    return reader->GetOutput() ;
}

// Average a set of images
int averageImage( vector < string > files, string outFile ) {
    AddType::Pointer adder = AddType::New() ;
    ImageType::Pointer imgTotal = LoadImage( files[0] ) ;
        
    // Sum of images
    for ( int i = 1 ; i < files.size() ; i++ ) {
	adder->SetInput1( imgTotal );
	adder->SetInput2( LoadImage( files[i] ) );
	adder->Update() ;
	imgTotal = adder->GetOutput() ;
    }

    // Averaging
    MultiplyType::Pointer multi = MultiplyType::New() ;
    double factor = 1/( (double) files.size() ) ;
    multi->SetInput( imgTotal ) ;
    multi->SetConstant( factor ) ;
    multi->Update() ;

    // Write images
    WriterType::Pointer writer = WriterType::New() ;
    writer->SetFileName( outFile ) ;
    writer->SetInput( multi->GetOutput() ) ;
    writer->Update() ;

    return 0; 
} 


int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 3 ) {
	cerr << "Usage: " << endl ;
	cerr << argv[0] << "methodSelection deformableRegistrationIndex" << endl ;
	return -1 ;
    }

    int method = atoi(argv[1]) ;
    int ind = atoi(argv[2]) ;

    // Read base files
    vector < string > files ;
    DIR* dirp = opendir( "./itk-images/" ) ;
    struct dirent * dp ;
    while( ( dp = readdir( dirp ) ) != NULL ) {
	if ( strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name,"..") != 0) {
	    files.push_back( dp->d_name ) ;
	}
    }
    closedir( dirp ) ;

    // Attach directory
    string dir1 = "./itk-images/" ;
    for ( int j = 0 ; j < files.size() ; j++ ) {
	files[j] = dir1 + files[j] ;	
    }

    // Attach directory for registrations
    string dir2 = "./registrations/AR-" ;
    string ft = ".nii.gz" ;
    vector < string > regArray ;
    
    for ( int i = 0 ; i < files.size() ; i++ ) {
 	regArray.push_back ( dir2 + files[i].substr( 21, 2 ) + ft ) ;
    }

    if ( method == 1 ) { 		// Calculation of average of original images
	averageImage( files, "initialAverage.nii.gz" ) ;
    } 
    else if ( method == 2 ) {		// Affine registration and averaging
	for (int i = 0; i < files.size() ; i++ ) {
	    affineRegistration( i, files[0], files[i], regArray[i] ) ;
	}
	averageImage( regArray, "affineAverage.nii.gz" ) ;	
    }
    else if ( method == 3 ) {		// Demons registration (uses slurm job array)
	deformableRegistration( "affineAverage.nii.gz", regArray[ind], regArray[ind] ) ;
    }
    else if ( method == 4 ) {		// Deformable averaging
	averageImage( regArray, "./atlas-images/deformableAverage" + string( argv[2] ) + ".nii.gz" ) ;
    }

    return 0 ;
}
