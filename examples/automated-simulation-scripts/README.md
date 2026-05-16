# automated-simulation-scripts

Required packages:
- Python3 Pexpect (command to install: `sudo apt install python3-pexpect`)

## automated-interactive-cooja

Usage of `automated_interactive_cooja.py`:

    >./automated_interactive_cooja.py --contiki_ng_path <full path to contiki-ng> --sim_path <full path to contiki-ng/examples/*simulation*> --sim_file <CSC file to simulate> --ip_addr <first group of IPv6 address to be used> --listen_port <Listen port for RPL Border Router to connect> --random_seed <random seed for the simulation of CSC file>

Note: random_seed is optional

## simulation

Usage of `simulation.sh`:

    >./simulation.sh <full path to contiki-ng> simulation.csv <max number of cases of simulation running in parallel per random seed>

## automated-interactive-cooja-metrics

Usage of `automated_interactive_cooja_metrics.py`:

    >./automated_interactive_cooja_metrics.py --contiki_ng_path <full path to contiki-ng> --sim_path <full path to contiki-ng/examples/*simulation*> --sim_file <CSC file to simulate> --ip_addr <first group of IPv6 address to be used> --listen_port <Listen port for RPL Border Router to connect>

## simulation-metrics

Usage of `simulation_metrics.sh`:

    >./simulation_metrics.sh <full path to contiki-ng> simulation_metrics.csv
