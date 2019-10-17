#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkDemonsRegistrationFilter.h"
#include "itkPDEDeformableRegistrationFilter.h"

#include <string>

// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image< double, nDims > ImageType ;



int deformableRegistration( std::string inFixed, std::string inMoving, std::string outImage ) {
  
  typedef itk::ImageFileReader < ImageType > ReaderType ;
  ReaderType::Pointer reader1 = ReaderType::New() ;
  reader1->SetFileName( inFixed ) ;
  reader1->Update() ;
  ImageType::Pointer fixed = reader1->GetOutput() ;

  ReaderType::Pointer reader2 = ReaderType::New() ;
  reader2->SetFileName( inMoving ) ;
  reader2->Update() ;
  ImageType::Pointer moving = reader2->GetOutput() ;

  typedef itk::DemonsRegistrationFilter < ImageType, ImageType, ImageType > DemonsType ;
  typedef itk::PDEDeformableRegistrationFilter < ImageType, ImageType, ImageType > RegistrationType ;

  // DemonsType::Pointer demons = DemonsType::New() ;
  RegistrationType pdeDeform = RegistrationType::New() ;


  //demons->SetFixedImage( fixed ) ;
  //demons->SetMovingImage( moving ) ;
  //demons->SetNumberOfIterations( 100 ) ;
  // demons->SetStandardDeviations( 1.0 ) ;
/*
  try {
    demons->Update() ;
  } 
  catch (itk::ExceptionObject & exct ) {
    std::cerr << exct << std::endl ;
    return -1 ;
  }
*/
  //typedef itk::ImageFileWriter < ImageType > WriterType ;
  //WriterType::Pointer writer = WriterType::New() ;
  //writer->SetFileName( outImage ) ;
  //writer->SetInput( demons->GetOutput() ) ;
  //writer->Update() ;

  return 0;
}
