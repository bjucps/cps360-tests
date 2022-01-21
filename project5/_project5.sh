function run_test {
  EXE=$1
  NUM_BYTES=$2

  if [ -e $EXE ]; then
    # Generate some random data
    dd if=/dev/urandom of=testfile bs=$NUM_BYTES count=1 2>/dev/null
    # Compute reverse of file
    perl -0777pe '$_=reverse $_' testfile > testfile_rev

    result=FAIL
    if run-program ./$EXE testfile; then
      if diff testfile testfile_rev >/dev/null; then
        result=PASS
      fi
    fi
    report-result $result "Warning" "$EXE produces correct results on $NUM_BYTES file"
  else
    echo "No $EXE executable detected..."
    exit 1
  fi

}


require-files --test-message "Makefile exists" Makefile reversemm.c reversecio.c

do-compile --expect-exe reversemm make

exit-if-must-pass-tests-failed

run_test reversemm 16K
run_test reversecio 16K

if [ -e reverseciov2 ]; then
  run_test reverseciov2 16K 
  run_test reverseciov2 10M
fi

require-files  --test-message "Report submitted" REPORT.md 
