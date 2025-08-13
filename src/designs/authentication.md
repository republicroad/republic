
# authentication

authentication 认证, 用于系统中对访问者进行身份验证, 确认访问者的身份. 最常见的身份认证功能就是用户名和密码认证机制. 还有一些社区登录系统，依赖某种大型系统已经认证的信息，但是其源头仍然是那些被信任系统的用户密码认证体系。

## 密码
如何存储用户认证信息，尤其是密码的存储是认证系统的关键。
对于密码存储，业界的共识是存储密码被散列后的字符，因为黑客会把简单密码的散列值也算出来做成一个数据表来暴力破解(彩虹表)，所以一般需要把密码和一个随机的字符串(salt值)一起散列，这样抵抗那些彩虹表的破解。  
所以现在有了一些把上述密码散列的标准实践, 出现了一些用于密码散列的专用算法，帮忙处理多轮加盐散列，以增加不可预测性，避免彩虹表的攻击. 比如 **bcrypt**, **Scrypt**, **PBKDF2**, **Argon2**等。

密码散列算法会把上述使用的多轮加盐散列算法的计算和校验编码到同一个编码串中, 从而方便对密码进行验证。这样可以做到不存储密码明文而对密码进行校验的目的。

下列展示一些语言关于密码操作的最佳实践。
### passlib(python)


```python
>>> from passlib.hash import bcrypt

>>> # generate new salt, hash password
>>> h = bcrypt.hash("password")
>>> h
'$2a$12$NT0I31Sa7ihGEWpka9ASYrEFkhuTNeBQ2xfZskIiiJeyFXhRgS.Sy'

>>> # verify password
>>> bcrypt.verify("password", h)
True
>>> bcrypt.verify("wrong", h)
False
```

对`密码`的散列值如下:

> `$2a$12$NT0I31Sa7ihGEWpka9ASYrEFkhuTNeBQ2xfZskIiiJeyFXhRgS.Sy`

散列后的格式是 `${hashes}${rounds}${salt}{checksum}`, 密码散列后出现的值中包含了一些元信息, 用什么算法散列, 计算轮次, 用的盐值和最后的校验字符串 :

- `{hashes}` 是指使用的密码散列算法， 2a 是 bcrypt 使用的其中一种密码散列算法([不同操作系统支持的密码散列算法](https://passlib.readthedocs.io/en/stable/modular_crypt_format.html#os-defined-hashes)).
- `{rounds}`是一个计算代价参数, 是一个使用2为底数的指数计算`{iterations}=2**{rounds}`，在此示例中，`{rounds}`为12，最终迭代次数是 4096(`2**12`)
-  `{salt}` 是一个由 22 个字符组成的盐字符串，使用正则表达式范围 [./A-Za-z0-9] 内的字符（示例中为 NT0I31Sa7ihGEWpka9ASYr）。
- `{checksum}`  是一个 31 个字符的校验和，使用与盐后面的字符(示例中为 EFkhuTNeBQ2xfZskIiiJeyFXhRgS.Sy)。

以上这些参数可以修改:
```python
>>> from passlib.hash import bcrypt

>>> # the same, but with an explicit number of rounds
>>> bcrypt.using(rounds=13).hash("password")
'$2b$13$HMQTprwhaUwmir.g.ZYoXuRJhtsbra4uj.qJPHrKsX5nGlhpts0jm'
```

### bun(typescript)

**bun 默认使用 argon2**

```ts
import {password} from "bun";

const hash = await password.hash("hello world");
const verify = await password.verify("hello world", hash);
console.log(verify); // true
```

**使用参数指定使用bcrypt**

```ts
import {password} from "bun";

const hash = await password.hash("hello world", "bcrypt");
// algorithm is optional, will be inferred from the hash if not specified
const verify = await password.verify("hello world", hash, "bcrypt");

console.log(verify); // true
```

https://bun.sh/reference/bun/password  
[Hash a password with Bun](https://bun.com/guides/util/hash-a-password)  


## 多因子认证(MFA)

 multi-factor authentication(MFA)




## 参考资料

[Authentication and Authorization Best Practices](https://blog.gitguardian.com/authentication-and-authorization/)  
