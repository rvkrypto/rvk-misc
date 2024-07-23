#	Makefile
#	2024-07-17	Markku-Juhani O. Saarinen <mjos@iki.fi>.  See LICENSE.

XBIN	?=	xtest
CSRC	=	$(wildcard *.c	*/*.c)
SSRC	=	$(wildcard *.S	*/*.S)
OBJS	= 	$(CSRC:.c=.o) $(SSRC:.S=.o)

XCHAIN	=	riscv64-unknown-linux-gnu-
CC 		=	$(XCHAIN)clang
SPIKE	=	spike

RVISA	=	rv64gcv_zkn_zks_zbb_zvbb_zvbc_zvkg_zvkned_zvkn_zvks_zvl256b
CFLAGS	=	-march=$(RVISA) -menable-experimental-extensions
SPIKEFL	=	--isa=$(RVISA)_zicntr_zihpm #_zvkk
CFLAGS	+=	-Wall -Wextra -Wshadow -Wno-unused-parameter 
CFLAGS	+=	-DRVK_ALGTEST_VERBOSE_SIO
#CFLAGS	+=	-Rpass=loop-vectorize -Rpass-missed=loop-vectorize \
#			-Rpass-analysis=loop-vectorize -fsave-optimization-record 
CFLAGS	+=	-O3 -g
CFLAGS	+=	-I.
LDFLAGS	+=	-static
LDLIBS	+=

#	xtest
$(XBIN):	$(OBJS)
	$(CC) $(LDFLAGS) $(CFLAGS) -o $(XBIN) $(OBJS) $(LDLIBS)

run:	$(XBIN)
	$(SPIKE)	$(SPIKEFL) pk $(XBIN)

$(XBIN).dis:	$(XBIN)
	$(XCHAIN)objdump -C -g -l -S -d $^ > $@

%.dis:	%.o
	$(XCHAIN)objdump -S --visualize-jumps -C -l -x $^ | expand > $@

%.o:	%.[csS]
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	$(RM) -rf $(XBIN) $(XBIN).* $(PROFDIR) $(OBJS) \
	*.opt.yaml *.dis *.rsp *.req

