import time
import pytest
import pytest_asyncio
import asyncio
from settings import *


test_command = [
        ("NICK " + SEP, f"{PREFIX} 431 :No nickname given"),
        ("NICK" + SEP, f"{PREFIX} 431 :No nickname given"),
        ("NICK 1234567890" + SEP, f"{PREFIX} 432 * 1234567890 :Erroneous nickname"),
        ("NICK 123456789 0123" + SEP, f"{PREFIX} 432 * 123456789 0123 :Erroneous nickname"),
        ("NICK :test test" + SEP, f"{PREFIX} 425 * test test :Invalid char detected"),
        ("NICK : test allo" + SEP, f"{PREFIX} 432 *  test allo :Erroneous nickname"),
        ("USER " + SEP, f"{PREFIX} 461 * :Not enough parameters"),
        ("USER yoo * 0 :fda fsa\t" + SEP, f"{PREFIX} 425 * yoo * 0 fda fsa\t :Invalid char detected"),
        ("USER yoo * 0 :fda fsa\037" + SEP, f"{PREFIX} 425 * yoo * 0 fda fsa\037 :Invalid char detected"),
        ]

@pytest.mark.asyncio
@pytest.mark.parametrize("command,server_return", test_command)
async def test_errors_user_nick(clean_log, command, server_return):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)
    writer.write(command.encode())
    retval = await get_read_content(reader)
    writer.close()
    await writer.wait_closed()
    assert len(retval), "nothing return from server"
    if len(server_return) and type(retval) is not list:
        assert len(retval)
        assert retval.strip() == server_return
    else:
        assert len(retval) != 2

@pytest.mark.asyncio
async def test_errors_nick_not_authenticated(clean_log):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    command = f"NICK test{SEP}"
    writer.write(command.encode())
    retval = await get_read_content(reader)
    writer.close()
    await writer.wait_closed()
    if  type(retval) is not list:
        assert len(retval);
        assert retval.strip() == f"{PREFIX} 451 * test :You have not registered"
    else:
        assert len(retval) != 2

@pytest.mark.asyncio
async def test_errors_user_not_authenticated(clean_log):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    command = f"USER test 0 * :Cobby Brian{SEP}"
    writer.write(command.encode())
    retval = await get_read_content(reader)
    writer.close()
    await writer.wait_closed()
    if  type(retval) is not list:
        assert len(retval)
        assert retval.strip() == f"{PREFIX} 451 * test 0 * Cobby Brian :You have not registered"
    else:
        assert len(retval) != 2

test_pass = [
        ("PASS" + SEP, f"{PREFIX} 461 * :Not enough parameters"),
        ("PASS fsdfsda" + SEP, f"{PREFIX} 464 * fsdfsda :Password incorrect"),
        ]

@pytest.mark.asyncio
@pytest.mark.parametrize("command, expected", test_pass)
async def test_errors_pass(clean_log, command, expected):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    writer.write(command.encode())
    time.sleep(0.2)
    retval = await get_read_content(reader)
    writer.close()
    await writer.wait_closed()
    if  type(retval) is not list:
        print(retval)
        assert len(retval)
        assert expected == retval.strip()
    else:
        assert len(retval) != 2
        assert len(retval[0])

@pytest.mark.asyncio
async def test_errors_nick_unicity(clean_log):
    _, writer_to_close = await log_user_test("test", "truc", "Truch muche")
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)
    command = f"NICK test{SEP}"
    writer.write(command.encode())
    retval = await get_read_content(reader)
    writer.close()
    await writer.wait_closed()
    writer_to_close.close()
    await writer_to_close.wait_closed()
    if  type(retval) is not list:
        assert len(retval);
        assert retval.strip() == f"{PREFIX} 433 * test :Nickname is already in use"
    else:
        assert len(retval) != 2

@pytest.mark.asyncio
async def test_errors_nick_unicity(clean_log):
    reader, writer_to_close = await log_user_test("test", "truc", "Truch muche")
    command = f"USER test * 0 :Yoo Yoo{SEP}"
    writer_to_close.write(command.encode())
    await writer_to_close.drain()
    retval = await get_read_content(reader)
    writer_to_close.close()
    await writer_to_close.wait_closed()
    assert len(retval[1]);
    assert retval[1] == f"{PREFIX} 462 test test * 0 Yoo Yoo :You may not reregister"
