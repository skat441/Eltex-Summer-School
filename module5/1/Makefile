CC=x86_64-linux-gnu-gcc-13

obj-m += hello_module.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules

insert:
	sudo insmod hello_module.ko

remove:
	sudo rmmod hello_module.ko

show:
	dmesg | tail -1

see_info:
	modinfo -a hello_module.ko
	modinfo -d hello_module.ko

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean