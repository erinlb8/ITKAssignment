#include <iostream>
#include <fstream>
<<<<<<< HEAD
#include <cstdlib>
=======
#include <string>
#include <ctime>
#include <cstdlib>
#include <thread>
>>>>>>> 7ee0c728c26cdc44873264d56a3ad55425ce0376
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkAddImageFilter.h"
#include "itkMultiplyImageFilter.h"
#include "itkImageFileWriter.h"
#include "itkDivideImageFilter.h" 


// Set up types
void affineRegistration( std::string, std::string, std::string );

const unsigned int nDims = 3 ;
<<<<<<< HEAD
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
  
=======
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
>>>>>>> 7ee0c728c26cdc44873264d56a3ad55425ce0376

ImageType::Pointer LoadImage ( std::string inputFile ) {    
    ReaderType::Pointer reader = ReaderType::New() ;
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    return reader->GetOutput() ;
}


<<<<<<< HEAD
ImageType::Pointer averageImage( std::string files[] ) {
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

=======
int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 2 ) {
	std::cerr << "Usage: " << std::endl ;
	std::cerr << argv[0] << "inputImageFile inputFixedImageFile outputImageFile" << std::endl ;
	return -1 ;
    }

/*
>>>>>>> 7ee0c728c26cdc44873264d56a3ad55425ce0376
    std::fstream f ;
    f.open( argv[1] ) ;
    std::string line ;
    std::string files[21];
    int c = 0 ;
<<<<<<< HEAD
    std::string ln2  = "" ;  

    while( getline(f, line) ) {
	ln2 = "./itk-images/" + line ;
	files[c] = ln2 ;
	// std::cout << files[c] << std::endl ;
	c++ ;
    }

    f.close() ;
/*    
    ImageType::Pointer finImage = averageImage( files ) ;

    typedef itk::ImageFileWriter < ImageType > WriterType ;
    WriterType::Pointer writer = WriterType::New() ;
    writer->SetFileName( argv[2] ) ;
    writer->SetInput( finImage ) ;
    writer->Update() ;
*/
    AddType::Pointer adder2 = AddType::New() ;
    ImageType::Pointer imgTotal = LoadImage( files[0] ) ;
    // ImageType::Pointer imgTotal  = randSub ;
    // std::thread t[21] ;

    std::string regArray[21] ;
    regArray[0] = files[0] ;

    for (int i = 1; i < 21; i++ ) {
	std::string outFile = "./registrations/AR-" + files[i].substr( 21, 2 ) + ".nii.gz" ;
	//affineRegistration( files[0], files[i], outFile ) ;
	std::cout << outFile << std::endl ;
	regArray[i] = outFile ;
    }

    
    imgTotal = averageImage( regArray ) ;

    typedef itk::ImageFileWriter < ImageType > WriterType ;
    WriterType::Pointer writer = WriterType::New() ;
    writer->SetFileName( argv[2] ) ;
    writer->SetInput( imgTotal ) ;
    writer->Update() ;
=======
  
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
>>>>>>> 7ee0c728c26cdc44873264d56a3ad55425ce0376

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


