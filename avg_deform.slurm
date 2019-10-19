#!/bin/bash
#SBATCH --mail-user=erin.l.bratu@vanderbilt.edu
#SBATCH --mail-type=FAIL
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:10:00
#SBATCH --mem=2G
#SBATCH --output=./log_files/slde_avg.log

module purge
module restore cs8395-modules

./ITKAssignment 4 $1 

