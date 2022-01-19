require-files Makefile encrypt.c rot47.c install.sh

[ -e encrypt.ko ] && rm *.ko
[ -e rot47 ] && rm rot47

do-compile --expect-exe encrypt.ko make

require-files  --test-message "Compile produced rot47" rot47

require-files  --test-message "Report submitted" REPORT.md 
