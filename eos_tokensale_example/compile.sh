#!/bin/sh
echo "eosiocpp -o tokensale_step3.wast tokensale_step3.cpp"
eosiocpp -o eos_tokensale_example.wast eos_tokensale_example.cpp

echo "eosiocpp -g tokensale_step3.abi tokensale_step3.cpp"
eosiocpp -g eos_tokensale_example.abi eos_tokensale_example.cpp



