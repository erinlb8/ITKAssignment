#!/bin/bash
#SBATCH --mail-user=erin.l.bratu@vanderbilt.edu
#SBATCH --mail-type=ALL
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:05:00
#SBATCH --mem=2G
#SBATCH --output=./log_files/sloriginal.log

module purge
module restore cs8395-modules

./ITKAssignment 1 0
