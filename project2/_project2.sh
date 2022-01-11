sudo apt-get update && apt-get install -y qemu qemu-kvm
require-files Makefile 

# Copy pstat.h to user folder
if [[ ! -e user/pstat.h && ! -e include/pstat.h ]]
then
  if [ -e kernel/pstat.h ]
  then
    cp kernel/pstat.h user
  else
    echo 'ALERT: No user/pstat.h or include/pstat.h found.'
    report-error "$CAT_MUST_PASS" "pstat.h submitted in user or include folder"
  fi
fi

echo Executing make clean...
make clean >/dev/null 2>&1

cp $TEST_DIR/*.c user

do-compile make

exit-if-must-pass-tests-failed
 
# Can't use timeout with qemu, so use sleep to simulate.
CPUS=1 make run &
QPID=$!
sleep 10
kill $QPID

require-files  --test-message "Report submitted" REPORT.md 
