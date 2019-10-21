#!/bin/bash

[[ -d "../ITKAssignment-build" ]] && echo "Directory ITKAssignment-build exists." || mkdir "../ITKAssignment-build"

[[ -d "../ITKAssignment-build/itk-images" ]] && echo "Directory itk-images exists." || mkdir "../ITKAssignment-build/itk-images"

[[ -d "../ITKAssignment-build/registrations" ]] && echo "Directory registrations exists." || mkdir "../ITKAssignment-build/registrations"

[[ -d "../ITKAssignment-build/atlas-images" ]] && echo "Directory atlas-images exists." || mkdir "../ITKAssignment-build/atlas-images"

[[ -d "../ITKAssignment-build/log_files" ]] && echo "Directory log_files exists." || mkdir "../ITKAssignment-build/log_files"

for f in *.slurm
do
    cp -v "$f" ../ITKAssignment-build/"$f"
done

cp -v "slurm_control.sh" "../ITKAssignment-build/slurm_control.sh"
