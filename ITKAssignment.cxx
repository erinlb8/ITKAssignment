#include <iostream>
#include <fstream>
#include <cstdlib>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkDivideImageFilter.h" 


// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
 
 
ImageType::Pointer affineRegistration( std::string, std::string, std::string );

ImageType::Pointer LoadImage ( std::string inputFile ) {    
    ReaderType::Pointer reader = ReaderType::New() ;
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    return reader->GetOutput() ;
}

ImageType::Pointer averageImage( std::string files[] ) {
    AddType::Pointer adder = AddType::New() ;

    ImageType::Pointer imgTotal = LoadImage( files[0] ) ;
        
    for ( int i = 1 ; i < 21 ; i++ ) {
	adder->SetInput1( imgTotal );
	adder->SetInput2( LoadImage( files[i] ) );
	adder->Update() ;
	imgTotal = adder->GetOutput() ;
    }

    MultiplyType::Pointer multi = MultiplyType::New() ;
    multi->SetInput( imgTotal ) ;
    multi->SetConstant( (double) 1/21 ) ;
    multi->Update() ;
    return multi->GetOutput() ; 
} 


int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 3 ) {
	std::cerr << "Usage: " << std::endl ;
	std::cerr << argv[0] << "inputImageFile outputImageFile" << std::endl ;
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
    

    //AddType::Pointer adder2 = AddType::New() ;
 
    //ImageType::Pointer randSub = LoadImage( files[0] ) ;
    //imgTotal = randSub ;

    //for (int i = 0; i < 21; i++ ) {
//	if (i != r ) {
    //	    std::string outFile = "./registrations/AR-" + std::string( argv[1] ).substr( 8, 2 ) + ".nii.gz" ;
	    //t[i] = std::thread(affReg, randSub, files[i], outFile ) ;
//	    ImageType::Pointer afr = affineRegistration( fixedImage, "./itk-images/" + argv[1], outFile ) ;
	    
// 	}
//    }
/*
    for (int i=0; i<21; i++ ) {
	t[i].join()
    }

*/
    ImageType::Pointer finImage = averageImage( files ) ;

//    std::cout << multi->GetConstant() << std::endl ;

    typedef itk::ImageFileWriter < ImageType > WriterType ;
    WriterType::Pointer writer = WriterType::New() ;
    writer->SetFileName( argv[2] ) ;
    writer->SetInput( finImage ) ;
    writer->Update() ;

    return 0 ;
}


