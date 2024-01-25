import pytest
import asyncio

HOST = "127.0.0.1"
PORT = 6670


async def tcp_connection():
    reader, writer = await asyncio.open_connection(HOST, PORT)
    writer.close()
    await writer.wait_closed()

@pytest.mark.asyncio
async def test_connection():
    await tcp_connection()
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert content, "nothing in log file"
    print(content)
    if content:
        assert content[0].strip() == f"new connection with {HOST}:{PORT}"
