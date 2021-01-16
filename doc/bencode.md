# What is Bencode

bencode is a data format. bencode support 4 data type.

- number
- string and binary
- list
- dict

Bencode spec is defined at http://bittorrent.org/beps/bep_0003.html


### Number
Number 123 is described as follows

```
i123e
```

### String
String hello is described as follows

```
5:hello
```

### List

List has hello and 12 which is described as follows

```
l5:helloi12ee
```

### Dict 
A dict is stored  a element which conbination of "num" key and 12 value.   
d3:numi12ee


# libtorrent sample  code

app/main_bencode.cpp
app/main_bdecode.cpp


