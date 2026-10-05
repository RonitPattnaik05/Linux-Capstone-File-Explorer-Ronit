obj-m += capstone_driver.o

KERNEL_DIR := $(HOME)/WSL2-Linux-Kernel
PWD := $(shell pwd)

all:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean
