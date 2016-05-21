all:	de1.vcd viewvectors

rt_functions.o: rtclock/rt_functions.c
	gcc -c rtclock/rt_functions.c

rt_clk.o: rtclock/rt_clk.vhd
	ghdl -a rtclock/rt_clk.vhd

de1simulator_c.o: de1simulator.c sharedmem.h
	gcc -c de1simulator.c -o de1simulator_c.o

de1simulator.o: de1simulator.vhd
	ghdl -a de1simulator.vhd

vga.o: vga.vhd
	ghdl -a vga.vhd

testbench.o:	testbench.vhd rt_clk.o rt_functions.o de1simulator_c.o \
				de1simulator.o vga.o
	ghdl -a testbench.vhd

hexdriver.o:  hexdriver.vhd
	ghdl -a hexdriver.vhd

de1testbench:	de1simulator.o de1simulator_c.o testbench.o \
				rt_functions.o rt_clk.o hexdriver.o vga.o
	ghdl -e -Wl,de1simulator_c.o -Wl,rt_functions.o de1testbench

de1.vcd:	de1testbench
	ghdl -r de1testbench --vcd=de1.vcd

viewvectors: de1.vcd
	gtkwave de1.vcd

clean:
	rm *.o work-obj93.cf 
