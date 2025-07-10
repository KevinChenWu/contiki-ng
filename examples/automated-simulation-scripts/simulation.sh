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
        echo "Sending ./automated_interactive_cooja.py --contiki_ng_path $contiki_ng_path --sim_path $contiki_ng_path/examples/${parts[0]} --sim_file rpl-udp-z1.csc --ip_addr ${parts[1]} --listen_port ${parts[2]} --random_seed $r"
        ./automated_interactive_cooja.py --contiki_ng_path $contiki_ng_path --sim_path $contiki_ng_path/examples/${parts[0]} --sim_file rpl-udp-z1.csc --ip_addr ${parts[1]} --listen_port ${parts[2]} --random_seed $r &
    done
    wait
done
