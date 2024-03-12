#! /bin/sh
python_script="./tcpSessionApp_sendscript_generator.py"

for i in {1..2}; do
    cd ./scripts
    python3 "$python_script"
    cd ..
    inet -u Cmdenv -c ThreeBaysUnderProvisioned --seed-set=${i} --output-scalar-file results/ThreeBaysUnderProvisioned${i}.sca --output-vector-file results/ThreeBaysUnderProvisioned${i}.vec
done
