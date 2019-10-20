#!/bin/bash
id1=$(sbatch --parsable deform_job.slurm)
sleep 1
echo "${id1}"
id2=$(sbatch --parsable --dependency=afterok:"${id1}" avg_deform.slurm "1")
sleep 1
for x in {2..10} 
do
    id1=$(sbatch --parsable --dependency=afterok:"${id2}" deform_job.slurm)
    sleep 1
    id2=$(sbatch --parsable --dependency=afterok:"${id1}" avg_deform.slurm ${x})
    sleep 1
    echo "Iteration ${x}"
done

