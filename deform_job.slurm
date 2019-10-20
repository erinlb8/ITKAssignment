#!/bin/bash
#SBATCH --mail-user=erin.l.bratu@vanderbilt.edu
#SBATCH --mail-type=FAIL
#SBATCH --nodes=1
#SBATCH --ntasks=1
#SBATCH --time=00:20:00
#SBATCH --mem=16G
#SBATCH --output=./log_files/slde_%a.log
#SBATCH --array=0-20

module purge
module restore cs8395-modules

./ITKAssignment 3 ${SLURM_ARRAY_TASK_ID} 

