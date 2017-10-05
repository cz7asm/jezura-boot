#!/bin/bash

rm -f flash.bin
dd if=/dev/zero bs=1M count=16 | tr '\000' '\377' > flash.bin
dd if=spl/u-boot-spl.bin of=flash.bin conv=notrunc
dd if=u-boot.bin of=flash.bin bs=1024 seek=32 conv=notrunc
printf '\x00\x60\x00\x00' | dd of=flash.bin bs=1 seek=20 conv=notrunc

