#!/bin/sh
echo "eosiocpp -o tokensale_step3.wast tokensale_step3.cpp"
eosiocpp -o tokensale_step3.wast tokensale_step3.cpp

echo "eosiocpp -g tokensale_step3.abi tokensale_step3.cpp"
eosiocpp -g tokensale_step3.abi tokensale_step3.cpp



