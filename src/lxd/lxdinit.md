
## lxd init(2019)

 1. snap install lxd --channel=3.0/stable


```shell
ryefccd@fccd:~$ snap install lxd --channel=3.0/stable
lxd (3.0/stable) 3.0.4 from Canonical✓ installed
```

 2. sudo lxd init
```shell
ryefccd@fccd:~$ sudo lxd init
Would you like to use LXD clustering? (yes/no) [default=no]: 
Do you want to configure a new storage pool? (yes/no) [default=yes]: 
Name of the new storage pool [default=default]: 
Name of the storage backend to use (btrfs, ceph, dir, lvm, zfs) [default=zfs]: 
Create a new ZFS pool? (yes/no) [default=yes]: 
Would you like to use an existing block device? (yes/no) [default=no]: 
Size in GB of the new loop device (1GB minimum) [default=42GB]: 
Would you like to connect to a MAAS server? (yes/no) [default=no]: 
Would you like to create a new local network bridge? (yes/no) [default=yes]: 
What should the new bridge be called? [default=lxdbr0]: 
What IPv4 address should be used? (CIDR subnet notation, “auto” or “none”) [default=auto]: 
What IPv6 address should be used? (CIDR subnet notation, “auto” or “none”) [default=auto]: 
Would you like LXD to be available over the network? (yes/no) [default=no]: yes
Address to bind LXD to (not including port) [default=all]: 
Port to bind LXD to [default=8443]: 
Trust password for new clients: 
Again: 
Would you like stale cached images to be updated automatically? (yes/no) [default=yes] 
Would you like a YAML "lxd init" preseed to be printed? (yes/no) [default=no]: yes
config:
  core.https_address: '[::]:8443'
  core.trust_password: fccdjny
networks:
- config:
    ipv4.address: auto
    ipv6.address: auto
  description: ""
  managed: false
  name: lxdbr0
  type: ""
storage_pools:
- config:
    size: 42GB
  description: ""
  name: default
  driver: zfs
profiles:
- config: {}
  description: ""
  devices:
    eth0:
      name: eth0
      nictype: bridged
      parent: lxdbr0
      type: nic
    root:
      path: /
      pool: default
      type: disk
  name: default
cluster: null
```
 3. 重命名原来的 image 源, 并添加清华的源

```shell
lxc remote rename images images_bak
lxc remote add images https://mirrors.tuna.tsinghua.edu.cn/lxc-images/ --protocol=simplestreams --public
```

 4. lxc image copy images:ubuntu/16.04 local:
```shell
ryefccd@fccd:~$ lxc image list
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
| ALIAS | FINGERPRINT  | PUBLIC |                  DESCRIPTION                  |  ARCH  |   SIZE   |         UPLOAD DATE          |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 368bb7174b67 | no     | ubuntu 18.04 LTS amd64 (release) (20190722.1) | x86_64 | 177.56MB | Jul 29, 2019 at 2:07am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 4bfe62583826 | no     | ubuntu 14.04 LTS amd64 (release) (20190514)   | x86_64 | 122.40MB | Jul 29, 2019 at 4:52am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 5337a61fbc44 | no     | Ubuntu trusty amd64 (20190728_07:42)          | x86_64 | 74.81MB  | Jul 29, 2019 at 4:00am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
|       | 8b430b6d8271 | no     | ubuntu 16.04 LTS amd64 (release) (20190628)   | x86_64 | 158.72MB | Jul 29, 2019 at 4:07am (UTC) |
+-------+--------------+--------+-----------------------------------------------+--------+----------+------------------------------+
```

 5. lxc launch local:8b430b6d8271 redisai

```shell
ryefccd@fccd:~$ lxc list
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
|  NAME   |  STATE  |         IPV4         |                     IPV6                      |    TYPE    | SNAPSHOTS |
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
| redisai | RUNNING | 10.213.178.59 (eth0) | fd42:8fa4:9940:a6aa:216:3eff:fe16:efaa (eth0) | PERSISTENT | 0         |
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
| whtest  | STOPPED |                      |                                               | PERSISTENT | 0         |
+---------+---------+----------------------+-----------------------------------------------+------------+-----------+
```

 6. lxc exec redisai -- sudo --user ubuntu --login
 7. 制作本地镜像
	
	 lxc publish my-container --alias my-new-image
	 lxc publish my-container/some-snapshot --alias some-image

