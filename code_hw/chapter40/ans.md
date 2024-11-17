# 1
- seed 17:
```
inode bitmap  10000000
inodes       [d a:0 r:2][][][][][][][]
data bitmap   10000000
data         [(.,0) (..,0)][][][][][][][]
```
initialize fs with . and ..

```
inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (u,1)][(.,1) (..,0)][][][][][][]
```
mkdir /u, u to inode[1], allocate in data[1], direct link of root increased

```
inode bitmap  11100000
inodes       [d a:0 r:3][d a:1 r:2][f a:-1 r:1][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (u,1) (a,2)][(.,1) (..,0)][][][][][][]
```
create /a: a has only 1 link, it is a regular file, and a is empty

```
inode bitmap  11000000
inodes       [d a:0 r:3][d a:1 r:2][][][][][][]
data bitmap   11000000
data         [(.,0) (..,0) (u,1)][(.,1) (..,0)][][][][][][]
```
unlink /a: inode and directory of a removed

```
inode bitmap  11100000
inodes       [d a:0 r:4][d a:1 r:2][d a:2 r:2][][][][][]
data bitmap   11100000
data         [(.,0) (..,0) (u,1) (z,2)][(.,1) (..,0)][(.,2) (..,0)][][][][][]
```

mkdir /z: z with inode[2], allocated in data[2] and have 2 direct link, is a directory

```
inode bitmap  11110000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0)][(.,3) (..,0)][][][][]
```

mkdir /s: s with inode[3], allocated in data[3], have 2 direct link, is a directory

```
inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0) (x,4)][(.,3) (..,0)][][][][]
```

create /z/x, x have 1 direct link, is a regular file, with inode[4]

```
inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:-1 r:2][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0) (b,4)][(.,2) (..,0) (x,4)][(.,3) (..,0)][][][][]
```
link /z/x, /u/b, u/b increases and shares the inode with z/x, z/x's direct link increased to 2, bit map shows no new file created and allocated
```
inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:-1 r:1][][][]
data bitmap   11110000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0) (x,4)][(.,3) (..,0)][][][][]
```
unlink /u/b inode[4] direct link and data for /u/b removed
```
inode bitmap  11111000
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:4 r:1][][][]
data bitmap   11111000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0)][(.,2) (..,0) (x,4)][(.,3) (..,0)][v][][][]
```
write to /z/x, /z/x becomes a non empty regular file, which can see in data bitmap and inode

answer is fd=open("/z/x", O_WRONLY|O_APPEND); write(fd, buf, BLOCKSIZE); close(fd);

```
inode bitmap  11111100
inodes       [d a:0 r:5][d a:1 r:2][d a:2 r:2][d a:3 r:2][f a:4 r:1][f a:-1 r:1][][]
data bitmap   11111000
data         [(.,0) (..,0) (u,1) (z,2) (s,3)][(.,1) (..,0) (b,5)][(.,2) (..,0) (x,4)][(.,3) (..,0)][v][][][]
```
create /n/b, b is a empty regular file, have direct link of 1 and not allocated


# 2
sequentially, from the front to the end, take the first unoccupied spot

# 3
cannot allocate if the data node number is 2

# 3
same
