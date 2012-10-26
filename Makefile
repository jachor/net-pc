#
TARGET     = net-pc
# List all the source files here
# eg if you have a source file foo.c then list it here
SOURCES = main.c
SOURCES += ip.c ip_hw_pc.c ip_utils.c ip_arp.c
SOURCES += ip_crc.c ip_icmp.c ip_tcp.c
# Include are located in the Include directory
INCLUDES = -IInclude
#######################################################################################
CFLAGS   = -g -O0 -Wall -Wunused $(INCLUDES)
ASFLAGS  = -x assembler-with-cpp -Wa,-gstabs
LDFLAGS  = 
########################################################################################
CC       = gcc
CP       = cp -p
RM       = rm -f
MV       = mv
########################################################################################
# the file which will include dependencies
DEPEND = $(SOURCES:.c=.d)
# all the object files
OBJECTS = $(SOURCES:.c=.o)
all: $(TARGET)
$(TARGET): $(OBJECTS)
	echo "Linking $@"
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@
	echo
%.o: %.c
	echo "Compiling $<"
	$(CC) -c $(CFLAGS) -o $@ $<
# rule for making assembler source listing, to see the code
%.lst: %.c
	$(CC) -c $(ASFLAGS) -Wa,-anlhd $< > $@
# include the dependencies unless we're going to clean, then forget about them.
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPEND)
endif
# dependencies file
# includes also considered, since some of these are our own
# (otherwise use -MM instead of -M)
%.d: %.c
	echo "Generating dependencies $@ from $<"
	$(CC) -M ${CFLAGS} $< >$@
.SILENT:
.PHONY:	clean all
clean:
	-$(RM) $(OBJECTS)
	-$(RM) $(TARGET).txt $(TARGET).hex $(TARGET).elf
	-$(RM) $(SOURCES:.c=.lst)
	-$(RM) $(DEPEND)
