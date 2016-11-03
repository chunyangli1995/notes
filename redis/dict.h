typedef struct dictht {
    // 哈希表数组
    dicEntry **table;

    // 哈希表大小
    unsigned long size;

    // 哈希表的大小掩码 用于计算索引值
    // 总是等于size-1
    unsigned int sizemask；

    // 该哈希表已有节点的数量
    unsigned int used;
} dictht;

typedef struct dictEntry {
    // 键
    void *key;

    // 值 可以是一个指针、一个64位无符号整数或者64有符号整数
    union {
        void *val;
        uint64_tu64;
        int64_ts64;
    };

    //指向下一个哈希表节点 为了使用拉链法来解决哈希冲突
    struct dictEntry *next;

} dictEntry;

typedef struct dict {

    // 类型特定函数
    dictType *type;

    // 私有数据 和type两个字段是为了创建多态的字典而设置的
    void *private;

    // 哈希表
    dictht ht[2];

    // rehash索引
    // 当rehash不在进行时  值为-1
    int trehashidx;
}
