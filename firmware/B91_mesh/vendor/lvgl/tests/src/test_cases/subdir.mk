################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/tests/src/test_cases/_test_template.c \
../vendor/lvgl/tests/src/test_cases/test_arc.c \
../vendor/lvgl/tests/src/test_cases/test_bar.c \
../vendor/lvgl/tests/src/test_cases/test_checkbox.c \
../vendor/lvgl/tests/src/test_cases/test_config.c \
../vendor/lvgl/tests/src/test_cases/test_demo_stress.c \
../vendor/lvgl/tests/src/test_cases/test_demo_widgets.c \
../vendor/lvgl/tests/src/test_cases/test_dropdown.c \
../vendor/lvgl/tests/src/test_cases/test_event.c \
../vendor/lvgl/tests/src/test_cases/test_font_loader.c \
../vendor/lvgl/tests/src/test_cases/test_fs.c \
../vendor/lvgl/tests/src/test_cases/test_line.c \
../vendor/lvgl/tests/src/test_cases/test_mem.c \
../vendor/lvgl/tests/src/test_cases/test_obj_tree.c \
../vendor/lvgl/tests/src/test_cases/test_screen_load.c \
../vendor/lvgl/tests/src/test_cases/test_slider.c \
../vendor/lvgl/tests/src/test_cases/test_snapshot.c \
../vendor/lvgl/tests/src/test_cases/test_style.c \
../vendor/lvgl/tests/src/test_cases/test_switch.c \
../vendor/lvgl/tests/src/test_cases/test_table.c \
../vendor/lvgl/tests/src/test_cases/test_textarea.c \
../vendor/lvgl/tests/src/test_cases/test_tiny_ttf.c \
../vendor/lvgl/tests/src/test_cases/test_txt.c 

OBJS += \
./vendor/lvgl/tests/src/test_cases/_test_template.o \
./vendor/lvgl/tests/src/test_cases/test_arc.o \
./vendor/lvgl/tests/src/test_cases/test_bar.o \
./vendor/lvgl/tests/src/test_cases/test_checkbox.o \
./vendor/lvgl/tests/src/test_cases/test_config.o \
./vendor/lvgl/tests/src/test_cases/test_demo_stress.o \
./vendor/lvgl/tests/src/test_cases/test_demo_widgets.o \
./vendor/lvgl/tests/src/test_cases/test_dropdown.o \
./vendor/lvgl/tests/src/test_cases/test_event.o \
./vendor/lvgl/tests/src/test_cases/test_font_loader.o \
./vendor/lvgl/tests/src/test_cases/test_fs.o \
./vendor/lvgl/tests/src/test_cases/test_line.o \
./vendor/lvgl/tests/src/test_cases/test_mem.o \
./vendor/lvgl/tests/src/test_cases/test_obj_tree.o \
./vendor/lvgl/tests/src/test_cases/test_screen_load.o \
./vendor/lvgl/tests/src/test_cases/test_slider.o \
./vendor/lvgl/tests/src/test_cases/test_snapshot.o \
./vendor/lvgl/tests/src/test_cases/test_style.o \
./vendor/lvgl/tests/src/test_cases/test_switch.o \
./vendor/lvgl/tests/src/test_cases/test_table.o \
./vendor/lvgl/tests/src/test_cases/test_textarea.o \
./vendor/lvgl/tests/src/test_cases/test_tiny_ttf.o \
./vendor/lvgl/tests/src/test_cases/test_txt.o 

C_DEPS += \
./vendor/lvgl/tests/src/test_cases/_test_template.d \
./vendor/lvgl/tests/src/test_cases/test_arc.d \
./vendor/lvgl/tests/src/test_cases/test_bar.d \
./vendor/lvgl/tests/src/test_cases/test_checkbox.d \
./vendor/lvgl/tests/src/test_cases/test_config.d \
./vendor/lvgl/tests/src/test_cases/test_demo_stress.d \
./vendor/lvgl/tests/src/test_cases/test_demo_widgets.d \
./vendor/lvgl/tests/src/test_cases/test_dropdown.d \
./vendor/lvgl/tests/src/test_cases/test_event.d \
./vendor/lvgl/tests/src/test_cases/test_font_loader.d \
./vendor/lvgl/tests/src/test_cases/test_fs.d \
./vendor/lvgl/tests/src/test_cases/test_line.d \
./vendor/lvgl/tests/src/test_cases/test_mem.d \
./vendor/lvgl/tests/src/test_cases/test_obj_tree.d \
./vendor/lvgl/tests/src/test_cases/test_screen_load.d \
./vendor/lvgl/tests/src/test_cases/test_slider.d \
./vendor/lvgl/tests/src/test_cases/test_snapshot.d \
./vendor/lvgl/tests/src/test_cases/test_style.d \
./vendor/lvgl/tests/src/test_cases/test_switch.d \
./vendor/lvgl/tests/src/test_cases/test_table.d \
./vendor/lvgl/tests/src/test_cases/test_textarea.d \
./vendor/lvgl/tests/src/test_cases/test_tiny_ttf.d \
./vendor/lvgl/tests/src/test_cases/test_txt.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/tests/src/test_cases/%.o: ../vendor/lvgl/tests/src/test_cases/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


