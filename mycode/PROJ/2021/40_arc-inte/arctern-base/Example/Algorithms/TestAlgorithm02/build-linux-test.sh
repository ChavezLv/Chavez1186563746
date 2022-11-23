#!/bin/bash
rm -r build/*
cd build
cmake \
      -DDEPLOY_TARGET=linux-x86_64 \
      -DCMAKE_BUILD_TYPE=Debug \
      -DLOCALBUILD=true \
      -DMXNET_ALLOWED=true \
      ..
make -j2

