import pytest
import asyncio
import time

HOST = "127.0.0.1"
PORT = 6933

def clean_log_file():
    with open("../ft_irc.log", "w") as file:
        file.write("")

async def tcp_connection(depth=1):
    print("test", depth)
    reader, writer = await asyncio.open_connection(HOST, PORT)
    if depth > 1:
        await tcp_connection(depth - 1)
    writer.close()
    await writer.wait_closed()

@pytest.mark.asyncio
async def test_connection():
    clean_log_file()
    await tcp_connection()
    time.sleep(1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    if content:
        to_cmp = content[0].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == f"new connection with {HOST}"
    clean_log_file()

@pytest.mark.asyncio
async def test_multiple_connection():
    clean_log_file()
    await tcp_connection(depth=5)
    time.sleep(1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    if content:
        assert len(content) == 10
        for line in content:
            if line.find("disconnected") != -1:
                continue
            to_cmp = line.strip()
            to_cmp = to_cmp.split("] ")
            assert to_cmp[1] == f"new connection with {HOST}"
    clean_log_file()

@pytest.mark.asyncio
async def client_welcome_msg():
    error = 1
    clean_log_file()
    reader, writer = await asyncio.open_connection(HOST, PORT)
    data = ""
    try:
        async with asyncio.timeout(1):
            data = await reader.readuntil(separator=b'\n')
    except TimeoutError:
        error = 0
    else:
        data = data.decode()
        assert error
        assert data == "Welcome to IRC\n"
    finally:
        writer.close()
        await writer.wait_closed()

@pytest.mark.asyncio
async def test_commandes():
    clean_log_file()
    reader, writer = await asyncio.open_connection(HOST, PORT)
    writer.write(b"NICK test\n")
    await writer.drain()
    writer.close()
    await writer.wait_closed()
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
        print(content)
    assert content, "nothing in log file"
    if content:
        to_cmp = content[1].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == f"new user nickname test"
