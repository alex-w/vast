# adding '-std=c89 -Wno-error' to avoid compile problem with gcc-15
CFLAGS= -g -D_FILE_OFFSET_BITS=64 -std=gnu99 -Wno-error
#CFLAGS= -g -D_FILE_OFFSET_BITS=64
#CC= cc
CC := $(shell ./find_gcc_compiler.sh)
LIBWCS = libwcs/libwcs.a
LIBS = $(LIBWCS) -lm
#LIBS = $(LIBWCS) -lm -lnsl -lsocket
BIN = bin
.PRECIOUS: ${LIBWCS} ${LIBNED}
.c.o:
	$(CC) -c $(CFLAGS) $(DEFS) $<

all:	cphead delwcs delhead edhead fixpix gethead i2f imcat imhead immatch \
	imrot imsize imstar imwcs scat sethead addpix getpix setpix sky2xy \
	keyhead skycoor subpix xy2sky wcshead conpix gettab newfits getfits \
	imstack imextract sumpix remap getcol getdate imfill imsmooth imresize \
	fileroot filename filext char2sp sp2char crlf htmlsp isnum isrange isfits \
	isfile simpos bincat

addpix: addpix.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/addpix addpix.c $(LIBS)

bincat: bincat.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/bincat bincat.c $(LIBS)

char2sp: char2sp.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/char2sp char2sp.c $(LIBWCS)

conpix: conpix.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/conpix conpix.c $(LIBS)

cphead: cphead.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/cphead cphead.c $(LIBS)

crlf: crlf.c
	$(CC) $(CFLAGS) -o $(BIN)/crlf crlf.c

delwcs: delwcs.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/delwcs delwcs.c $(LIBS)

delhead: delhead.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/delhead delhead.c $(LIBS)

edhead: edhead.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/edhead edhead.c $(LIBS)

filename: filename.c
	$(CC) $(CFLAGS) -o $(BIN)/filename filename.c

filedir: filedir.c
	$(CC) $(CFLAGS) -o $(BIN)/filedir filedir.c

fileroot: fileroot.c
	$(CC) $(CFLAGS) -o $(BIN)/fileroot fileroot.c

filext: filext.c
	$(CC) $(CFLAGS) -o $(BIN)/filext filext.c

fixhead: fixhead.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/fixhead fixhead.c $(LIBS)

fixpix: fixpix.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/fixpix fixpix.c $(LIBS)

getcol: getcol.c $(LIBWCS) libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/getcol getcol.c $(LIBS)

getdate: getdate.c $(LIBWCS) libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/getdate getdate.c $(LIBS)

gethead: gethead.c $(LIBWCS) libwcs/wcs.h libwcs/fitsfile.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/gethead gethead.c $(LIBS)

getfits: getfits.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/getfits getfits.c $(LIBS)

getpix: getpix.c $(LIBWCS) libwcs/wcs.h libwcs/fitsfile.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/getpix getpix.c $(LIBS)

gettab: gettab.c $(LIBWCS) libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/gettab gettab.c $(LIBS)

httpget: httpget.c $(LIBWCS) libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/httpget httpget.c $(LIBS)

htmlsp: htmlsp.c
	$(CC) $(CFLAGS) -o $(BIN)/htmlsp htmlsp.c

i2f: i2f.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/i2f i2f.c $(LIBS)

imcat: imcat.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/imcat imcat.c $(LIBS)

imfill: imfill.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/imfill imfill.c $(LIBS)

imresize: imresize.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/imresize imresize.c $(LIBS)

imsmooth: imsmooth.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/imsmooth imsmooth.c $(LIBS)

imhead: imhead.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/imhead imhead.c $(LIBS)

imrot: imrot.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/imrot imrot.c $(LIBS)

imsize: imsize.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/imsize imsize.c $(LIBS)

imstack: imstack.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/imstack imstack.c $(LIBS)

imextract: imextract.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/imextract imextract.c $(LIBS)

imstar: imstar.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h libwcs/lwcs.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/imstar imstar.c $(LIBS)

isfile: isfile.c $(LIBWCS) libwcs/fitshead.h
	$(CC) $(CFLAGS) -o $(BIN)/isfile isfile.c $(LIBS)

isfits: isfits.c $(LIBWCS) libwcs/fitshead.h
	$(CC) $(CFLAGS) -o $(BIN)/isfits isfits.c $(LIBS)

isnum: isnum.c $(LIBWCS) libwcs/fitshead.h
	$(CC) $(CFLAGS) -o $(BIN)/isnum isnum.c $(LIBS)

isdate: isdate.c $(LIBWCS) libwcs/fitshead.h
	$(CC) $(CFLAGS) -o $(BIN)/isdate isdate.c $(LIBS)

isimlist: isimlist.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/isimlist isimlist.c $(LIBS)

isrange: isrange.c
	$(CC) $(CFLAGS) -o $(BIN)/isrange isrange.c

imwcs: imwcs.c $(LIBWCS) libwcs/fitsfile.h libwcs/lwcs.h
	$(CC) $(CFLAGS) -o $(BIN)/imwcs imwcs.c $(LIBS)

immatch: immatch.c $(LIBWCS) libwcs/fitsfile.h libwcs/lwcs.h
	$(CC) $(CFLAGS) -o $(BIN)/immatch immatch.c $(LIBS)

immwcs: immwcs.c $(LIBWCS) libwcs/fitsfile.h libwcs/lwcs.h
	$(CC) $(CFLAGS) -o $(BIN)/immwcs immwcs.c $(LIBS)

keyhead: keyhead.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/keyhead keyhead.c $(LIBS)

matchcat: matchcat.c $(LIBWCS) libwcs/wcscat.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/matchcat matchcat.c $(LIBS)

newfits: newfits.c $(LIBWCS) libwcs/fitshead.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/newfits newfits.c $(LIBS)

remap: remap.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/remap remap.c $(LIBS)

scat: scat.c $(LIBWCS) libwcs/wcscat.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/scat scat.c $(LIBS)

sethead: sethead.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcs.h
	$(CC) $(CFLAGS) -o $(BIN)/sethead sethead.c $(LIBS)

setpix: setpix.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/setpix setpix.c $(LIBS)

sky2xy: sky2xy.c $(LIBWCS) libwcs/wcs.h libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/sky2xy sky2xy.c $(LIBS)

skycoor: skycoor.c $(LIBWCS) libwcs/wcs.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/skycoor skycoor.c $(LIBS)

sp2char: sp2char.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/sp2char sp2char.c $(LIBWCS)

subpix: subpix.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/subpix subpix.c $(LIBS)

sumpix: sumpix.c $(LIBWCS) libwcs/fitsfile.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/sumpix sumpix.c $(LIBS)

wcshead: wcshead.c $(LIBWCS) libwcs/fitsfile.h
	$(CC) $(CFLAGS) -o $(BIN)/wcshead wcshead.c $(LIBS)

xy2sky: xy2sky.c $(LIBWCS) libwcs/wcs.h libwcs/wcscat.h
	$(CC) $(CFLAGS) -o $(BIN)/xy2sky xy2sky.c $(LIBS)

simpos: simpos.c libwcs/libwcs.a
	$(CC) $(CFLAGS) -o $(BIN)/simpos simpos.c $(LIBS)

gsc2cat: gsc2cat.c $(LIBWCS)
	$(CC) $(CFLAGS) -o $(BIN)/gsc2cat gsc2cat.c $(LIBS)

tmcat: tmcat.c
	$(CC) $(CFLAGS) -o $(BIN)/tmcat tmcat.c

tmcate: tmcate.c
	$(CC) $(CFLAGS) -o $(BIN)/tmcate tmcate.c

$(LIBWCS): libwcs/*.c libwcs/*.h
	cd libwcs; make

objclean:
	cd libwcs; make clean

binclean:
	rm -f ./bin/*

clean:
	make objclean; make binclean
