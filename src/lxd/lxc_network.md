

## lxd network

```shell
lxc stop c1
lxc network attach lxdbr0 c1 eth0 eth0
lxc config device set c1 eth0 ipv4.address 10.99.10.42
lxc start c1
```