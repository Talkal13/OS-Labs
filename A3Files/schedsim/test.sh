

echo "Enter file name: "
read filename

while [ ! -f $filename ]; do
    echo "ERROR: The file does not exist!"
    echo "Enter file name: "
    read filename;
done

echo "Enter max number CPUs: "
read nr_cpus;

while [ $nr_cpus -gt 8 -o $nr_cpus -lt 1 ]; do 
    echo "ERROR: Invalid number of CPUs"
    echo "Enter max number CPUs: "
    read nr_cpus;
done

if [ -d ./results ]; then
    rm -rf ./results    
fi 
mkdir ./results
output=$(./schedsim -L | tail -n +2);
read -r line <<< $output
while read -r sched; do
    for cpus in $(seq 1 $nr_cpus); do
        ./schedsim -n $cpus -i $filename;
        for i in $(seq 0 $(expr $cpus - 1)); do
            mv CPU_$i.log results/$sched-CPU-$i.log
        done
    done
done <<< $output;
    