#! /bin/sh
python_script="./tcpSessionApp_sendscript_generator.py"

for i in {1..2}; do
    cd ./scripts
    python3 "$python_script"
    cd ..
    inet -u Cmdenv -c ThreeBaysOverProvisioned --seed-set=${i} --output-scalar-file results/ThreeBaysOverProvisioned${i}.sca --output-vector-file results/ThreeBaysOverProvisioned${i}.vec
done
