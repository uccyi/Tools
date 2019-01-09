#
# %W% %G%
# makefile
#
SRC10 =	config.c	\
		tool.c

DLL10 = ../lib/libtools.so

# Compiler flags. 

CFLAGS	= -DOW_I18N -DLINUX
#CFLAGS	= -DOW_I18N -DLINUX -D_LARGEFILE_SOURCE -D_FILE_OFFSET_BITS=64
CPPFLAGS += -I/usr/include -I../include
LDLIBS2 += -lXm -lXt -lX11
#CC = cc -O3
CC = cc -O3 -fPIC
#CC = gcc

OBJ10 = $(SRC10:.c=.o)

#
## all	:DLL、全コマンドの作成
## dll	:DLLのみ作成
## clean:DLL、全コマンド、coreファイルの削除

all : 
	make $(DLL10) ;

clean :
	$(RM)		\
	$(DLL10)	\
	$(OBJ10)	\
	core
#
## libtools.so	: DLL作成 
$(DLL10) : $(SRC10) $(OBJ10)
	$(LINK.c) -shared $(OBJ10) -o $(DLL10);
