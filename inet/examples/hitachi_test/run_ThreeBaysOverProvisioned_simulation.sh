#! /bin/sh

for i in $(seq 1 $1); do
    echo "Starting iteration $i"
    cd ./scripts
    python3 ./tcpSessionApp_sendscript_generator.py ${i}
    cd ..
    inet -u Cmdenv -c ThreeBaysOverProvisioned --seed-set=${i} --output-scalar-file results/ThreeBaysOverProvisioned${i}.sca --output-vector-file results/ThreeBaysOverProvisioned${i}.vec
    echo "Completed iteration $i"
done
