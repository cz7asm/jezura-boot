#!/bin/bash

rm -f flash.bin
dd if=/dev/zero of=flash.bin bs=1M count=16
dd if=spl/u-boot-spl.bin of=flash.bin conv=notrunc
dd if=u-boot.bin of=flash.bin bs=1024 seek=33 conv=notrunc
printf '\x00\x60\x00\x00' | dd of=flash.bin bs=1 seek=20 conv=notrunc

