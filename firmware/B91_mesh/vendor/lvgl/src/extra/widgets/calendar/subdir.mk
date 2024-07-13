################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/src/extra/widgets/calendar/lv_calendar.c \
../vendor/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.c \
../vendor/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.c 

OBJS += \
./vendor/lvgl/src/extra/widgets/calendar/lv_calendar.o \
./vendor/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.o \
./vendor/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.o 

C_DEPS += \
./vendor/lvgl/src/extra/widgets/calendar/lv_calendar.d \
./vendor/lvgl/src/extra/widgets/calendar/lv_calendar_header_arrow.d \
./vendor/lvgl/src/extra/widgets/calendar/lv_calendar_header_dropdown.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/src/extra/widgets/calendar/%.o: ../vendor/lvgl/src/extra/widgets/calendar/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


