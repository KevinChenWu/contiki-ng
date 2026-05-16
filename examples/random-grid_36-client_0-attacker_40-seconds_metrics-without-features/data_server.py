#!/usr/bin/python3
import socket
import argparse
import time
import re
from statistics import mode

parser = argparse.ArgumentParser()
parser.add_argument("-c", "--csv_file", action="store", type=str, default="sim.csv", help="CSV file to save the received packets (default: %(default)s)")
parser.add_argument("-w", "--write_csv", action="store_true", default=False, help="Writes the received packets to the CSV file (default: %(default)s)")
args = parser.parse_args()

print(args)
csv_file = args.csv_file
write_csv = args.write_csv

server_ip = "fdff::1"
server_port = 5678
buffer_size = 1024

udp_server = socket.socket(family=socket.AF_INET6, type=socket.SOCK_DGRAM)
udp_server.bind((server_ip, server_port))

metrics_list = ["timestamp_start", "node.id", "seq_no", "node.parent", "node.rank", "node.parent.rank", "node.parent.etx", "node.num_neighbors", "node.dioint", "cpu", "lpm", "deep_lpm", "transmit", "listen", "delta_time", "timestamp_end"]
metrics_dict = {k: "" for k in metrics_list}

if write_csv:
    with open(csv_file, mode="w", encoding="utf-8") as csv_file_handler:
        csv_file_handler.write(",".join(metrics_list) + "\n")

timestamp_dict = {}

print("UDP Data server up and listening")

while True:
    if write_csv:
        with open(csv_file, mode="a", encoding="utf-8") as csv_file_handler:
            data_bytes, address = udp_server.recvfrom(buffer_size)
            current_time = int(time.time())
            node_id = int(address[0].split(":")[-1], 16)
            data = data_bytes.decode()
            if re.match(r"Current clock time: (\d+)", data):
                clock_seconds = int(data.split(": ")[-1])
                timestamp_dict[node_id] = current_time - clock_seconds
                print(f"Node ID: {node_id}, Node IP: \"{address[0]}\", data: \"{data}\", timestamp_dict: \"{timestamp_dict}\"")
            else:
                data_list = data.split(",")
                metrics_dict = {metrics_list[i]: data_list[i].strip() for i in range(len(metrics_list)-1)}
                metrics_dict["timestamp_start"] = str(int(metrics_dict["timestamp_start"]) + timestamp_dict.get(node_id, mode(timestamp_dict.values())))
                metrics_dict["timestamp_end"] = str(current_time)
                csv_file_handler.write(",".join(metrics_dict.values()) + "\n")
                print(f"Node ID: {metrics_dict['node.id']}, Node IP: \"{address[0]}\", data_list: \"{data_list}\", metrics_dict: \"{metrics_dict}\"")
    else:
        data_bytes, address = udp_server.recvfrom(buffer_size)
        current_time = int(time.time())
        node_id = int(address[0].split(":")[-1], 16)
        data = data_bytes.decode()
        if re.match(r"Current clock time: (\d+)", data):
            clock_seconds = int(data.split(": ")[-1])
            timestamp_dict[node_id] = current_time - clock_seconds
            print(f"Node ID: {node_id}, Node IP: \"{address[0]}\", data: \"{data}\", timestamp_dict: \"{timestamp_dict}\"")
        else:
            data_list = data.split(",")
            metrics_dict = {metrics_list[i]: data_list[i].strip() for i in range(len(metrics_list)-1)}
            print(node_id)
            metrics_dict["timestamp_start"] = str(int(metrics_dict["timestamp_start"]) + timestamp_dict.get(node_id, mode(timestamp_dict.values())))
            metrics_dict["timestamp_end"] = str(current_time)
            print(f"Node ID: {metrics_dict['node.id']}, Node IP: \"{address[0]}\", data_list: \"{data_list}\", metrics_dict: \"{metrics_dict}\"")
