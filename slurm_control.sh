#!/bin/bash
# Uses a loop to create dependent jobs for each iteration of the deformable registration
# avg_deform.slurm jobs receive an input used to identify which iteration's average it
# belongs to.
id1=$(sbatch --parsable deform_job.slurm)
sleep 1
echo "${id1}"
id2=$(sbatch --parsable --dependency=afterok:"${id1}" avg_deform.slurm "0")
sleep 1
for x in {1..99} 
do
    id1=$(sbatch --parsable --dependency=afterok:"${id2}" deform_job.slurm)
    sleep 1
    id2=$(sbatch --parsable --dependency=afterok:"${id1}" avg_deform.slurm ${x})
    sleep 1
    echo "Iteration ${x}"
done

