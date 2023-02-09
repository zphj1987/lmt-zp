# 监控工具lmt
软件分为ldiskfs版本和zfs版本

## ldiskfs版本软件
```bash
[root@test201 lmt]# ls release/ldiskfs/
cerebro-1.21-8.x86_64.rpm                        lmt-3.2.11-1.el7.x86_64.rpm         lmt-server-agent-3.2.11-1.el7.x86_64.rpm
cerebro-clusterlist-hostsfile-1.21-8.x86_64.rpm  lmt-server-3.2.11-1.el7.x86_64.rpm
```
## zfs版本软件
```bash
[root@test201 lmt]# ls release/zfs/
cerebro-1.21-8.x86_64.rpm                        lmt-3.2.11-1.el7.x86_64.rpm         lmt-server-agent-3.2.11-1.el7.x86_64.rpm
cerebro-clusterlist-hostsfile-1.21-8.x86_64.rpm  lmt-server-3.2.11-1.el7.x86_64.rpm
```

直接安装即可

## 配置方法
### 1、写配置文件
```bash
[root@test201 ~]# cat /etc/hostsfile
test201
```
###2、启动进程
```bash
[root@test201 ~]# systemctl start cerebrod
[root@test201 ~]# systemctl enable cerebrod
Created symlink from /etc/systemd/system/multi-user.target.wants/cerebrod.service to /usr/lib/systemd/system/cerebrod.service.
```
注意启动的时候使用上面的命令启动，停止的时候需要执行
```bash
killall cerebrod
```
