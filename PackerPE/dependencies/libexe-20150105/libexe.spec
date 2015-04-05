Name: libexe
Version: 20150105
Release: 1
Summary: Library to access the executable (EXE) format
Group: System Environment/Libraries
License: LGPL
Source: %{name}-%{version}.tar.gz
URL: https://github.com/libyal/libexe/
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)
              
              

%description
libexe is a library to access the executable (EXE) format

%package devel
Summary: Header files and libraries for developing applications for libexe
Group: Development/Libraries
Requires: libexe = %{version}-%{release}

%description devel
Header files and libraries for developing applications for libexe.

%package tools
Summary: Several tools for reading executable (EXE) files
Group: Applications/System
Requires: libexe = %{version}-%{release} 
 

%description tools
Several tools for reading executable (EXE) files

%package python
Summary: Python bindings for libexe
Group: System Environment/Libraries
Requires: libexe = %{version}-%{release} python
BuildRequires: python-devel

%description python
Python bindings for libexe

%prep
%setup -q

%build
%configure --prefix=/usr --libdir=%{_libdir} --mandir=%{_mandir} --enable-python
make %{?_smp_mflags}

%install
rm -rf ${RPM_BUILD_ROOT}
make DESTDIR=${RPM_BUILD_ROOT} install

%clean
rm -rf ${RPM_BUILD_ROOT}

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%attr(755,root,root) %{_libdir}/*.so.*

%files devel
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README ChangeLog
%{_libdir}/*.a
%{_libdir}/*.la
%{_libdir}/*.so
%{_libdir}/pkgconfig/libexe.pc
%{_includedir}/*
%{_mandir}/man3/*

%files tools
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%attr(755,root,root) %{_bindir}/exeinfo
%{_mandir}/man1/*

%files python
%defattr(644,root,root,755)
%doc AUTHORS COPYING NEWS README
%{_libdir}/python*/site-packages/*.a
%{_libdir}/python*/site-packages/*.la
%{_libdir}/python*/site-packages/*.so

%changelog
* Mon Jan  5 2015 Joachim Metz <joachim.metz@gmail.com> 20150105-1
- Auto-generated

