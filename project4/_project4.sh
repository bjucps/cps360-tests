require-files --test-message "Makefile exists" Makefile 

echo "Looking for project ..."
if [ -e kernel/sign.pl ]; then
  echo "Kernel threads project detected."
  chmod +x kernel/sign.pl
  cp $TEST_DIR/*.c user
else
  report-error "$MUST_PASS" "No project detected."
fi

exit-if-must-pass-tests-failed

echo "Executing make clean ..."
make clean >/dev/null 2>&1

do-compile make

exit-if-must-pass-tests-failed

# Can't use timeout with qemu, so use sleep to simulate.
#let timeout=15
echo -e "\nRunning my tests ..."
make run &
QPID=$!
#sleep $timeout
#kill $QJOB 2>/dev/null
wait $QPID

require-files  --test-message "Report submitted" REPORT.md 
