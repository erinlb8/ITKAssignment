// Resources Used: In-class code and ITK Documentation

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkResampleImageFilter.h"
#include "itkAffineTransform.h"
#include "itkMultiResolutionImageRegistrationMethod.h"
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkLinearInterpolateImageFunction.h"
#include "itkRegularStepGradientDescentOptimizer.h"
#include <string>

const unsigned int nDims = 3 ;
//std::mutex fixed_image_mutex ;
typedef itk::Image< float, nDims > ImageType;


class OptimizationObserver : public itk::Command {

public:
  typedef OptimizationObserver Self ;
  typedef itk::Command Superclass ;
  typedef itk::SmartPointer < Self > Pointer ;
  itkNewMacro( Self ) ;
  
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType ;
  typedef const OptimizerType * OptimizerPointerType ;

  void Execute(itk::Object *caller, const itk::EventObject &event) {
    Execute ( (const Object *) caller, event ) ;
  }

  void Execute(const itk::Object *caller, const itk::EventObject &event) {
    OptimizerPointerType optimizer = dynamic_cast < OptimizerPointerType > ( caller ) ;
  }

protected:
  OptimizationObserver() {};
  
};

class RegistrationObserver : public itk::Command {

public:
  typedef RegistrationObserver Self ;
  typedef itk::Command Superclass ;
  typedef itk::SmartPointer < Self > Pointer ;
  itkNewMacro( Self ) ;
  
  typedef itk::RegularStepGradientDescentOptimizer OptimizerType ;
  typedef OptimizerType * OptimizerPointerType ;
  typedef itk::MultiResolutionImageRegistrationMethod < ImageType, ImageType > RegWrapperType ;
  typedef RegWrapperType * RegPointerType ;

  void Execute(itk::Object *caller, const itk::EventObject &event) {
    RegPointerType regWrapper = dynamic_cast < RegPointerType > ( caller ) ;
    OptimizerPointerType rsgd = dynamic_cast < OptimizerPointerType > ( regWrapper->GetModifiableOptimizer() ) ;
    
    //std::cout << "RegObserver" << rsgd->GetCurrentIteration() << std::endl ;
    
    int level = regWrapper->GetCurrentLevel() ;
    if ( level == 0 )
       rsgd->SetMaximumStepLength ( 0.0625 ) ;
    else if (level == 1 )
       rsgd->SetMaximumStepLength ( 0.125 / 2 ) ;
    else
       rsgd->SetMaximumStepLength ( 0.25 / 2 ) ;
  }

  void Execute(const itk::Object *caller, const itk::EventObject &event) {
 
  }

protected:
  RegistrationObserver() {};
  
};

void affineRegistration( int id, std::string inFixed, std::string inMoving, std::string outImage )
{
  // Setup types
  // Create and setup a reader for moving image
  typedef itk::ImageFileReader< ImageType >  readerType;
  readerType::Pointer reader = readerType::New(); 
  reader->SetFileName( inMoving );
  reader->Update();
  ImageType::Pointer movingImage = reader->GetOutput() ;

  // Same for the fixed image
  readerType::Pointer reader2 = readerType::New() ;
  reader2->SetFileName ( inFixed ) ;
  reader2->Update() ;
  ImageType::Pointer fixedImage = reader2->GetOutput() ;
  
  // Register images
  // Set up typedefs
  typedef itk::AffineTransform < double, 3 > AffineType ;
  typedef itk::MultiResolutionImageRegistrationMethod < ImageType, ImageType > RegWrapperType ;
  typedef itk::MeanSquaresImageToImageMetric < ImageType, ImageType > MeanSquares ;
  typedef itk::LinearInterpolateImageFunction < ImageType, double > LinInterpolate ;
  typedef itk::RegularStepGradientDescentOptimizer RSGDOpt ;

  // Declare the variables
  RegWrapperType::Pointer regWrapper = RegWrapperType::New() ;
  AffineType::Pointer affine = AffineType::New() ;
  MeanSquares::Pointer msquares = MeanSquares::New() ;
  LinInterpolate::Pointer linear = LinInterpolate::New() ;
  RSGDOpt::Pointer rsgd = RSGDOpt::New() ;

  // Optimization Observer
  OptimizationObserver::Pointer observer = OptimizationObserver::New() ;
  rsgd->AddObserver ( itk::IterationEvent(), observer );

  // Registration Observer
  RegistrationObserver::Pointer rObserver = RegistrationObserver::New() ;
  regWrapper->AddObserver( itk::IterationEvent(), rObserver ) ;
  // Connect the pipeline
  affine->SetIdentity() ;
 
  regWrapper->SetMovingImage( movingImage ) ;
  regWrapper->SetFixedImage( fixedImage ) ;
  regWrapper->SetTransform( affine ) ;
  regWrapper->SetMetric( msquares ) ;
  regWrapper->SetInterpolator( linear ) ;
  regWrapper->SetOptimizer( rsgd ) ;
  regWrapper->SetInitialTransformParameters( affine->GetParameters() ) ;
  regWrapper->SetFixedImageRegion( fixedImage->GetLargestPossibleRegion() );
  regWrapper->SetNumberOfLevels ( 3 ) ; 
  // Run the registration
 
  try 
    {  
      regWrapper->Update() ;
    }
  catch( itk::ExceptionObject & excp ) 
    {
      std::cerr << "Error in registration" << std::endl;  
      std::cerr << excp << std::endl; 
    }
 
  // Update the transform
  affine->SetParameters( regWrapper->GetLastTransformParameters() ) ;

  // Apply the transform
  typedef itk::ResampleImageFilter < ImageType, ImageType > ResampleFilterType ;
  ResampleFilterType::Pointer filter = ResampleFilterType::New() ;
  filter->SetInput ( movingImage ) ;

  filter->SetTransform ( affine ) ;
  filter->SetSize ( movingImage->GetLargestPossibleRegion().GetSize() ) ;
  filter->SetReferenceImage ( fixedImage ) ;
  filter->UseReferenceImageOn() ;
  filter->Update() ;

  // write out the result to argv[3]
  typedef itk::ImageFileWriter < ImageType > writerType ;
  writerType::Pointer writer = writerType::New() ;
  writer->SetInput ( filter->GetOutput() ) ; 
  writer->SetFileName ( outImage ) ;
  writer->Update() ;

  // Done.
  return ;
}
