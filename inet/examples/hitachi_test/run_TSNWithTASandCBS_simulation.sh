#! /bin/sh

for i in $(seq 1 $1); do
    echo "Starting iteration $i"
    cd ./scripts
    python3 ./tcpSessionApp_sendscript_generator.py ${i}
    cd ..
    inet -u Cmdenv -c TSNWithTASandCBS --seed-set=${i} --output-scalar-file results/TSNWithTASandCBS${i}.sca --output-vector-file results/TSNWithTASandCBS${i}.vec
    echo "Completed iteration $i"
done
