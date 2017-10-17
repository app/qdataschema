NAME   =	libqdataschema
NAMEV  =	$(NAME)-1.1.0
MAKE = make
QMAKE = qmake
MAKEFILE = Makefile
LIBDIR = $(INSTALL_ROOT)/usr/lib
BINDIR = $(INSTALL_ROOT)/usr/bin
DOCDIR = $(INSTALL_ROOT)/usr/share/doc/$(NAMEV)
INCLUDEDIR = $(INSTALL_ROOT)/usr/include/qdataschema

#BINDIR =	/usr/bin
#MANDIR =	/usr/man/man8
#VARDIR =	/usr/share/$(NAME)
TAR    =  	$(HOME)/tmp/$(NAMEV)
TARGET =	i586

first: all 

all: 
	cd src && $(QMAKE) && $(MAKE)
clean:
	cd src && $(QMAKE) && $(MAKE) distclean
	cd doc && $(MAKE) clean
#	rm -fr lib
	rm -fr rpm
#	rm -fr bin

doc: dummy
	cd doc && make
	
install: all
	find src doc -name Makefile -delete
	cd src && $(QMAKE) && $(MAKE) install LIBDIR=$(LIBDIR) BINDIR=$(BINDIR) INCLUDEDIR=$(INCLUDEDIR) && $(MAKE) install DOCDIR=$(DOCDIR)
	
uninstall:
	cd src && $(MAKE) uninstall

tgz:	dummy
	mkdir -p $(TAR)
	cp -r -f * $(TAR)
	cd $(TAR) 
	cd $(TAR) && find src -name Makefile -delete 
#	cd $(TAR) && find . -name CVS -exec rm -fr {} \;
	cd $(HOME)/tmp && tar cf - "$(NAMEV)/" | bzip2 -f9 > "$(HOME)/RPM/SOURCES/$(NAMEV).tar.bz2"	
	rm -f -r $(TAR)

srpm:	tgz
	mkdir -p rpm
	rpmbuild --target=$(TARGET) -bs $(NAME).spec
	mv -f $(HOME)/RPM/SRPMS/$(NAME)* rpm

rpm: srpm
	rpmbuild --target=$(TARGET) -bb $(NAME).spec
	mv -f $(HOME)/RPM/RPMS/$(TARGET)/$(NAME)* rpm

dummy:
