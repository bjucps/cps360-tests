if "%1" == "" goto none
goto continue

:none
echo Usage: run asmtcode [ -d ]
goto done

:continue

setlocal
if "%2" == "-d" set DEBUG_OPT=-x
docker run -it --rm -v %cd%:/submission_src -v %~dp0:/tests  bjucps/cps360-test bash %DEBUG_OPT% tests/rundocker.sh %*

:done