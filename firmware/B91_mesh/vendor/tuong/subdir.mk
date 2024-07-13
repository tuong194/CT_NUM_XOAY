################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/tuong/LCD_128.c \
../vendor/tuong/LCD_lvgl.c \
../vendor/tuong/LED_LC8823.c \
../vendor/tuong/Message_Control.c \
../vendor/tuong/RD_Secure.c \
../vendor/tuong/font.c \
../vendor/tuong/image.c \
../vendor/tuong/my_Function.c \
../vendor/tuong/switch.c 

OBJS += \
./vendor/tuong/LCD_128.o \
./vendor/tuong/LCD_lvgl.o \
./vendor/tuong/LED_LC8823.o \
./vendor/tuong/Message_Control.o \
./vendor/tuong/RD_Secure.o \
./vendor/tuong/font.o \
./vendor/tuong/image.o \
./vendor/tuong/my_Function.o \
./vendor/tuong/switch.o 

C_DEPS += \
./vendor/tuong/LCD_128.d \
./vendor/tuong/LCD_lvgl.d \
./vendor/tuong/LED_LC8823.d \
./vendor/tuong/Message_Control.d \
./vendor/tuong/RD_Secure.d \
./vendor/tuong/font.d \
./vendor/tuong/image.d \
./vendor/tuong/my_Function.d \
./vendor/tuong/switch.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/tuong/%.o: ../vendor/tuong/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


