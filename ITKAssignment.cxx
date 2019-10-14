#include "itkImage.h"
#include "itkImageFileReader.h"

int main ( int argc, char * argv[] )
{
    // Verify command line arguments
    if (argc < 3 ) {
	std::cerr << "useage: " << std::endl ;
	std::cerr << argv[0] << "inputImageFile outputImageFile" << std::endl ;
	return -1 ;
    }

    // Set up types
    const unsigned int nDims = 3 ;
    typedef itk::Image < int, nDims > ImageType ;



    return 0 ;
}
