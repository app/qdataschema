# trade equipment project
# Spec file for building rpm and rpms under ALT
# $Id: libqt3-qdataschema.spec,v 1.25 2008/11/09 20:03:16 leader Exp $
# $Name:  $
#

%define ShortName qdataschema
%define _qtdir %(echo $QTDIR)

Name: libqt3-qdataschema
Version: 1.2.0
Release: alt0

Summary: QT Data schema library

License: GPL
Group: Office
Url: http://www.leaderit.ru/
Vendor:	Leader InfoTech
Source:		%name-%version.tar.bz2

Requires:	qt3 >= 3.2

BuildRequires:       gcc-c++ libstdc++-devel
BuildRequires:       libqt3-devel doxygen tetex-latex tetex-dvips

%description
QT Data Schema library
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n libqt3-qdataschema-devel
Summary: QT Data Schema library

Group: Office
Requires: libqt3-qdataschema

%description -n libqt3-qdataschema-devel
QT Data Schema library headers
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n libqt3-qdataschema-utils
Summary: QT Data Schema administrator

Group: Office
Requires: libqt3-qdataschema

%description -n libqt3-qdataschema-utils
QT Data Schema administrator
QT Data Schema library
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n libqt3-qdataschema-doc
Summary: QT Data Schema library documentation

Group: Office
#Requires:

%description -n libqt3-qdataschema-doc
QT Data Schema library documentation
QT Data Schema library
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

%prep
%setup

%build
export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
%make -e QT_VER=3

%install
export QTDIR=/usr/lib/qt3
export PATH=${PATH}:$QTDIR/bin
make doc && make install INSTALL_ROOT="$RPM_BUILD_ROOT" LIBDIR=%_libdir BINDIR=%_bindir DOCDIR=%_docdir/%name-%version INCLUDEDIR=%_includedir/%name
%__mkdir_p %buildroot/%_bindir
%__mkdir_p %buildroot/$QTDIR/lib
%__mkdir_p %buildroot/$QTDIR/bin
%__mkdir_p %buildroot/$QTDIR/include/%ShortName
%__mkdir_p %buildroot/%_docdir/%name-%version/html
%__mkdir_p %buildroot/%_docdir/%name-%version/html-ru
%__mkdir_p %buildroot/%_docdir/%name-%version/example

cp -df %_builddir/%name-%version/lib/* %buildroot/$QTDIR/lib/
cp -df %_builddir/%name-%version/bin/* %buildroot$QTDIR/bin/
cp -f %_builddir/%name-%version/src/lib/*.h %buildroot/$QTDIR/include/%ShortName
cp -f %_builddir/%name-%version/doc/html/* %buildroot%_docdir/%name-%version/html/
cp -f %_builddir/%name-%version/doc/html-ru/* %buildroot%_docdir/%name-%version/html-ru/
cp -Rdf %_builddir/%name-%version/src/example/* %buildroot%_docdir/%name-%version/example/
cd %buildroot%_libdir/ && ln -s qt3/lib/* ./

%post

%postun

%clean

%files
%_libdir/*

%files -n libqt3-qdataschema-utils
%_libdir/qt3/bin/*

%files -n libqt3-qdataschema-doc
%_docdir/%name-%version/*

%files -n libqt3-qdataschema-devel
%_libdir/qt3/include/%ShortName/*.h

%changelog
* Sun Nov 09 2008 Valeriy Grazhdankin <leader@altlinux.ru> 1.2.0-alt0
  + Correct updates for SQLite, MySQL

* Sat Mar 19 2005 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt0
  + init
