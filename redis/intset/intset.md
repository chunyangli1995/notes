## 整数集合的实现

    typedef struct intset {
        
		// 编码方式
		uint32_t encoding;

		// 集合中包含元素的数量
  		uint32_t length;

		// 保存元素的数组
		int8_t contents[];
 
    } intset;

> 虽然intset结构将contents属性声明为int8_t类型的数组，但是实际上contents数组并不保存任何int8_t类型的值，contents的数组的真正类型取决于encoding的值。

## 整数集合的升级

每当我们把一个新的元素插入到一个整数集合的时候，并且新元素的类型比整数集合中的所有元素类型都要长时，整数集合需要先进行升级，才能把新元素插入到整数集合里。升级步骤如下：

1. 根据新元素的类型，扩展整数集合底层数组的空间大小，并为新元素分配空间。
2. 将底层数组所拥有的全部元素都转换成和新元素相同的类型，并把转换后的元素放到正确的位上，而且在放置的过程中，要维持底层数组的有序顺序不变。
3. 将新元素添加到底层数组里

向整数集合中添加新元素的时间复杂度是O（N）。

## 升级的好处

1. 提升灵活性
 
	因为C语言是静态类型的语言，为了避免类型的错误，我们通常不会把两种类型的值放在同一个数据结构里。

2. 节约内存

	目前的实现保证只有在需要的时候才会去进行升级操作，而不是一开始就把类型定义成最大的。

## 降级

整数集合不支持降级操作，一旦对数组进行了升级，编码就会一直保持升级后的状态。

