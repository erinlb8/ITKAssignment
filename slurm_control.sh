#!/bin/bash

id1=$(sbatch --parsable deform_job.slurm)
sleep 2
echo "${id1}"
id2=$(sbatch --parsable --dependency=afterok:"${id1}" avg_deform.slurm "0")
sleep 2
for x in {1..50} 
do
    id1=$(sbatch --parsable --dependency=afterok:"${id2}" deform_job.slurm)
    sleep 2
    id2=$(sbatch --parsable --dependency=afterok:"${id1}" avg_deform.slurm ${x})
    sleep 2
    echo "Iteration ${x}"
done

