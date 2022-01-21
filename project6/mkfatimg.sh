#!/bin/bash

# Execute me using sudo

mkfs.fat -C -f 1 -F 16 -r 112 -s 1 fat.img  4096
mkdir /mnt/fatdisk
mount -o loop fat.img /mnt/fatdisk
echo Hello > /mnt/fatdisk/hello.txt
echo "This is a test file" > /mnt/fatdisk/test.txt
umount /mnt/fatdisk