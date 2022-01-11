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

require-files  --test-message "Report submitted" REPORT.md 
