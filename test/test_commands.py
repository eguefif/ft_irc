import pytest
import pytest_asyncio
import asyncio
from settings import *
import time

@pytest.mark.asyncio
async def test_authentification(clean_log):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)
    time.sleep(0.1)
    writer.close()
    await writer.wait_closed()
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert len(content) >= 2, "nothing in log file"
    if content and len(content) >= 3:
        to_cmp = content[1].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == "user authenticated " + HOST

test_command = [
        ("NICK Emmanuel" + SEP, "", "user nickname set to Emmanuel " + HOST),
        (":allo NICK Emmanuel" + SEP, "", "user nickname set to Emmanuel " + HOST),
        (":allo NICK Emmanuel " + SEP, "", "user nickname set to Emmanuel " + HOST),
        (":allo  NICK  Emmanuel" + SEP, "", "user nickname set to Emmanuel " + HOST),
        (":allo NICK  Emmanuel" + SEP, "", "user nickname set to Emmanuel " + HOST),
        ("USER test * 0 :Robby Williams" + SEP, "", "user set to test (Robby Williams) " + HOST),
        ]

@pytest.mark.asyncio
@pytest.mark.parametrize("command,server_return,log", test_command)
async def test_commands(clean_log, command, server_return, log):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)
    writer.write(command.encode())
    await writer.drain()
    writer.close()
    await writer.wait_closed()
    time.sleep(0.1)
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    print(content)
    assert len(content) >= 3, "nothing in log file"
    if content and len(content) >= 3:
        to_cmp = content[2].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == log

@pytest.mark.asyncio
async def test_register(clean_log):
    error = 1
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)
    await register(writer, "Test", "Robby Williams", "Ben")
    time.sleep(0.2)
    data = ""
    try:
        async with asyncio.timeout(0.2):
            data = await reader.readline()
            data2 = await reader.readline()
    except TimeoutError:
        error = 0
    else:
        data2 = data2.decode()
        data = data.decode()
        assert len(data2) == 0, "Too much data received"
        assert error
        assert len(data) == len("Welcome to IRC!\n")
        assert data == "Welcome to IRC!\n"
    finally:
        writer.close()
        await writer.wait_closed()

@pytest.mark.asyncio
async def test_register_opposite(clean_log):
    error = 1
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)

    command = f"USER Test2 0 * : Robby Wiliams{SEP}"
    writer.write(command.encode())
    await writer.drain()
    command = f"NICK robby2{SEP}"
    writer.write(command.encode())
    await writer.drain()
    time.sleep(0.2)

    data = ""
    try:
        async with asyncio.timeout(0.2):
            data = await reader.readline()
            data2 = await reader.readline()
    except TimeoutError:
        error = 0
    else:
        data2 = data2.decode()
        data = data.decode()
        assert len(data2) == 0, "Too much data received"
        assert error
        assert len(data) == len("Welcome to IRC!\n")
        assert data == "Welcome to IRC!\n"
    finally:
        writer.close()
        await writer.wait_closed()

@pytest.mark.asyncio
async def test_change_nickname(clean_log):
    reader, writer = await log_user_test("Manu", "test", "Emmanuel Guefif")
    time.sleep(0.3)
    command = f"NICK Manu2{SEP}"
    writer.write(command.encode())
    await writer.drain()
    writer.close()
    await writer.wait_closed()
    time.sleep(0.3)

    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
        print(content)
    assert len(content), "nothing in log file"
    if content and len(content) >= 3:
        to_cmp = content[-2].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == "user nickname set to Manu2 " + HOST
