# trade equipment project
# Spec file for building rpm and rpms under ALT
# $Id: libqt4-qdataschema.spec,v 1.9 2008/11/09 20:03:16 leader Exp $
# $Name:  $
#

%define ShortName libqt4-qdataschema
%define _qtdir %(echo $QTDIR)

Name: libqt4-qdataschema
Version: 1.2.0
Release: alt0

Summary: QT Data schema library

License: GPL
Group: Office
Url: http://www.leaderit.ru/
Vendor:	Leader InfoTech
Source: %name-%version.tar.bz2

Requires: qt4 >= 4.2


BuildRequires:       gcc-c++ libstdc++-devel
BuildRequires:       libqt4-devel doxygen tetex-latex tetex-dvips

%description
QT Data Schema library
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n libqt4-qdataschema-devel
Summary: QT Data Schema library

Group: Office
Requires: libqt4-qdataschema

%description -n libqt4-qdataschema-devel
QT Data Schema library headers
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>


#==============================================================
%package -n libqt4-qdataschema-utils
Summary: QT Data Schema utils

Group: Office
Requires: libqt4-qdataschema

%description -n libqt4-qdataschema-utils
QT Data Schema utils
Provide Application to database server 
structure verification and syncronysation.
Support MySQL, PostgreSQL, SQLite.

Developers
---------------
Valery Grazhdankin <vg at leaderit dot ru>

#==============================================================
%package -n libqt4-qdataschema-doc
Summary: QT Data Schema library documentation

Group: Office

%description -n libqt4-qdataschema-doc
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
export QTDIR=/usr/lib/qt4
export PATH=${PATH}:$QTDIR/bin
%make -e QT_VER=4

%install
export QTDIR=/usr/lib/qt4
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
cd %buildroot%_libdir/ && ln -s qt4/lib/* ./

%post

%postun

%clean

%files
%_libdir/*

%files -n libqt4-qdataschema-utils
%_libdir/qt4/bin/*

%files -n libqt4-qdataschema-doc
%_docdir/%name-%version/*

%files -n libqt4-qdataschema-devel
%_libdir/qt4/include/%ShortName/*.h

%changelog
* Sun Nov 09 2008 Valeriy Grazhdankin <leader@altlinux.ru> 1.2.0-alt0
  + Correct updates for SQLite, MySQL

* Sat Mar 19 2005 Valery Grazdankin <leader@altlinux.ru> 1.0.0-alt0
  + init
