import time
import pytest
import pytest_asyncio
import asyncio
from settings import *


test_command = [
        (b"NICK " + SEP, f"{PREFIX} 431 :No nickname given", "Invalid NICK command: 127.0.0.1"),
        (b"NICK 1234567890" + SEP, f"{PREFIX} 432 * 1234567890 :Erroneous Nickname", "Invalid NICK command: 127.0.0.1"),
        (b"NICK 123456789 0123" + SEP, f"{PREFIX} 432 * 1234567890 0123 :Erroneous Nickname", "Invalid NICK command: 127.0.0.1"),
        ]

async def get_read_content(reader):
    try:
        async with asyncio.timeout(0.1):
            retval = await reader.readline()
            retval2 = await reader.readline()
    except TimeoutError:
        return ""
    return [retval, retval2]
    
@pytest.mark.asyncio
@pytest.mark.parametrize("command,server_return,log", test_command)
async def test_errors(clean_log, command, server_return, log):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    _ = await reader.readline()
    writer.write(command)
    retval = await get_read_content(reader)
    writer.close()
    await writer.wait_closed()
    with open("../ft_irc.log", "r") as file:
        content = file.readlines()
    assert len(content) >= 3, "nothing in log file"
    if content and len(content) >= 3:
        to_cmp = content[1].strip()
        to_cmp = to_cmp.split("] ")
        assert to_cmp[1] == log
        print(retval)
        if len(server_return):
            assert retval[0] == server_return
            assert len(retval[1]) == 0
        else:
            assert len(retval) == 0

