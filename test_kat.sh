#!/bin/bash

# 1. Make shared library object
make lib

# 2. Run test cases
pushd wrapper/python
cp ../../kats/kyber{512,768,1024}.kat .
python3 -m pytest -v
rm kyber{512,768,1024}.kat
popd
