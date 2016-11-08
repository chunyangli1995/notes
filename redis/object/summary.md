## 对象的类型和编码

redis使用对象来表示数据库中的键值。redis中的每个对象都由一个redisObject结构表示，该结构和保存数据有关的三个属性分别是type、encoding和ptr属性。

    typedef struct redisObject {
		// 类型
 		unsigned type: 4；

		// 编码
 		unsigned encoding: 4；

		// 指向底层实现的数据结构的指针
		void *ptr;
    } robj;

### 类型

type记录了对象的类型，type可以是下列常量的任何一个：

* REDIS_STRING 
* REDIS_LIST
* REDIS_HASH
* REDIS_SET
* REDIS_ZSET

redis中保存的键值对而言，键总是字符串对象，而值可以是五种对象的任一种。

## 编码和底层实现

对象的ptr指针指向对象的底层数据结构，而这些数据结构由对象的encoding属性决定。

encoding属性决定了对象使用什么数据结构作为底层的实现。这个属性的值可以是以下的任一个：

* REDIS\_ENCODING\_INT
* REDIS\_ENCODING\_EMBSTR
* REDIS\_ENCODING\_RAW
* REDIS\_ENCODING\_HT
* REDIS\_ENCODING\_LINKEDLIST
* REDIS\_ENCODING\_ZIPLIST
* REDIS\_ENCODING\_INTSET
* REDIS\_ENCODING\_SKIPLIST

每种类型的对象都至少使用了两种编码。

通过encoding属性来设置对象所使用的编码，极大地提升了redis的灵活性和效率。因为redis可以根据不同的使用场景来为一个对象设置不同的编码，从而优化对象在某个场景下的性能。


几个小特性：

* Redis的对象系统带有引用计数实现的内存回收机制，当一个对象不再被使用的时候，该对象占用的内存就会被自动释放。

* redis会共享0-9999的字符串对象

* redis对象会记录自己的最后一次被访问的时间，这个属性可以用于计算对象的空转时间。

