#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkMultiResolutionPDEDeformableRegistration.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkDisplacementFieldTransform.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkDemonsRegistrationFilter.h"

#include <string>

// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image< double, nDims > ImageType ;

class RegistrationObserver : public itk::Command {
public:
  typedef RegistrationObserver Self ;
  typedef itk::Command Superclass ;
  typedef itk::SmartPointer < Self > Pointer ;
  itkNewMacro( Self ) ;

  typedef itk::Vector < float, 2 > Vector ;
  typedef itk::Image < Vector, 2 > DisFieldType ;
  typedef itk::DemonsRegistrationFilter < ImageType, ImageType, DisFieldType > DemonsType ;
  typedef DemonsType * DemonsPointer ;

 
  void Execute( itk::Object * caller, const itk::EventObject & event ) {
    DemonsPointer demons = dynamic_cast < DemonsPointer > ( caller ) ;
    if (!(itk::IterationEvent().CheckEvent( & event ) ) ) {
      return ;
    }
    //std::cout << demons->GetMetric() << std::endl ;
  }

  void Execute( const itk::Object *caller, const itk::EventObject &event) {

  }

protected:
  RegistrationObserver() {};

} ;


void deformableRegistration( std::string inFixed, std::string inMoving, std::string outImage )
{
  std::cout << "Fixed" << std::endl;

  typedef itk::ImageFileReader < ImageType > ReaderType ;
  ReaderType::Pointer reader1 = ReaderType::New() ;
  reader1->SetFileName( inFixed ) ;
  reader1->Update() ;
  ImageType::Pointer fixed = reader1->GetOutput() ;

  ReaderType::Pointer reader2 = ReaderType::New() ;
  reader2->SetFileName( inMoving ) ;
  reader2->Update() ;
  ImageType::Pointer moving = reader2->GetOutput() ;

  typedef itk::HistogramMatchingImageFilter < ImageType, ImageType > MatchType ;
  typedef itk::Vector < double, nDims > Vector ;
  typedef itk::Image < Vector, nDims > DisType ;
  typedef itk::DemonsRegistrationFilter < ImageType, ImageType, DisType > Demons ;

  MatchType::Pointer match = MatchType::New() ;
  Demons::Pointer demons = Demons::New() ;
  RegistrationObserver::Pointer observer = RegistrationObserver::New() ;

  match->SetInput( moving ) ;
  match->SetReferenceImage( fixed ) ;
  match->SetNumberOfHistogramLevels( 1024 ) ;
  match->SetNumberOfMatchPoints( 7 ) ;
  match->ThresholdAtMeanIntensityOn() ;

  std::cout << "Moving" << std::endl ;


  //demons->AddObserver( itk::IterationEvent(), observer ) ;
  demons->SetFixedImage( match->GetOutput() ) ;
  demons->SetMovingImage( moving ) ;
  demons->SetNumberOfIterations( 50 ) ;
  demons->SetStandardDeviations( 1.0 ) ;

  std::cout << "Moving" << std::endl ;

  
  try { 
// std::cout << transform->GetNumberOfParameters() << std::endl ;
    demons->Update() ;
  } catch (itk::ExceptionObject & e ) {
    std::cerr << e << std::endl ;
  }

  typedef itk::ResampleImageFilter < ImageType, ImageType > ResampleType ;
  typedef itk::DisplacementFieldTransform < double, nDims > DisplacementType ;
  
  DisplacementType::Pointer transform = DisplacementType::New() ;
  transform->SetDisplacementField( demons->GetOutput() ) ;
 
  ResampleType::Pointer resample = ResampleType::New() ;
  resample->SetInput( moving ) ;
  resample->SetTransform( transform ) ;
  resample->SetSize( moving->GetLargestPossibleRegion().GetSize() ) ;
  resample->SetReferenceImage( fixed ) ;
  resample->UseReferenceImageOn() ;
  resample->Update() ;


  typedef itk::ImageFileWriter < ImageType > WriterType ;
  WriterType::Pointer writer = WriterType::New() ;
  writer->SetFileName( outImage ) ;
  writer->SetInput( resample->GetOutput() ) ;
  writer->Update() ;

  return ;
}
