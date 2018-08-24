#!/bin/sh
echo "eosiocpp -o tokensale_step2.wast tokensale_step2.cpp"
eosiocpp -o tokensale_step2.wast tokensale_step2.cpp

echo "eosiocpp -g tokensale_step2.abi tokensale_step2.cpp"
eosiocpp -g tokensale_step2.abi tokensale_step2.cpp



