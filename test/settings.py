import asyncio
import time

SEP = "\015\n"
HOST = "127.0.0.1"
PORT = 6667
PREFIX = ":ft_irc"
PASSWORD = "test"

async def get_read_content(reader):
    try:
        async with asyncio.timeout(0.1):
            retval = await reader.readline()
    except TimeoutError:
        return ""
    try:
        async with asyncio.timeout(0.1):
            retval2 = await reader.readline()
    except TimeoutError:
        return retval.decode().strip()
    return [retval.decode().strip(), retval2.decode().strip()]

async def authenticate(writer):
    command = f"PASS {PASSWORD}" + SEP
    writer.write(command.encode())
    await writer.drain()

async def register(writer, user, realname, nick):
    command = f"NICK {nick}" + SEP
    writer.write(command.encode())
    await writer.drain()

    command = f"USER {user} 0 * :{realname}" + SEP
    writer.write(command.encode())
    await writer.drain()

async def log_user_test(nick, user, realname):
    reader, writer = await asyncio.open_connection(HOST, PORT)
    await authenticate(writer)
    await register(writer, user, realname, nick)
    return reader, writer
