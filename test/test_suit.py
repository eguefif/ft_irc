import pytest
import asyncio
import time

HOST = "127.0.0.1"
PORT = 6670

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
    await tcp_connection()
    time.sleep(1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    if content:
        to_cmp = content[0].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == f"new connection with {HOST}:{PORT}"
    clean_log_file()

@pytest.mark.asyncio
async def test_multiple_connection():
    await tcp_connection(depth=5)
    time.sleep(1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    if content:
        assert len(content) == 5
        for line in content:
            to_cmp = line.strip()
            to_cmp = to_cmp.split("] ")
            assert to_cmp[1] == f"new connection with {HOST}:{PORT}"
    clean_log_file()

@pytest.mark.asyncio
async def test_client_welcome_msg():
    reader, writer = await asyncio.open_connection(HOST, PORT)
    data = ""
    try:
        async with asyncio.timeout(1):
            data = await reader.readuntil(separator=b'\n')
    except TimeoutError:
        assert len(data), "Timeout"
        return
    data = data.decode()
    writer.close()
    await writer.wait_closed()
    assert data == "Welcome to IRC\n"
