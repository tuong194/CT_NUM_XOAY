################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/src/widgets/lv_arc.c \
../vendor/lvgl/src/widgets/lv_bar.c \
../vendor/lvgl/src/widgets/lv_btn.c \
../vendor/lvgl/src/widgets/lv_btnmatrix.c \
../vendor/lvgl/src/widgets/lv_canvas.c \
../vendor/lvgl/src/widgets/lv_checkbox.c \
../vendor/lvgl/src/widgets/lv_dropdown.c \
../vendor/lvgl/src/widgets/lv_img.c \
../vendor/lvgl/src/widgets/lv_label.c \
../vendor/lvgl/src/widgets/lv_line.c \
../vendor/lvgl/src/widgets/lv_objx_templ.c \
../vendor/lvgl/src/widgets/lv_roller.c \
../vendor/lvgl/src/widgets/lv_slider.c \
../vendor/lvgl/src/widgets/lv_switch.c \
../vendor/lvgl/src/widgets/lv_table.c \
../vendor/lvgl/src/widgets/lv_textarea.c 

OBJS += \
./vendor/lvgl/src/widgets/lv_arc.o \
./vendor/lvgl/src/widgets/lv_bar.o \
./vendor/lvgl/src/widgets/lv_btn.o \
./vendor/lvgl/src/widgets/lv_btnmatrix.o \
./vendor/lvgl/src/widgets/lv_canvas.o \
./vendor/lvgl/src/widgets/lv_checkbox.o \
./vendor/lvgl/src/widgets/lv_dropdown.o \
./vendor/lvgl/src/widgets/lv_img.o \
./vendor/lvgl/src/widgets/lv_label.o \
./vendor/lvgl/src/widgets/lv_line.o \
./vendor/lvgl/src/widgets/lv_objx_templ.o \
./vendor/lvgl/src/widgets/lv_roller.o \
./vendor/lvgl/src/widgets/lv_slider.o \
./vendor/lvgl/src/widgets/lv_switch.o \
./vendor/lvgl/src/widgets/lv_table.o \
./vendor/lvgl/src/widgets/lv_textarea.o 

C_DEPS += \
./vendor/lvgl/src/widgets/lv_arc.d \
./vendor/lvgl/src/widgets/lv_bar.d \
./vendor/lvgl/src/widgets/lv_btn.d \
./vendor/lvgl/src/widgets/lv_btnmatrix.d \
./vendor/lvgl/src/widgets/lv_canvas.d \
./vendor/lvgl/src/widgets/lv_checkbox.d \
./vendor/lvgl/src/widgets/lv_dropdown.d \
./vendor/lvgl/src/widgets/lv_img.d \
./vendor/lvgl/src/widgets/lv_label.d \
./vendor/lvgl/src/widgets/lv_line.d \
./vendor/lvgl/src/widgets/lv_objx_templ.d \
./vendor/lvgl/src/widgets/lv_roller.d \
./vendor/lvgl/src/widgets/lv_slider.d \
./vendor/lvgl/src/widgets/lv_switch.d \
./vendor/lvgl/src/widgets/lv_table.d \
./vendor/lvgl/src/widgets/lv_textarea.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/src/widgets/%.o: ../vendor/lvgl/src/widgets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


