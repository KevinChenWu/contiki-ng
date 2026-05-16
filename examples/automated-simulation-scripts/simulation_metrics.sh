#!/usr/bin/env bash

contiki_ng_path="$1"
echo "Contiki NG path: $contiki_ng_path"
simulation_file="$2"
echo "Simulation file: $simulation_file"
stack_step="$3"
if [ -z $3 ]; then
    stack_step=1
fi
echo "Stack step: $stack_step"

read -a lines -d EOF < "$simulation_file"
sim_len=${#lines[@]}
slice=($(seq 0 $stack_step $(($sim_len-1))))

for stack_start in "${slice[@]}"; do
    stack_stop=$(($stack_start + $stack_step-1))
    if [ $stack_stop -ge $sim_len ]; then
        stack_stop=$(($sim_len-1))
    fi
    stack=($(seq $stack_start $stack_stop))
    for s in "${stack[@]}"; do
        IFS=',' read -ra parts <<< "${lines[$s]}"
        echo "Sending ./automated_interactive_cooja_metrics.py --contiki_ng_path $contiki_ng_path --sim_path $contiki_ng_path/examples/${parts[0]} --sim_file rpl-udp-z1.csc --ip_addr ${parts[1]} --listen_port ${parts[2]}"
            ./automated_interactive_cooja_metrics.py --contiki_ng_path $contiki_ng_path --sim_path $contiki_ng_path/examples/${parts[0]} --sim_file rpl-udp-z1.csc --ip_addr ${parts[1]} --listen_port ${parts[2]} &
    done
    wait
done
