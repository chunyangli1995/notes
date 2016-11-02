### Python的filter
给定一个列表，如果过滤掉其中值为None的，怎么实现呢？之前我是使用lambda这么实现的：
    
    filter(lambda e: e, elems)
但其实可以这样

    filter(None, elems)
