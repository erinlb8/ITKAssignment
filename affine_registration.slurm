#!/bin/bash
#SBATCH --mail-user=erin.l.bratu@vanderbilt.edu
#SBATCH --mail-type=ALL
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --output=slaffine.log
#SBATCH --job-name=affine_registration

module purge
module restore cs8395-modules

./ITKAssignment 2 0
