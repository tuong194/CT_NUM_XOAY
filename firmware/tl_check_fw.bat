@echo off
echo "*************** start of post build *****************"
echo "this is post build!!! current configure is :%1"
riscv32-elf-objcopy -S -O binary %1.elf  output/%1.bin

:: par[0]:EXE path;
:: par[1]:bin path; 
:: par[2]: 1 means FW_CHECK_AGTHM1, 2 means FW_CHECK_AGTHM2;  other values is invalid; In encryption mode, AGTHM2 is automatically used.
:: par[3]: signature private key with 32 bytes. 0 means no signature.  (Optional)
:: par[4]: "2.elf" means that if there is 'key_encode_bin' in firmware, encryption mode will be enable. 0 means no encryption mode. (Optional)

..\tl_auth_check_fw.exe  output/%1.bin  2  0  %1.elf
:: echo result=%errorlevel%

if  %errorlevel% == 0 (
echo  "exec successful"
) else (
echo  ":1: ###########################  Error: Run tl_auth_check_fw.exe failed  ###########################"
echo  ":1: ###########################  Error: Run tl_auth_check_fw.exe failed  ###########################"
echo  ":1: ###########################  Error: Run tl_auth_check_fw.exe failed  ###########################"
echo  ":1: Error: exec failed, exit code=%errorlevel%"
rm -rf output/%1.bin
)

echo  "---------------------------  SDK version info ---------------------------"
grep -E "[\$]{3}[a-zA-Z0-9 _.]+[\$]{3}" --text -o output/%1.bin | sed 's/\$//g'
echo  "---------------------------  SDK version end  ---------------------------"

echo "**************** end of post build ******************"
exit 0