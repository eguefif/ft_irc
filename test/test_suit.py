import pytest
import pytest_asyncio
import asyncio
import time
from settings import *


async def tcp_connection(depth=1):
    print("test", depth)
    reader, writer = await asyncio.open_connection(HOST, PORT)
    if depth > 1:
        await tcp_connection(depth - 1)
    writer.close()
    await writer.wait_closed()

@pytest.mark.asyncio
async def test_connection(clean_log):
    await tcp_connection()
    time.sleep(1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    if content:
        to_cmp = content[0].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == f"new connection with {HOST}"
    
@pytest.mark.asyncio
async def test_multiple_connection(clean_log):
    max_depth = 5
    await tcp_connection(depth=max_depth)
    time.sleep(1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    if content:
        assert len(content) == max_depth * 2
        for i, line in enumerate(content):
            to_cmp = line.strip()
            to_cmp = to_cmp.split("] ")
            if i < max_depth:
                assert to_cmp[1] == f"new connection with {HOST}"
            else:
                assert to_cmp[1] == f"{HOST} disconnected"
    
@pytest.mark.asyncio
async def test_client_welcome_msg(clean_log):
    error = 1
    reader, writer = await asyncio.open_connection(HOST, PORT)
    data = ""
    try:
        async with asyncio.timeout(1):
            data = await reader.readline()
            data2 = await reader.readline()
    except TimeoutError:
        error = 0
    else:
        data = data.decode()
        assert len(data2) == 0, "Too much data received"
        assert error
        assert len(data) == len("Welcome to IRC\n")
        assert data == "Welcome to IRC\n"
    finally:
        writer.close()
        await writer.wait_closed()

@pytest.mark.asyncio
async def test_mass_connections_disconnections(clean_log):
    max_connection = 1000
    for i in range(max_connection):
        reader, writer = await asyncio.open_connection(HOST, PORT)
        writer.close()
        await writer.wait_closed()
    with open("../ft_irc.log", "r") as file:
        lines = file.readlines()
    assert i == max_connection - 1
    assert len(lines) / 2 == max_connection

'''
message_split= [
        (b"USER test * 0: Emmanuel Guefif" + SEP, "USER test * 0 Emmanuel Guefif"),
        (b"USER   test   * 0:        Emmanuel Guefif" + SEP, "USER test * 0 Emmanuel Guefif"),
        (b"USER test * 0:Emmanuel Guefif                  " + SEP, "USER test * 0 Emmanuel Guefif"),
        ]

@pytest.mark.asyncio
@pytest.mark.parametrize("command,log", message_split)
async def test_message_split(message, command, log):
        reader, writer = await asyncio.open_connection(HOST, PORT)
        writer.write(command)
        data = await reader.readline()
        data2 = await reader.readline()
        writer.close()
        await writer.wait_closed()

        assert data.strip() == log
        assert len(data2) == 0
'''
