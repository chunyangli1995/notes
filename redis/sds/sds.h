struct sdshdr {
    // 记录buf数组中已用字节的数量
    // 等于SDS所保存的字符串的长度
    int len;

    // 记录buf数组中未使用字节的数量
    int free;

    // 字节数组 用于保存字符串
    char buf[];
}
