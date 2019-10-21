# ITKAssignment
ITK assignment for CS 8395, Vanderbilt University, Fall 2019

## Overview
The repository contains files for performing affine and deformable registrations of images and averaging the results using ITK

## Files
### C++ Files:
- ITKAssignment.cxx : Driver file, contains the main method
- AffineRegistration.cxx : Performs affine registration
- DeformableRegistration.cxx : Performs deformable registration using the Demons method

### Slurm Files:
- original_average.slurm : Submits a SLURM job to average the original image files
- affine_registration.slurm : Submits a SLURM job to perform affine registration and average the results
- deform_job.slurm : Submits a SLURM job array (size 21) to perform deformable registration on each image
- avg_deform.slurm : Submits a SLURM job to average the deformable registrations

### Shell Scripts:
- setup.sh : Creates the build directory and subdirectories and copies the slurm and shell scripts to the build directory
- slurm_control.sh : Controls submission of SLURM jobs for multiple iterations of deformable registrations by alternating between registration and averaging, with each job being dependent upon successful completion of the previous job. (Default iterations: 100)

## Setup
- Running setup.sh will take care of creating the directories in the build folder
- The user must run "ccmake ../ITKAssignment" from the build directory (ITKAssignment-build)
- The program expects to find the original image files in the directory "itk-images"

## Execution
- The first script to execute is original_average.slurm, followed by affine_registration.slurm using "sbatch slurm_script_name_here"
- Following the successful execution of the affine registrations, the main build directory should contain files "initialAverage.nii.gz" and "affineAverage.nii.gz".
- The shell script slurm_control.sh is used to create multiple iterations of deformable registrations using "./slurm_control.sh". The number of iterations can be edited by editing the file and changing the maximum value of the for loop.
- If only one iteration is to be performed, simply run deform_job.slurm, followed by avg_deform.slurm after successful execution.
