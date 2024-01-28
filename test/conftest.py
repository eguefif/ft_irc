import pytest

@pytest.fixture
def clean_log():
    with open("../ft_irc.log", "w") as file:
        file.write("")
    yield
    with open("../ft_irc.log", "w") as file:
        file.write("")

