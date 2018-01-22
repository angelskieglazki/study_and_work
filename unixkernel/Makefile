obj-m += ex_symbdriver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

load: all
	sudo insmod ex_symbdriver.ko
	sudo dmesg -c

unload:
	sudo rmmod ex_symbdriver.ko
	sudo dmesg -c