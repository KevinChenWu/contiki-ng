#!/usr/bin/env python3

import argparse
import os.path
import sys
import pexpect
import subprocess
import signal
import os
import time

os.setpgrp()
parser = argparse.ArgumentParser()
parser.add_argument("--contiki_ng_path", action="store", type=str, help="Full path to <contiki-ng>")
parser.add_argument("--sim_path",        action="store", type=str, help="Full path to <contiki-ng/examples/*simulation*>")
parser.add_argument("--sim_file",        action="store", type=str, help="CSC file to simulate")
parser.add_argument("--ip_addr",         action="store", type=str, help="First group of IPv6 address to be used")
parser.add_argument("--listen_port",     action="store", type=int, help="Listen port of Contiki-NG Cooja for RPL Border Router to connect")
parser.add_argument("--random_seed",     action="store", type=int, help="Random seed for the simulation of CSC file")
args = parser.parse_args()

contiki_ng_path = args.contiki_ng_path
sim_path = args.sim_path
sim_file = args.sim_file
ip_addr = args.ip_addr
listen_port = args.listen_port
random_seed = args.random_seed

if contiki_ng_path is None:
    print("The dir contiki_ng_path is not provided. Exiting")
    sys.exit(1)
elif not os.path.isdir(contiki_ng_path):
    print(f"The dir {contiki_ng_path} does not exist. Exiting")
    sys.exit(1)

if sim_path is None:
    print("The dir sim_path is not provided. Exiting")
    sys.exit(1)
elif not os.path.isdir(sim_path):
    print(f"The dir {sim_path} does not exist. Exiting")
    sys.exit(1)

sim_file_path = f"{sim_path}/rpl-classic-vna/{sim_file}"
rpl_br_path = f"{sim_path}/rpl-border-router"
udp_wsn_server_path = f"{sim_path}"
password = " "

if sim_file is None:
    print("The file sim_file is not provided. Exiting")
    sys.exit(1)
elif not os.path.isfile(sim_file_path):
    print(f"The file {sim_file_path} does not exist. Exiting")
    sys.exit(1)

if not os.path.isdir(rpl_br_path):
    print(f"The dir {rpl_br_path} does not exist. Exiting")
    sys.exit(1)

if not os.path.isfile(f"{udp_wsn_server_path}/wsn_server.py"):
    print(f"The file {udp_wsn_server_path}/wsn_server.py does not exist. Exiting")
    sys.exit(1)

if ip_addr is None:
    print("The IPv6 address for Python UDP server is not provided. Exiting")
    sys.exit(1)

if listen_port is None:
    print("The listen port of Cooja for RPL Border Router is not provided. Exiting")
    sys.exit(1)

if random_seed is None:
    print("The random seed for the simulation is not provided. Using the default value of CSC file")
elif not (random_seed > 0 and random_seed < 1000000):
    print("The random seed for the simulation should be integer, positive and at most 6 digits. Exiting")
    sys.exit(1)
else:
    print(f"The random seed for the simulation: {random_seed}")

def get_shell():
    terminal = pexpect.spawn("/bin/bash -i", encoding="utf-8", timeout=None)
    terminal.setwinsize(terminal.getwinsize()[0], 1000)
    if not terminal:
        print("Could not run \"/bin/bash\"")
    else:
        print("Interactive shell created")
    prompt = "\\$ "
    idx = terminal.expect([pexpect.TIMEOUT, prompt], timeout=120)
    if idx == 0 and issubclass(cooja_terminal.match, pexpect.exceptions.TIMEOUT):
        text = terminal.before
        print("Timeout while running \"/bin/bash\"")
        print(f"Last text received: \"{text}\"")
        terminal.close(force=True)
        sys.exit(1)
    return terminal

prompt = ">"
cooja_terminal = get_shell()
cooja_terminal.send("PS1='>'\n")
cooja_terminal.send(f"cd {contiki_ng_path}/tools/cooja; pwd\n")
cooja_terminal.expect(f"{prompt}$")
cooja_terminal.send(f"./gradlew run --args='--no-gui --autostart {sim_file_path}'\n")
cooja_terminal.expect("Listening on port: (\\d+)")
print(cooja_terminal.match.group(0))
listen_port_sim = int(cooja_terminal.match.group(1))
if listen_port_sim != listen_port:
    print(f"Listen port from simulation file {listen_port_sim} did not match provided Listen port {listen_port}")
    print("Exiting from Contiki-NG Cooja")
    cooja_terminal.sendcontrol('c')
    cooja_terminal.send("exit\n")
    cooja_terminal.close(force=True)
    sys.exit(1)

cooja_terminal.expect("Waiting 10 seconds for RPL Border Router to start")
print(cooja_terminal.match.group(0))
rpl_br_terminal = subprocess.Popen(f"cd {rpl_br_path}; sudo -S make TARGET=z1 connect-router-cooja PREFIX={ip_addr}::1/64 COOJA_RPL_BR_SERVER_PORT={listen_port}", shell=True, text=True, executable="/bin/bash", stdin=subprocess.PIPE, stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
rpl_br_terminal.stdin.write(' \n')
rpl_br_terminal.stdin.flush()
cooja_terminal.expect("Client connected: /\\d+\\.\\d\\.\\d\\.\\d")
print(cooja_terminal.match.group(0))

cooja_terminal.expect("Waiting 10 seconds for Python UDP WSN Server to start")
print(cooja_terminal.match.group(0))
if random_seed is None:
    udp_wsn_terminal = subprocess.Popen(f"cd {udp_wsn_server_path}; ./wsn_server.py -w -s {ip_addr}::1 -c {ip_addr}.csv", shell=True, text=True, executable="/bin/bash", stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)
else:
    udp_wsn_terminal = subprocess.Popen(f"cd {udp_wsn_server_path}; ./wsn_server.py -w -s {ip_addr}::1 -c {ip_addr}_{random_seed}.csv", shell=True, text=True, executable="/bin/bash", stdout=subprocess.DEVNULL, stderr=subprocess.STDOUT)

cooja_terminal.expect("Starting simulation")
print(cooja_terminal.match.group(0))
simulation_finish = False
while not simulation_finish:
    cooja_terminal.expect("(\\d+% completed, .* sec remaining)|(BUILD SUCCESSFUL)", timeout=None)
    print(cooja_terminal.match.group(0))
    if cooja_terminal.match.group(0) == "BUILD SUCCESSFUL":
        simulation_finish = True

cooja_terminal.expect(f"{prompt}$")
print("Exiting from Contiki-NG Cooja")
cooja_terminal.send("exit\n")
cooja_terminal.close(force=True)
print("Exiting from RPL Border Router")
rpl_br_terminal.kill()
print("Exiting from Python UDP WSN Server")
os.killpg(0, signal.SIGKILL)
#udp_wsn_terminal.kill()
