#!/bin/bash
./ircserv 6933 test &> ft_irc.log &
cd test
pytest
pgrep ircserv | xargs kill -9
