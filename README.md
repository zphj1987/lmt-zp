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

## change log

### 2023-02-13 12:07
1、处理systemctl无法重启cerebrod的问题

### 2023-02-10 17:00
- 1、处理新安装后显示不正确的问题
- 2、把zfs的包和ldiskfs的包区分名称处理
- 3、分支分开了，两个分支处理代码，main负责发布下载
### 2023-02-10 10:29
缺少依赖包perl-Date-Manip，放入到release包当中
