# OTUSLesson17
**bayan** -- solution to lesson 17 homework otus.ru (task 09)



## Description

**bayan** is utility for searching identical files.



## Start parameters for bayan

-h, --help			Help output

-b, --blocksize	block size (bytes) to scan for files compare (default = 1)

-H, --hash			hash type: *boost*, *crc32*, *md5*

-f,  --files			  list of files to search identical (can be set without -f)	

**Example**: `bayan -b 3 -H crc32 file1.txt file2.txt file3.txt `