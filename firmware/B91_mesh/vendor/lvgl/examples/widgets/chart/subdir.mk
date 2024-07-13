################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_1.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_2.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_3.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_4.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_5.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_6.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_7.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_8.c \
../vendor/lvgl/examples/widgets/chart/lv_example_chart_9.c 

OBJS += \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_1.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_2.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_3.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_4.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_5.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_6.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_7.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_8.o \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_9.o 

C_DEPS += \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_1.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_2.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_3.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_4.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_5.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_6.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_7.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_8.d \
./vendor/lvgl/examples/widgets/chart/lv_example_chart_9.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/examples/widgets/chart/%.o: ../vendor/lvgl/examples/widgets/chart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


