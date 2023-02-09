rm -rf lmt-3.2.11.tar.gz;
make dist;
unalias cp;
cp -ra lmt-3.2.11.tar.gz /root/rpmbuild/SOURCES/;
rm -rf /root/rpmbuild/RPMS/x86_64/lmt*.rpm
rpmbuild -bb lmt-nomysql.spec

rpm -ivh /root/rpmbuild/RPMS/x86_64/lmt*.rpm --force
killall cerebrod
systemctl restart cerebrod
