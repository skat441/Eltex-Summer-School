obj-m += netlink_module.o

all: module user

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
	rm user
user: user.c
	gcc user.c -o user
module: netlink_module.c
	make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
