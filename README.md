# xilinx-ref-vc707
I exclusively created this repository with working code to use as a reference for whenever I (and you) to prototype on the Xilinx Virtex-7 FPGA VC707 Evaluation Kit.

###ip_repo

This folder is repository of IP Core designed for Vivado HLS. In each core, there are three files:

- *_HLS.cpp (main file for Vivado HLS)
- *_HLS_test.cpp (file for testbench used for Vivado HLS)
- *_SDK.c (file for software SDK)

####Getting started using ip_repo

1) Add a new solution

![alt text][img1]
[img1]: https://3.bp.blogspot.com/-DQUL19DgrCo/V770t7HHxTI/AAAAAAAAE90/7QEdaP00zNwkw27z0yUptEUKk7g0wysowCLcB/s1600/vivado-hls-new-solution.png "Add New Solution"

2) Select board

![alt text][img2]
[img2]: https://4.bp.blogspot.com/-SJlwJ74oLSo/V770tq4RovI/AAAAAAAAE9w/BkYJ1TDW3-sIdEKRwOsW7qwkI6iPBIHqwCLcB/s1600/vivado-hls-solution-board.png "Select board"

3) The solution is ready to be synthesized

![alt text][img3]
[img3]: https://1.bp.blogspot.com/-2tp7MCXPITg/V770tm34aQI/AAAAAAAAE94/z6MdwMiq6hM14vHzZxlnzw5C1k2LTA4JwCLcB/s1600/vivado-hls-solution-ok.png "Solution Ready"

###vivado-201x.x

This folder contains TCL script and can be used to 'source' the *.tcl file in Vivado 201x.x.
