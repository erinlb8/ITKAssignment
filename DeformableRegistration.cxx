#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkBSplineTransform.h"
#include "itkResampleImageFilter.h"
#include "itkImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkResampleImageFilter.h"

#include <pthread.h>
#include <string>

// Set up types
const unsigned int nDims = 3 ;
typedef itk::Image< double, nDims > ImageType ;

struct thread_data {
  int thread_id ;
  std::string fixed ;
  std::string moving ;
  std::string output ;
} ;

void * deformableRegistration( void * thread_args ) // std::string inFixed, std::string inMoving, std::string outImage ) {
{
  struct thread_data * args = (struct thread_data *) thread_args ;
  std::string inFixed = args->fixed ;
  std::string inMoving = args->moving ;
  std::string outImage = args->output ;

  typedef itk::ImageFileReader < ImageType > ReaderType ;
  ReaderType::Pointer reader1 = ReaderType::New() ;
  reader1->SetFileName( inFixed ) ;
  reader1->Update() ;
  ImageType::Pointer fixed = reader1->GetOutput() ;

  ReaderType::Pointer reader2 = ReaderType::New() ;
  reader2->SetFileName( inMoving ) ;
  reader2->Update() ;
  ImageType::Pointer moving = reader2->GetOutput() ;

  typedef itk::BSplineTransform < double, nDims, 3 > BSplineType ;
  typedef itk::MeanSquaresImageToImageMetric < ImageType, ImageType > MetricType ;
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType ;
  typedef itk::ImageRegistrationMethod < ImageType, ImageType > RegistrationType ;
  typedef itk::LinearInterpolateImageFunction < ImageType, double > InterpolatorType ;
  
  BSplineType::Pointer transform = BSplineType::New() ;
  RegistrationType::Pointer registration = RegistrationType::New() ;
  MetricType::Pointer meanSquares = MetricType::New() ;
  OptimizerType::Pointer rsgd = OptimizerType::New() ;
  InterpolatorType::Pointer linear = InterpolatorType::New() ;

  transform->SetIdentity() ;
  
  BSplineType::PhysicalDimensionsType fixedPD ;  
  BSplineType::MeshSizeType meshSize ;
  for ( int i = 0 ; i < nDims ; i++ ) { 
    fixedPD[i] = fixed->GetSpacing()[i] * static_cast<double>(fixed->GetLargestPossibleRegion().GetSize()[i] - 1) ;
  }

  meshSize.Fill( 5 ) ;
  
  registration->SetMovingImage( moving ) ;
  registration->SetFixedImage( fixed ) ;
  registration->SetMetric( meanSquares ) ;
  registration->SetOptimizer( rsgd ) ;
  registration->SetInterpolator( linear ) ;
  registration->SetTransform( transform ) ;
  // registration->InPlaceOn() ;

  transform->SetTransformDomainOrigin( fixed->GetOrigin() ) ;
  transform->SetTransformDomainPhysicalDimensions( fixedPD ) ;
  transform->SetTransformDomainDirection( fixed->GetDirection() ) ;
  transform->SetTransformDomainMeshSize( meshSize ) ;

  BSplineType::ParametersType parameters( transform->GetNumberOfParameters() ) ;
  
  parameters.Fill( 0.0 ) ;
  transform->SetParameters( parameters ) ;
  registration->SetInitialTransformParameters( transform->GetParameters() ) ;
  registration->SetFixedImageRegion( fixed->GetLargestPossibleRegion() ) ;

  rsgd->SetMaximumStepLength( 0.25 ) ;
  rsgd->SetMinimumStepLength( 0.0625 ) ;
  rsgd->SetNumberOfIterations( 200 ) ;

  try { 
    std::cout << transform->GetNumberOfParameters() << std::endl ;
    registration->Update() ;
  } catch (itk::ExceptionObject & e ) {
    std::cerr << e << std::endl ;
  }

  transform->SetParameters( registration->GetLastTransformParameters() ) ;
  typedef itk::ResampleImageFilter < ImageType, ImageType > ResampleType ;
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

  pthread_exit( NULL );
}
