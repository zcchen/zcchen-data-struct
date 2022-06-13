BUILDDIR_PREFIX     = build
INSTALLDIR_PREFIX   = install

CMAKE_ARGS    =
#CMAKE_ARGS   += -DCMAKE_VERBOSE_MAKEFILE:BOOL=ON
CMAKE_ARGS   += 

.PHONY: all clean all/% clean/% linux win32 win64

all: all/linux all/win32 all/win64
clean: clean/linux clean/win32 clean/win64

linux all/linux: $(BUILDDIR_PREFIX)-linux
	cmake $(CMAKE_ARGS) -B $< -DCMAKE_INSTALL_PREFIX=$(INSTALLDIR_PREFIX)-linux ./
	cmake --build $<
	cmake --build $< -t install

win32 all/win32: $(BUILDDIR_PREFIX)-win32
	cmake $(CMAKE_ARGS) -B $< \
		-DCMAKE_INSTALL_PREFIX=$(INSTALLDIR_PREFIX)-win32 \
		-DCMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES=/usr/lib/gcc/i686-w64-mingw32/10-win32/include/ \
		-DCMAKE_C_COMPILER=i686-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=i686-w64-mingw32-g++ -DCMAKE_RC_COMPILER=i686-w64-mingw32-windres -DCMAKE_RANLIB=i686-w64-mingw32-dlltool \
		./
	cmake --build $<
	cmake --build $< -t install

win64 all/win64: $(BUILDDIR_PREFIX)-win64
	cmake $(CMAKE_ARGS) -B $< \
		-DCMAKE_INSTALL_PREFIX=$(INSTALLDIR_PREFIX)-win64 \
		-DCMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES=/usr/lib/gcc/x86_64-w64-mingw32/10-win32/include/ \
		-DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ -DCMAKE_RC_COMPILER=x86_64-w64-mingw32-windres -DCMAKE_RANLIB=x86_64-w64-mingw32-dlltool \
		./
	cmake --build $<
	cmake --build $< -t install

clean/linux:
	-rm -rf $(INSTALLDIR_PREFIX)-linux
	-rm -rf $(BUILDDIR_PREFIX)-linux

clean/win32:
	-rm -rf $(INSTALLDIR_PREFIX)-win32
	-rm -rf $(BUILDDIR_PREFIX)-win32

clean/win64:
	-rm -rf $(INSTALLDIR_PREFIX)-win64
	-rm -rf $(BUILDDIR_PREFIX)-win64


$(BUILDDIR_PREFIX)-linux $(BUILDDIR_PREFIX)-win32 $(BUILDDIR_PREFIX)-win64:
	mkdir -p $@

