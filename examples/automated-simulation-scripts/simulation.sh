#!/usr/bin/env bash

contiki_ng_path="$1"
echo "Contiki NG path: $contiki_ng_path"
simulation_file="$2"
echo "Simulation file: $simulation_file"

random_seed_array=("482931" "739204" "156782" "624013" "893726" "215490" "367821" "508237" "741609" "690158")

read -a lines -d EOF < "$simulation_file"

for r in "${random_seed_array[@]}"; do
    for i in "${lines[@]}"; do
        IFS=',' read -ra parts <<< "$i"
        echo "Sending ./automated_interactive_cooja.exp $contiki_ng_path $contiki_ng_path/examples/${parts[0]} rpl-udp-z1.csc ${parts[1]} ${parts[2]} $r"
        ./automated_interactive_cooja.exp $contiki_ng_path $contiki_ng_path/examples/${parts[0]} rpl-udp-z1.csc ${parts[1]} ${parts[2]} $r &
    done
    wait
done
