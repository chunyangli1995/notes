## 频道的订阅和退订

当一个客户端执行subscribe命令订阅一个或者多个频道的时候，这个客户端就和被订阅的频道之间建立起了一种联系。

redis将所有频道的订阅关系都保存在服务器状态的pubsub_channels字典里面，键是某个被订阅的频道，值是一个链表，保存了所有订阅了这个频道的客户端。

## 模式的订阅和退订

服务端将所有模式的订阅关系保存在服务器状态的pubsub_patterns属性里面，pubsub_patterns属性是一个链表，链表中的每个节点都包含着一个pubsubPatterns结构，这个结构的pattern属性记录了被订阅的模式，而client属性记录了订阅模式的客户端。

## 发送消息

当一个客户端把命令发送给服务端的时候，服务端需要执行以下两个操作，

1. 将message消息发送给channel的所有订阅者
2. 如果有一个或者多个pattern与channel匹配，那么将消息发送给pattern模式的订阅者

## 查看订阅消息

使用pubsub命令

### pubsub channels

用于返回当前服务器被订阅的频道，其中patterns参数可选，

* 如果不给定pattern参数，那么命令返回服务器当前订阅的所有频道
* 给定pattern参数的话，只返回当前频道中哪些与pattern模式相匹配的频道

### pubsub numsub

接受多个频道作为输入参数，并返回这些频道的订阅者数量。

### pubsub numpat

用于返回当前服务器被订阅模式的数量
