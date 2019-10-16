#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <thread>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkDivideImageFilter.h" 


// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image < float, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
 
ImageType::Pointer affineRegistration( std::string, std::string, std::string );

/*
ImageType::Pointer affReg( ImageType::Pointer imgTotal, std::string inMoving, std::string outFile ) {
    adder2->SetInput1( imgTotal ) ;
    adder2->SetInput2( affineRegistration( randSub, inMoving, outFile ) ;
    adder2->Update() ;
    return  adder2->GetOutput() ;
}
*/

ImageType::Pointer LoadImage ( std::string inputFile ) {    
    ReaderType::Pointer reader = ReaderType::New() ;
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    return reader->GetOutput() ;
}


int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 2 ) {
	std::cerr << "Usage: " << std::endl ;
	std::cerr << argv[0] << "inputImageFile inputFixedImageFile outputImageFile" << std::endl ;
	return -1 ;
    }

/*
    std::fstream f ;
    f.open( argv[1] ) ;
    std::string line ;
    std::string files[21];
    int c = 0 ;
  
    getline(f, line) ;
*/
    //std::string ln2 = "./itk-images/" + std::string( argv[1] ) ;
    //files[c] = ln2 ;
    //c++;

    AddType::Pointer adder = AddType::New() ;

    ImageType::Pointer imgTotal = LoadImage( argv[1] ) ;
    // ImageType::Pointer img2 = ImageType::New() ;
    
    // while(getline(f,line)) {
//	ln2 = "./itk-images/" + line ;
	// adder->SetInput1( imgTotal );
	// adder->SetInput2( LoadImage( ln2 ) );
	// adder->Update() ;
	// imgTotal = adder->GetOutput() ;
        // files[c] = ln2 ;
	// c++;
    // }
 
    // srand(time(0)) ;
    // int r = (rand() % 21 ) + 1 ;
    AddType::Pointer adder2 = AddType::New() ;
 
    // std::cout << files[r] << std::endl ;
    std::string fixedImage = "./itk-images/KKI2009-01-MPRAGE.nii" ;
    ImageType::Pointer randSub = LoadImage( fixedImage ) ;
    imgTotal = randSub ;

    // const char * filePrefix = "./registrations/AR-" ;
    // char * fSuffix = ".gz" ;
    // std::thread t[21] ;

//    for (int i = 0; i < 21; i++ ) {
//	if (i != r ) {
    	    std::string outFile = "./registrations/AR-" + std::string( argv[1] ).substr( 19, 2 ) + ".nii.gz" ;
	    //t[i] = std::thread(affReg, randSub, files[i], outFile ) ;
	    ImageType::Pointer afr = affineRegistration( fixedImage, argv[1], outFile ) ;
	    
// 	}
//    }
/*
    for (int i=0; i<21; i++ ) {
	t[i].join()
    }


    typedef itk::ImageFileWriter < ImageType > WriterType ;
    WriterType::Pointer writer = WriterType::New() ;
    writer->SetFileName( argv[3] ) ;
    writer->SetInput( afr ) ;
    writer->Update() ;
*/
    return 0 ;
}


