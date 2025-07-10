# automated-simulation-scripts

Required packages:
- Python3 Pexpect (command to install: `sudo apt install python3-pexpect`)

## automated-interactive-cooja

Usage of `automated_interactive_cooja.py`:

    >./automated_interactive_cooja.py --contiki_ng_path <full path to contiki-ng> --sim_path <full path to contiki-ng/examples/*simulation*> --sim_file <CSC file to simulate> --ip_addr <first group of IPv6 address to be used> --listen_port <Listen port for RPL Border Router to connect> --random_seed <random seed for the simulation of CSC file>

Note: random_seed is optional
