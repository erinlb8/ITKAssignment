#include <string>
#include <sys/types.h>
#include <dirent.h>
#include <vector>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkDivideImageFilter.h" 


// Set up types
// If using threads, change to be void * 
void affineRegistration( int, std::string, std::string, std::string ) ;
void deformableRegistration( std::string, std::string, std::string ) ;

const unsigned int nDims = 3 ;
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
typedef itk::ImageFileWriter < ImageType > WriterType ;  

using namespace std ;

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

    int method = atoi(argv[1]) ;
    int index = atoi(argv[2]) ;

    time_t current_time ;
    current_time = time( NULL ) ;
    cout << current_time << endl ;

    vector < string > files ;
    DIR* dirp = opendir( "./itk-images/" ) ;
    struct dirent * dp ;
    while( ( dp = readdir( dirp ) ) != NULL ) {
	if ( strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name,"..") != 0) {
	    files.push_back( dp->d_name ) ;
	}
    }
    closedir( dirp ) ;

    string dir1 = "./itk-images/" ;
    for ( int j = 0 ; j < files.size() ; j++ ) {
	files[j] = dir1 + files[j] ;	
    }

    string dir2 = "./registrations/AR-" ;
    string dir3 = "./registrations/DR-" ;
    string ft = ".nii.gz" ;
    vector < string > regArray ;
    vector < string > defArray ;
    regArray.push_back( files[0] ) ;
    
    defArray.push_back ( dir3 + files[0].substr( 21, 2 ) + ft ) ;
    
    for ( int i = 1 ; i < files.size() ; i++ ) {
 	regArray.push_back ( dir2 + files[i].substr( 21, 2 ) + ft ) ;
        defArray.push_back ( dir3 + files[i].substr( 21, 2 ) + ft ) ;
    }

    if ( strcmp( argv[1], "1" ) == 0 ) {
	cout << "Method 1" << endl ;
	averageImage( files, "initialAverage.nii.gz" ) ;
    } 
    else if ( strcmp( argv[1], "2" ) == 0 ) {
        ImageType::Pointer fixedImage = ImageType::New() ;
	ReaderType::Pointer reader = ReaderType::New() ;
	reader->SetFileName( files[0] ) ;
	reader->Update() ;
        fixedImage = reader->GetOutput() ;

	cout << "Method 2" << endl ;
	for (int i = 1; i < files.size() ; i++ ) {
	    affineRegistration( i, files[0], files[i], regArray[i] ) ;
	    cout << "Thread " << i << " started" << endl ;
	}

	averageImage( regArray, "affineAverage.nii.gz" ) ;	
    }
    else if ( method == 3 ) {
	cout << "Method 3" << endl ;
        //for (int i = 0; i < 21 ; i++ ) {
	    deformableRegistration( "affineAverage.nii.gz", regArray[index], defArray[index] ) ; 
	    cout << regArray[index] << endl ;
	//}
    }
    else if ( strcmp( argv[1], "4" ) == 0 ) {
	averageImage( defArray, "deformableAverage.nii.gz" ) ;
    }

    current_time = time( NULL ) ;
    cout << current_time << endl ;

    return 0 ;
}


