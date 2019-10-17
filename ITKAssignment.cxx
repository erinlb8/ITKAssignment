#include <iostream>
#include <fstream>
#include <string>
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

int averageImage( std::string files[], std::string outFile ) {
    AddType::Pointer adder = AddType::New() ;
    ImageType::Pointer imgTotal = LoadImage( files[0] ) ;
        
    for ( int i = 1 ; i < 21 ; i++ ) {
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
    if (argc < 2 ) {
	std::cerr << "Usage: " << std::endl ;
	std::cerr << argv[0] << "inputImagesList outputImageFile methodChoice" << std::endl ;
	return -1 ;
    }

    std::fstream f ;
    f.open( argv[1] ) ;
    std::string line ;
    std::string files[21];
    int c = 0 ;
    std::string ln2  = "" ;  

    while( getline(f, line) ) {
	ln2 = "./itk-images/" + line ;
	files[c] = ln2 ;
	c++ ;
    }

    f.close() ;

    std::string regArray[21] ;
    std::string defArray[21] ;
    regArray[0] = files[0] ;
    defArray[0] = "./registrations/DR-" + files[0].substr( 21, 2 ) + ".nii.gz" ;

    for ( int i = 1 ; i < 21 ; i++ ) {
 	regArray[i] = "./registrations/AR-" + files[i].substr( 21, 2 ) + ".nii.gz" ;
        defArray[i] = "./registrations/DR-" + files[i].substr( 21, 2 ) + ".nii.gz" ;
    }
 
    if ( strcmp( argv[3], "1" ) == 0 ) {
        // std::cout << "Method 1" << std::endl ;
	averageImage( files, argv[2] ) ;
    } 
    else if ( strcmp( argv[3], "2" ) == 0 ) {
	// std::cout << "Method 2" << std::endl ;
	for (int i = 1; i < 21; i++ ) {
	    affineRegistration( files[0], files[i], regArray[i] ) ;
	    // std::cout << regArray[i] << std::endl ;
	}
    }
    else if ( strcmp( argv[3], "3" ) == 0 ) {
	// std::cout << "Method 3" << std::endl ;
	averageImage( regArray, argv[2] ) ;
    }
    else if ( strcmp( argv[3], "4" ) == 0 ) {
	std::cout << "Method 4" << std::endl ;
        //for (int i = 0; i < 21 ; i++ ) {
	// std::string outFile = "./registrations/DR-" + files[1].substr( 21, 2 ) + ".nii.gz" ;
	    deformableRegistration( "test6.nii.gz", regArray[0], defArray[0] ) ; 
	//}
    }
//    else if ( strcmp( argv[3], "5" ) ) {

//    }
    return 0 ;
}


