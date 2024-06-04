#! /bin/sh

for i in $(seq 1 $1); do
    echo "Starting iteration $i"
    cd ./scripts
    python3 ./tcpSessionApp_sendscript_generator.py ${i}
    cd ..
    inet -u Cmdenv -c EthernetWithPriority --seed-set=${i} --output-scalar-file results/EthernetWithPriority${i}.sca --output-vector-file results/EthernetWithPriority${i}.vec
    echo "Completed iteration $i"
done
