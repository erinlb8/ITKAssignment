#include <iostream>
#include <fstream>
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
void affineRegistration( std::string, std::string, std::string ) ;
int deformableRegistration( std::string, std::string, std::string ) ;

const unsigned int nDims = 3 ;
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
typedef itk::ImageFileWriter < ImageType > WriterType ;  


ImageType::Pointer LoadImage ( std::string inputFile ) {    
    ReaderType::Pointer reader = ReaderType::New() ;
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    return reader->GetOutput() ;
}

int averageImage( std::vector < std::string > files, std::string outFile ) {
    AddType::Pointer adder = AddType::New() ;
    ImageType::Pointer imgTotal = LoadImage( files[0] ) ;
        
    for ( int i = 1 ; i < files.size() ; i++ ) {
	adder->SetInput1( imgTotal );
	adder->SetInput2( LoadImage( files[i] ) );
	adder->Update() ;
	imgTotal = adder->GetOutput() ;
	std::cout << files[i] << std::endl ;
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
	std::cerr << "Usage: " << std::endl ;
	std::cerr << argv[0] << "inputImagesList outputImageFile methodChoice" << std::endl ;
	return -1 ;
    }

    std::vector < std::string > files ;
    DIR* dirp = opendir( "./itk-images/" ) ;
    struct dirent * dp ;
    while( ( dp = readdir( dirp ) ) != NULL ) {
	if ( strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name,"..") != 0) {
	    files.push_back( dp->d_name ) ;
	}
    }
    closedir( dirp ) ;
    
    for ( int j = 0 ; j < files.size() ; j++ ) {
	std::cout << files[j] << std::endl ;
	files[j] = "./itk-images/" + files[j] ;	
    }




    std::string regArray[ files.size() ] ;
    std::string defArray[ files.size() ] ;
    regArray[0] = files[0] ;
    defArray[0] = "./registrations/DR-" + files[0].substr( 21, 2 ) + ".nii.gz" ;

    for ( int i = 1 ; i < files.size() ; i++ ) {
 	regArray[i] = "./registrations/AR-" + files[i].substr( 21, 2 ) + ".nii.gz" ;
        defArray[i] = "./registrations/DR-" + files[i].substr( 21, 2 ) + ".nii.gz" ;
	std::cout << regArray[i] << std::endl ;
	std::cout << defArray[i] << std::endl ;
    }

//    if ( strcmp( argv[3], "1" ) == 0 ) {
        std::cout << "Method 1" << std::endl ;
	averageImage( files, "initialAverage.nii.gz" ) ;
/*    } 
    else if ( strcmp( argv[3], "2" ) == 0 ) {
	std::cout << "Method 2" << std::endl ;
	for (int i = 1; i < 21; i++ ) {
	    affineRegistration( files[0], files[i], regArray[i] ) ;
	    std::cout << regArray[i] << std::endl ;
	}
    }
    else if ( strcmp( argv[3], "3" ) == 0 ) {
	std::cout << "Method 3" << std::endl ;
	averageImage( regArray, argv[2] ) ;
    }
    else if ( strcmp( argv[3], "4" ) == 0 ) {
	std::cout << "Method 4" << std::endl ;
        for (int i = 0; i < 21 ; i++ ) {
	    deformableRegistration( "test6.nii.gz", regArray[0], defArray[0] ) ; 
	    std::cout << defArray[i] << std::endl ;
	}
    }
*/
    return 0 ;
}


