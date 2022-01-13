require-files Makefile 

for FILE in my-cat my-grep my-zip my-unzip; do
  if [ -e $FILE ]; then
    rm $FILE
  fi
done

do-compile make

echo "my- executables produced:"
ls my-cat my-grep my-zip  || report-error "$CAT_MUST_PASS" "At least one my- executable produced"

exit-if-must-pass-tests-failed

cp -r $TEST_DIR/test_files .

python3 $TEST_DIR/project1.py 

if [ -e zip.c -a -r unzip.c ]; then
  git clone https://github.com/sschaub/cps360
  cp *zip.c cps360/xv6/user
  cp $TEST_DIR/makefile.mk cps360/xv6/user
  cp $TEST_DIR/*.c cps360/xv6/user
  # Can't use timeout with qemu, so use sleep to simulate.
  echo -e "\nRunning xv6 zip/unzip test ..."
  cd cps360/xv6
  do-compile  make
  CPUS=1 make run &
  QPID=$!
  sleep 5
  kill $QPID 2>/dev/null
  cd ../..
fi

require-files  --test-message "Report submitted" REPORT.md 
