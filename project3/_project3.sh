require-files Makefile encrypt.c rot47.c install.sh

[ -e encrypt.ko ] && rm *.ko
[ -e rot47 ] && rm rot47

do-compile --expect-exe encrypt.ko make

require-files  --test-message "Compile produced rot47" rot47

run-program --test-message "install.sh runs without error" sudo bash install.sh

run-program --test-message "rot47 runs without error" --showoutputonpass  ./rot47 '"The Quick Brown Fox Jumps Over The Lazy Dog."'

run-program --test-message "rot47 runs without error with keys" --showoutputonpass  ./rot47 '"Fox KEY:01 Fox Fox KEY:02 Fox"'

require-files  --test-message "Report submitted" REPORT.md 
