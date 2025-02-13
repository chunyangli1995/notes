## 事务的实现

一个事务从开始到结束会经历三个阶段：

1. 事务开始
2. 命令入队
3. 事务执行

### 事务开始

multi命令标志着事物的开始：

这个命令可以将执行命令的客户端从非事务状态转换为事务状态，这一切换通过在客户端状态的flags属性中打开REDIS_MULTI标识来完成的。

### 命令的执行

当客户端处于非事务状态的时候，客户端发送的命令服务端会立刻执行，但是当客户端处于事务状态下，服务端会根据客户端发来的不同状态执行不同的命令：

1. 如果客户端发送的命令是exec、discard、watch、multi四个命令中的一个，那么服务器立刻执行这个命令
2. 如果不是上述四个命令中的一个，服务端会将这些命令放入一个事务队列中，然后向客户端返回queued回复。

### 事务队列

客户端redis客户端都有自己的事务状态，这个事务状态保存在客户端状态的mstate属性里面。

    typedef struct redisClient {
		// ...

		// 事务状态
		multiState mstate;

		// ...
	} redisClient;

事务状态包含一个事务队列，以及一个已入队命令的计数器

	typedef struct multiState {
		
		// 事务队列FIFO模式
		moltiCmd *commands;

		// 已入队命令计数
		int count;
	} multiState;

### 执行事务

 当一个处于事务状态的客户端向服务端发送exec命令时，这个命令会立刻被服务器执行，服务器会遍历这个客户端的事务队列，执行队列中保存的命令，最后将执行命令所得的结果全都返回给客户端。

## watch命令的实现

乐观锁机制，可以在exec命令执行之前，监视任意数量的数据库键，并在exec命令执行的时候，检查被监视的键是否至少有一个已经被修改过了。如果是的话，服务器将拒绝执行事务，并向客户端返回一个空回复。

### 使用watch命令监视数据库键

每个redis数据库都保存了一个watched_keys字典，这个字典的键是某个被watch命令监视的数据库键，而字典的值是一个链表，链表记录了所有监视了这个键的客户端。

### 监视机制的触发

所有对数据库进行修改的命令，执行之后都会对watched_keys字典进行检查，查看是否有客户端正在监视刚刚被命令修改过的数据库键，如果有的话，那么会将被监视的客户端的redis_dirty_cas标识打开，表示该客户端的安全性已经遭到了破坏。

### 判断事务是否安全

当服务端收到了客户端发来的exec命令的时候，服务端会根据这个客户端是否打开了redis_dirty_cas标识来决定是否执行事务。

## 事务的ACID属性

### 原子性

对于redis的事务功能来说，事务队列中的命令要么全部执行，要么一个都不执行，因此，redis中的事务是有原子性的。

redis和传统的关系型数据库事务的最大区别是，redis不支持事务回滚机制，即使事务队列中的某个命令在执行期间出现了错误，整个事务也会继续执行下去。

### 一致性

事务的一致性指的是，如果数据库在执行事务之前是一致的，那么在事务执行之后，无论事务是否执行成功，数据库也仍然应该是一致的。

下面是三个redis可能出现的错误以及redis的处理方式：

#### 入队错误

如果一个事务在入队的过程中，出现了命令不存在，或者命令格式不正确的情况，那么reids将拒绝这个事务

#### 执行错误

事务执行过程中可能有命令出错，这种情况下服务器会继续执行队列中的其他命令

#### 服务器停机

aof或者rdb或者空都不会出现不一致的问题

### 隔离性

事务的隔离性指的是即使数据库中多个事务并发的执行，各个事务之间也不会相互影响，并且在并发情况下执行事务和穿行情况下执行事务结果完全一样

redis使用单线程的方式来执行事务，所以事务之间是串行执行的，而且服务器保证， 在执行期间不会出现什么中断操作。

### 耐久性

事务的耐久性指的是，当一个事务执行完毕之后，执行这个事务所得的结果已经被保存到永久性存储介质里了，即使服务器在事务执行完毕之后停机，执行事务所得的结果也不会丢失。

redis事务的持久性由redis所使用的持久化模式来决定。