## Hello World

```bash
#!/bin/bash
echo "Hello World !"
```
- `#!`指定脚本使用的系统解释器

# Shell变量

```bash
your_name="Ryan"
echo $your_name
echo ${your_name}
```

```
for skill in Ada Coffe Action Java; do
	echo "I am good at ${skill}Script"
done
```

- `readonly` 将变量定义为只读变量
- `unset` 删除变量，变量被删除后不能再次使用

## 变量类型

- 局部变量：仅在当前shell实例中有效
- 环境变量：所有程序，包括shell启动的程序，均可以访问环境变量
- shell变量：由shell程序设置的特殊变量，其中一部分是局部变量，一部分是环境变量；shell变量确保shell的正常运行

linux中shell变量`$#`,`$@`,`$0`,`$1`,`$2`的含义解释: 
变量说明: 
`$$` Shell本身的PID（ProcessID） 
`$!` Shell最后运行的后台Process的PID 
`$?` 最后运行的命令的结束代码（返回值） 
`$-` 使用Set命令设定的Flag一览 
`$*` 所有参数列表。如"$*"用「"」括起来的情况、以"$1 $2 … $n"的形式输出所有参数。 
`$@` 所有参数列表。如"$@"用「"」括起来的情况、以"$1" "$2" … "$n" 的形式输出所有参数。 
`$#` 添加到Shell的参数个数 
`$0` Shell本身的文件名 
`$1`～`$n` 添加到Shell的各参数值。$1是第1参数、$2是第2参数…。 