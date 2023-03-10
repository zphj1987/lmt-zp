Name: lmt
Version: 3.2.11
Release: 1%{?dist}
License: GPL
Group: Applications/System
Summary: Lustre Montitoring Tool
URL: http://github.com/chaos/lmt
Packager: Ned Bass <bass6@llnl.gov>
Source: %{name}-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root

# MySQL was replaced with MariaDB in RHEL 8
#%if 0%{?rhel} >= 8
#BuildRequires: mariadb, mariadb-devel
#%else
#BuildRequires: mysql, mysql-devel
#%endif

BuildRequires: cerebro >= 1.3-5
BuildRequires: ncurses-devel
BuildRequires: lua-devel >= 5.1
#%define __spec_install_post /usr/lib/rpm/brp-compress || :
%define debug_package %{nil}

%description
Lustre Monitoring Tool

%package server
Summary: Lustre Monitoring Tools Server
Group: Applications/System
Requires: cerebro >= 1.3

# MySQL was replaced with MariaDB in RHEL 8
#%if 0%{?rhel} >= 8
#Requires: mariadb-server >= 4.1.20
#%else
#Requires: mysql-compat-server >= 4.1.20
#%endif

Requires: perl-DBI
%description server
Lustre Monitoring Tools (LMT) Server

%package server-agent
Summary: Lustre Monitoring Tools Server Agent
Group: Applications/System
Requires: cerebro >= 1.3
%description server-agent
Lustre Monitoring Tools (LMT) Server Agent

%prep
%setup

%build
%configure --disable-mysql
make

#%check
#SKIP_INTERACTIVE=true make -C test check

%install
rm -rf   $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files

%files server
%defattr(-,root,root)
#%{_libdir}/cerebro/cerebro_monitor_lmt*
%{_sbindir}/lmtsh
#%{_sbindir}/lmtinit
%{_bindir}/ltop
%{_mandir}/man1/*
%{_mandir}/man5/*
%{_mandir}/man8/*
%{_datadir}/lmt/*
%dir %{_sysconfdir}/lmt
%config(noreplace) %{_sysconfdir}/lmt/lmt.conf

%files server-agent
%{_libdir}/cerebro/cerebro_metric_lmt*
%{_sbindir}/lmtmetric
