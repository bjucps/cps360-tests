require-files --test-message "Makefile exists" Makefile 

echo "Looking for project ..."
if [ -e kernel/sign.pl ]; then
  echo "Kernel threads project detected."
  chmod +x kernel/sign.pl
  cp $TEST_DIR/*.c user
elif [ -e ut_threads.c ]; then
  echo "User threads project detected."
  require-files --test-message "ut_threads.h exists" ut_threads.h
else
  report-error "$MUST_PASS" "No project detected."
fi

exit-if-must-pass-tests-failed

echo "Executing make clean ..."
make clean

do-compile make

exit-if-must-pass-tests-failed

if [ -e ut_threads.c ]; then
  echo "Running user threads test..."
  run-program --test-message "make test runs without error" --timeout 5 make test
fi

require-files  --test-message "Report submitted" REPORT.md 
