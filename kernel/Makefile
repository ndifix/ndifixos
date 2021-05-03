TARGET = kernel.elf
OBJS = main.o ascii.o newlib_support.o console.o pci.o asmfunc.o \
usb/xhci/transfer_request_block.o

CFLAGS   += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone
CXXFLAGS += -O2 -Wall -g --target=x86_64-elf -ffreestanding -mno-red-zone \
	    -fno-exceptions -fno-rtti -std=c++17
LDFLAGS += --entry KernelMain -z norelro --image-base 0x110000 --static
DEPENDS = $(join $(dir $(OBJS)), $(addprefix .,$(notdir $(OBJS:.o=.d))))

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS) -lc

ascii.bin: ascii.txt
	../tool/makefont.py -o $@ ascii.txt

ascii.o: ascii.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 ascii.bin $@

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

%.o: %.cpp Makefile
	clang++ $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $<

.%.d: %.cpp
	clang++ $(CPPFLAGS) $(CXXFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.cpp=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

.%.d: %.c
	clang $(CPPFLAGS) $(CFLAGS) -MM $< > $@
	$(eval OBJ = $(<:.c=.o))
	sed --in-place 's|$(notdir $(OBJ))|$(OBJ)|' $@

.%.d: %.bin
	touch $@

.PHONY: depends
depends:
	$(MAKE) $(DEPENDS)

-include $(DEPENDS)
