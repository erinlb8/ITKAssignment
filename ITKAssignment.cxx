#include "itkImage.h"
#include "itkImageFileReader.h"

// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image < int, nDims > ImageType ;
typedef itk::ImageFileReader < ImageType > ReaderType ;

ImageType::Pointer LoadImage ( const char * inputFile ) {    
    ReaderType::Pointer reader = ReaderType::New() ;
    reader->SetFileName( inputFile ) ;
    reader->Update() ;
    ImageType::Pointer mvImg = reader->GetOutput() ;
    return mvImg ;
}

int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 3 ) {
	std::cerr << "useage: " << std::endl ;
	std::cerr << argv[0] << "inputImages outputImageFile" << std::endl ;
	return -1 ;
    }

    

    return 0 ;
}
