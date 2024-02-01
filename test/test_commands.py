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
        welcome_msg = f"{PREFIX} 001 Welcome to IRC!{SEP}"
        data2 = data2.decode()
        data = data.decode()
        assert len(data2) == 0, "Too much data received"
        assert error
        assert len(data) == len(welcome_msg)
        assert data == welcome_msg
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
    time.sleep(0.3)

    try:
        async with asyncio.timeout(0.2):
            data = await reader.readline()
            data2 = await reader.readline()
    except TimeoutError:
        error = 0
    else:
        confirm = f"{PREFIX} 001 Manu2 :Your nickname was changed{SEP}"
        data2 = data2.decode()
        data = data.decode()
        assert len(data2) == len(confirm)
        assert data2 == confirm
    finally:
        writer.close()
        await writer.wait_closed()

    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
        print(content)
    assert len(content), "nothing in log file"
    if content and len(content) >= 3:
        to_cmp = content[-2].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == "user nickname set to Manu2 " + HOST

@pytest.mark.asyncio
async def test_create_channel(clean_log):
    reader, writer = await log_user_test("test", "user", "robert malin")
    cmd = f"JOIN #chantest{SEP}"
    writer.write(cmd.encode())
    await writer.drain()
    time.sleep(0.1)

    rep1 = ""
    rep2 = ""
    rep3 = ""
    try:
        async with asyncio.timeout(0.2):
            _ = await reader.readline()
            rep1 = await reader.readline()
            rep2 = await reader.readline()
            rep3 = await reader.readline()
    except TimeoutError:
        error = 0
        assert len(rep1) != 0, "no server reply"
        assert len(rep2) != 0, "no server reply"
        assert len(rep3) != 0, "no server reply"
    else:
        confirm1 = f":test JOIN :#chantest{SEP}"
        confirm2 = f"{PREFIX} 353 test = #chantest :@test{SEP}"
        confirm3 = f"{PREFIX} 366 test #chantest :End of /NAMES list{SEP}"
        rep1 = rep1.decode()
        rep2 = rep2.decode()
        rep3 = rep3.decode()
        assert len(rep1) != 0, "no server reply"
        assert len(rep2) != 0, "no server reply"
        assert len(rep3) != 0, "no server reply"
        assert rep1 == confirm1
        assert rep2 == confirm2
        assert rep3 == confirm3
    finally:
        writer.close()
        await writer.wait_closed()

'''
@pytest.mark.asyncio
async def test_join_channel(clean_log):
    readerTmp, writerTmp = await log_user_test("test", "user", "robert malin")
    cmd = f"JOIN #chantest{SEP}"
    writerTmp.write(cmd.encode())
    await writerTmp.drain()
    reader, writer = await log_user_test("test2", "user2", "robert malin2")
    writer.write(cmd.encode())
    await writer.drain()
    time.sleep(0.1)

    rep1 = ""
    rep2 = ""
    rep3 = ""
    try:
        async with asyncio.timeout(0.4):
            # for _ in range(5):
            _ = await reader.readline()
            rep1 = await reader.readline()
            rep2 = await reader.readline()
            rep3 = await reader.readline()
    except TimeoutError:
        error = 0
        assert len(rep1) != 0, "no server reply"
        assert len(rep2) != 0, "no server reply"
        assert len(rep3) != 0, "no server reply"
    else:
        confirm1 = f":test2 JOIN :#chantest{SEP}"
        print(rep2)
        confirm2 = f"{PREFIX} 353 test2 = #chantest :@test test2{SEP}"
        confirm3 = f"{PREFIX} 366 test2 #chantest :End of /NAMES list{SEP}"
        assert len(rep1) != 0, "no server reply"
        assert len(rep2) != 0, "no server reply"
        assert len(rep3) != 0, "no server reply"
        rep1 = rep1.decode()
        rep2 = rep2.decode()
        rep3 = rep3.decode()
        assert rep1 == confirm1
        assert rep2 == confirm2
        assert rep3 == confirm3
    finally:
        writerTmp.close()
        await writerTmp.wait_closed()
        writer.close()
        await writer.wait_closed()
'''
