#!/bin/bash
./ircserv 6667 test &> ft_irc.log &
cd test
pytest
pgrep ircserv | xargs kill -9
