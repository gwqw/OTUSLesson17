# Bayan
[![Build Status](https://travis-ci.org/gwqw/OTUSLesson17.svg?branch=master)](https://travis-ci.org/gwqw/OTUSLesson17/)

## Description

**bayan** is utility for searching identical files. It outputs identical files divided into groups. 

## Start parameters for bayan

```bash
-h, --help			Help output
-b, --blocksize	block size (bytes) used to compare files (default = 1)
-H, --hash			hash type: *boost*, *crc32*
-f, --files			file list to find duplicates (can be set without option `-f`)
-d,  --dir			file list will be obtained from the specified directory recursively (-f is ignored)
```

**Examples**: 

`bayan -b 4096 -H crc32 file1.txt file2.txt file3.txt`
`bayan -b 10240 -H boost -d /home/username/`
`bayan -b 4096` - scan current directory for duplicates


## Dependencies

* boost libraries (crc, container hash, program options, filesystem)