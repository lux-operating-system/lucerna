PLATFORM=x86_64-lux
ASFLAGS=-f elf64 -i./src/platform/$(PLATFORM)
CCFLAGS=-c -I./src/include -ffreestanding -O3
AS=nasm
CC=x86_64-lux-gcc
AR=x86_64-lux-ar
SRCC:=$(shell find ./src -type f -name "*.c")
OBJC:=$(SRCC:.c=.o)
SRCA:=$(shell find ./src -type f -name "*.asm")
OBJA:=$(SRCA:.asm=.o)
ROOT:=$(shell whereis -bq x86_64-lux-gcc | sed 's/\/bin\/x86_64-lux-gcc//g')

all: libc.a

%.o: %.c
	@echo "\x1B[0;1;32m [  CC   ]\x1B[0m $<"
	@$(CC) $(CCFLAGS) -o $@ $<

%.o: %.asm
	@echo "\x1B[0;1;32m [  AS   ]\x1B[0m $<"
	@$(AS) $(ASFLAGS) -o $@ $<

libc.a: $(OBJA) $(OBJC)
	@cp ./src/platform/$(PLATFORM)/stubs/crt0.o .
	@echo "\x1B[0;1;34m [  AR   ]\x1B[0m libc.a"
	@OBJA_FILTERED=$(shell find ./src -type f -name "*.asm" ! -name "*crt*")
	@OBJA_FILTERED=$(OBJA_FILTERED:.asm=.o)
	@$(AR) rvs libc.a $(OBJA_FILTERED) $(OBJC)

install: libc.a
	@echo "lucerna: install binaries in $(ROOT)/lib"
	@cp libc.a $(ROOT)/lib/
	@cp crt0.o $(ROOT)/lib/
	@echo "lucerna: install binaries in $(ROOT)/x86_64-lux/lib"
	@cp libc.a $(ROOT)/lib/
	@cp crt0.o $(ROOT)/lib/
	@echo "lucerna: install binaries in $(ROOT)/lib/gcc/x86_64-lux/14.2.0/"
	@cp libc.a $(ROOT)/lib/gcc/x86_64-lux/14.2.0/
	@cp crt0.o $(ROOT)/lib/gcc/x86_64-lux/14.2.0/
	@echo "lucerna: install headers in $(ROOT)/lib/gcc/x86_64-lux/14.2.0/include/"

clean:
	@rm -f libc.a crt0.o $(OBJA) $(OBJC)

