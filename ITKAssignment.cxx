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
void affineRegistration( std::string, std::string, std::string );

const unsigned int nDims = 3 ;
typedef itk::Image < double, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;
typedef itk::MultiplyImageFilter < ImageType, ImageType, ImageType > MultiplyType ;
typedef itk::AddImageFilter < ImageType, ImageType, ImageType > AddType ;
  

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
    if (argc < 2 ) {
	std::cerr << "Usage: " << std::endl ;
	std::cerr << argv[0] << "inputImageFile inputFixedImageFile outputImageFile" << std::endl ;
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

    return 0 ;
}


