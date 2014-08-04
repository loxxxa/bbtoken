obj-m := wdtoken.o
KVERSION := $(shell uname -r)
KDIR := /lib/modules/$(KVERSION)/build
PWD := $(shell pwd)
 
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.o *.mod.* *.ko
