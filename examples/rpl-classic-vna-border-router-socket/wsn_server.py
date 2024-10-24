#!/usr/bin/python3
import socket

server_ip = "fd00::1"
server_port = 5678
buffer_size = 1024

udp_server = socket.socket(family=socket.AF_INET6, type=socket.SOCK_DGRAM)
udp_server.bind((server_ip, server_port))

print("UDP server up and listening")

while True:
    data_bytes, address = udp_server.recvfrom(buffer_size)
    node_id = address[0].split(":")[-1]
    data = data_bytes.decode()
    print(f"Node ID: {node_id}, Node IP: \"{address[0]}\", data: \"{data}\"")
