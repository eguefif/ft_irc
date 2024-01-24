import pytest
import asyncio

@pytest.mark.asyncio
async def test_connection():
    try:
        reader, writer = await asyncio.open_connection('127.0.0.1', 6668)
    except Exception as e:
        print("exception: ", e)
    with open("ft_irc.log", "r") as file:
        content = file.readlines()
    assert content[0] == "new connection with 127.0.0.1:6667"

