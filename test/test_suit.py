import pytest
import asyncio

async def tcp_connection():
    try:
        reader, writer = await asyncio.open_connection('127.0.0.1', 6667)
    except Exception as e:
        print("exception: ", e)
    writer.close()
    await writer.wait_closed()

@pytest.mark.asyncio
async def test_connection():
    asyncio.run(tcp_connection())
    with open("ft_irc.log", "r") as file:
        content = file.readlines()
    assert content[0] == "new connection with 127.0.0.1:6667"

