import pytest
import asyncio
import pytest_asyncio


HOST = "127.0.0.1"
PORT = 6933

@pytest.fixture
def clean_log():
    with open("../ft_irc.log", "w") as file:
        file.write("")
    yield
    with open("../ft_irc.log", "w") as file:
        file.write("")
