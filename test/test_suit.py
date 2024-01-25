import pytest
import socket

HOST = "127.0.0.1"
PORT = 6670

def tcp_connection():
    addr = (HOST, PORT)
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.setblocking(False)
    sock.connect_ex(addr)
    sock.close()

def test_connection():
    tcp_connection()
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    print(content)
    if content:
        assert content[0].strip() == f"new connection with {HOST}:{PORT}"
