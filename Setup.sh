#!/bin/bash

#Application path location of applicaiton

ToolDAQapp=../ToolDAQFramework

export LD_LIBRARY_PATH=./lib:${ToolDAQapp}/ToolDAQ/zeromq-4.0.7/lib:${ToolDAQapp}/ToolDAQ/boost_1_60_0/install/lib:${ToolDAQapp}/ToolDAQ/root/lib:$LD_LIBRARY_PATH
