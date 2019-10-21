#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkDisplacementFieldTransform.h"
#include "itkHistogramMatchingImageFilter.h"
#include "itkResampleImageFilter.h"
#include "itkDemonsRegistrationFilter.h"

#include <string>

// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image< double, nDims > ImageType ;

void deformableRegistration( std::string inFixed, std::string inMoving, std::string outImage )
{
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

  match->SetInput( moving ) ;
  match->SetReferenceImage( fixed ) ;
  match->SetNumberOfHistogramLevels( 1024 ) ;
  match->SetNumberOfMatchPoints( 7 ) ;
  match->ThresholdAtMeanIntensityOn() ;

  demons->SetFixedImage( fixed ) ;
  demons->SetMovingImage( match->GetOutput() ) ;
  demons->SetNumberOfIterations( 50 ) ;
  demons->SetStandardDeviations( 1.0 ) ;

  try { 
    demons->Update() ;
  } catch (itk::ExceptionObject & e ) {
    std::cerr << e << std::endl ;
  }

  typedef itk::ResampleImageFilter < ImageType, ImageType > ResampleType ;
  typedef itk::DisplacementFieldTransform < double, nDims > DisplacementType ;
  typedef itk::LinearInterpolateImageFunction < ImageType, double > LinearType ;

  LinearType::Pointer linear = LinearType::New() ;
  DisplacementType::Pointer transform = DisplacementType::New() ;
  ResampleType::Pointer resample = ResampleType::New() ;

  transform->SetDisplacementField( demons->GetOutput() ) ;
  
  resample->SetInput( moving ) ;
  resample->SetTransform( transform ) ;
  resample->SetInterpolator( linear ) ;
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
