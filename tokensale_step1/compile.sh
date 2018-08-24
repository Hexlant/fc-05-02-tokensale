#!/bin/sh
echo "eosiocpp -o tokensale_step1.wast tokensale_step1.cpp"
eosiocpp -o tokensale_step1.wast tokensale_step1.cpp

echo "eosiocpp -g tokensale_step1.abi tokensale_step1.cpp"
eosiocpp -g tokensale_step1.abi tokensale_step1.cpp



