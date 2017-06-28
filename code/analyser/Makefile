obj-m += analyser.o
analyser-objs := narray.o splay.o analyser_main.o
#KDIR =/lib/modules/3.13.0-68-generic/build
KDIR = /usr/src/linux-4.4.6/
#KBUILD_EXTRA_SYMBOLS := /usr/src/linux-4.4.6/Module.symvers
#IDIR = /usr/src/linux-4.4.6/drivers/md
all:
	#$(MAKE) -C $(KDIR) M=$(PWD) modules
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	rm -rf *.o *.ko *.mod *.symvers *.order
