# xilinx-ref-vc707
I exclusively created this repository with working code to use as a reference for whenever I (and you) to prototype on the Xilinx Virtex-7 FPGA VC707 Evaluation Kit.

###ip_repo

This folder is repository of IP Core designed for Vivado HLS. In each core, there are three files:

- *_HLS.cpp (main file for Vivado HLS)
- *_HLS_test.cpp (file for testbench used for Vivado HLS)
- *_SDK.c (file for software SDK)

###vivado-201x.x

This folder contains TCL script and can be used to 'source' the *.tcl file in Vivado 201x.x.
