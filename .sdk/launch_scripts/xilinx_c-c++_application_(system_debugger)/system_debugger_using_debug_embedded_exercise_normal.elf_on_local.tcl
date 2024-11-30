connect -url tcp:127.0.0.1:3121
source C:/Users/MattiTuomela/software/Josuoikea/Sulari/Exercise/ps7_init.tcl
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Xilinx PYNQ-Z1 003017A6E375A"} -index 0
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Xilinx PYNQ-Z1 003017A6E375A" && level==0} -index 1
fpga -file C:/Users/MattiTuomela/software/Josuoikea/Sulari/Exercise/top.bit
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Xilinx PYNQ-Z1 003017A6E375A"} -index 0
loadhw -hw C:/Users/MattiTuomela/software/Josuoikea/Sulari/Exercise/system.hdf -mem-ranges [list {0x40000000 0xbfffffff}]
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*" && jtag_cable_name =~ "Xilinx PYNQ-Z1 003017A6E375A"} -index 0
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Xilinx PYNQ-Z1 003017A6E375A"} -index 0
dow C:/Users/MattiTuomela/software/Josuoikea/Sulari/Embedded_Exercise_normal/Debug/Embedded_Exercise_normal.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "ARM*#0" && jtag_cable_name =~ "Xilinx PYNQ-Z1 003017A6E375A"} -index 0
con
