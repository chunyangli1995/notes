## 数据库的切换

每个redis客户端都有自己的目标数据库，默认是0号数据库，可以通过**select**命令选择数据库，RedisClient的db指针指向服务器中的不同数据库。

## 数据库的键空间

服务器中的每个数据库都由一个redisDb结构表示，其中，redisDB结构中的dict字典保存了数据库中的所有键值对，将这个字典称为键空间。

## 过期时间的保存

redisDB的expires字典保存了数据库中所有键的过期时间，我们称这个字典为过期字典。

* 键是一个指针，指向键空间中的某个键对象

* 值是一个long long类型的整数，这个整数保存了键所指向的数据库键的过期时间——一个毫秒精度的unix时间戳。

## 过期键的判定

* 查看给定键是否存在于过期字典，如果存在，那么取得键的过期时间
* 查看当前unix时间戳是否大于键的过期时间，如果是的话，那么键已经过期，否则的话，键未过期。

## 过期键的删除策略

**一个键过期了，什么时候会被删除呢？**

* 定时删除：给键设置过期时间的时候，顺便创建一个定时器，让定时器在键的过期时间来临的时候，立刻执行对键的删除。
* 惰性的删除：每次从键空间获取键的时候，看一个该键是否过期，过期的话就删除。
* 定期删除：每过一段时间，就对数据库做一次检查，删除里面的过期键。

## 三种策略优缺点

* 定时： 占用太多CPU时间，影响服务器的响应时间和吞吐量。
* 惰性：造成内存空间的浪费，有内存泄露风险。
* 定期：两者的折中。

实际中，redis是把惰性和定期配合来使用。

## AOF、RDB和复制功能对过期键的影响

* RDB文件在生成的时候会把过期键排除掉，而载入RDB文件的时候，主服务器会过滤掉过期键，从服务器不会过滤掉，但是主从服务器在数据同步的时候，从服务器的数据库会被清空，所以一般来说，过期键对载入RDB文件不会有什么影响。
* AOF写入的时候，会对删除过期键操作记录一条DEL，重写的时候会把过期键给排除掉。
* 复制：主服务器在删除一个过期键的时候，会向所有从服务器发送一个DEL命令，告诉从服务器删除这个过期键。从服务器接收到客户端的读请求的时候，即使遇到过期键也不做特殊的处理。

