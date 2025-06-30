#!/usr/bin/python3
import socket
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-c", "--csv_file", action="store", type=str, default="sim.csv", help="CSV file to save the received packets (default: %(default)s)")
parser.add_argument("-s", "--server_ip", action="store", type=str, default="fd00::1", help="Server IP to receive packets (default: %(default)s)")
parser.add_argument("-w", "--write_csv", action="store_true", default=False, help="Writes the received packets to the CSV file (default: %(default)s)")
args = parser.parse_args()

print(args)
csv_file = args.csv_file
server_ip = args.server_ip
write_csv = args.write_csv

server_port = 5678
buffer_size = 1024

udp_server = socket.socket(family=socket.AF_INET6, type=socket.SOCK_DGRAM)
udp_server.bind((server_ip, server_port))

features_list = ["dio.version", "dio.rank", "frame.len", "6lowpan.src", "6lowpan.dst", "dio.dtsn", "dao.sequence", "ipv6.hlim", "wpan.seq_no", "ipv6.plen", "icmpv6.code", "wpan.ack_request"]
features_dict = {k: "" for k in features_list}

if write_csv:
    with open(csv_file, mode="w", encoding="utf-8") as csv_file_handler:
        csv_file_handler.write(",".join(features_list) + "\n")

print("UDP WSN server up and listening")

while True:
    if write_csv:
        with open(csv_file, mode="a", encoding="utf-8") as csv_file_handler:
            data_bytes, address = udp_server.recvfrom(buffer_size)
            node_id = int(address[0].split(":")[-1], 16)
            data = data_bytes.decode()
            data_list = data.split(",")
            data_header, data_list = data_list[0], data_list[1:]
            data_header_bin = f"{int(data_header, 16):016b}"
            features_dict = {features_list[i]: data_list[i].strip() if int(data_header_bin[i]) else "" for i in range(len(features_list))}
            csv_file_handler.write(",".join(features_dict.values()) + "\n")
            print(f"Node ID: {node_id}, Node IP: \"{address[0]}\", data_header: \"{data_header}\", data_header_bin: \"{data_header_bin}\", data_list: \"{data_list}\", features_dict: \"{features_dict}\"")
    else:
        data_bytes, address = udp_server.recvfrom(buffer_size)
        node_id = int(address[0].split(":")[-1], 16)
        data = data_bytes.decode()
        data_list = data.split(",")
        data_header, data_list = data_list[0], data_list[1:]
        data_header_bin = f"{int(data_header, 16):016b}"
        features_dict = {features_list[i]: data_list[i].strip() if int(data_header_bin[i]) else "" for i in range(len(features_list))}
        print(f"Node ID: {node_id}, Node IP: \"{address[0]}\", data_header: \"{data_header}\", data_header_bin: \"{data_header_bin}\", data_list: \"{data_list}\", features_dict: \"{features_dict}\"")
