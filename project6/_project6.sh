require-files --test-message "Makefile exists" Makefile 

git clone --depth 1 https://github.com/sschaub/cps360

echo Executing make clean...
make clean >/dev/null 

cp $TEST_DIR/init.c user

for TEST in tester_basic.c tester_bigram.c tester_mrd.c tester_ramdirs.c tester_fat.c
do
   cp $TEST_DIR/$TEST user/tester.c
   echo Testing with $TEST ...

   if do-compile --test-message "Successful compile with $TEST" make
   then
    # For FAT read test...
    cp $TEST_DIR/fat.img test.img

    echo Executing tests...
    # Run in background because qemu doesn't work well run in foreground with a timeout in effect
    qemu-system-i386 -nographic -drive file=xv6.img,index=0,media=disk,format=raw -drive file=fs.img,index=1,media=disk,format=raw -smp 1 -device isa-debug-exit,iobase=0xf4,iosize=0x04 &
    QPID=$!
    countdown=1
    while [ $countdown -gt 0 ]
    do
       sleep 1
       if ! stdout=$(ps | grep qemu)
       then
          break
       fi
       let countdown=countdown-1
    done

    if stdout=$(ps | grep qemu)
    then
        echo -e "\n*** WARNING: Test did not complete in allotted time"
        kill $QPID 2>/dev/null
    fi
   fi
done

require-files  --test-message "Report submitted" REPORT.md 
