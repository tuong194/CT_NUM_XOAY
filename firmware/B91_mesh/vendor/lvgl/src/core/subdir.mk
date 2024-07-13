################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/src/core/lv_disp.c \
../vendor/lvgl/src/core/lv_event.c \
../vendor/lvgl/src/core/lv_group.c \
../vendor/lvgl/src/core/lv_indev.c \
../vendor/lvgl/src/core/lv_indev_scroll.c \
../vendor/lvgl/src/core/lv_obj.c \
../vendor/lvgl/src/core/lv_obj_class.c \
../vendor/lvgl/src/core/lv_obj_draw.c \
../vendor/lvgl/src/core/lv_obj_pos.c \
../vendor/lvgl/src/core/lv_obj_scroll.c \
../vendor/lvgl/src/core/lv_obj_style.c \
../vendor/lvgl/src/core/lv_obj_style_gen.c \
../vendor/lvgl/src/core/lv_obj_tree.c \
../vendor/lvgl/src/core/lv_refr.c \
../vendor/lvgl/src/core/lv_theme.c 

OBJS += \
./vendor/lvgl/src/core/lv_disp.o \
./vendor/lvgl/src/core/lv_event.o \
./vendor/lvgl/src/core/lv_group.o \
./vendor/lvgl/src/core/lv_indev.o \
./vendor/lvgl/src/core/lv_indev_scroll.o \
./vendor/lvgl/src/core/lv_obj.o \
./vendor/lvgl/src/core/lv_obj_class.o \
./vendor/lvgl/src/core/lv_obj_draw.o \
./vendor/lvgl/src/core/lv_obj_pos.o \
./vendor/lvgl/src/core/lv_obj_scroll.o \
./vendor/lvgl/src/core/lv_obj_style.o \
./vendor/lvgl/src/core/lv_obj_style_gen.o \
./vendor/lvgl/src/core/lv_obj_tree.o \
./vendor/lvgl/src/core/lv_refr.o \
./vendor/lvgl/src/core/lv_theme.o 

C_DEPS += \
./vendor/lvgl/src/core/lv_disp.d \
./vendor/lvgl/src/core/lv_event.d \
./vendor/lvgl/src/core/lv_group.d \
./vendor/lvgl/src/core/lv_indev.d \
./vendor/lvgl/src/core/lv_indev_scroll.d \
./vendor/lvgl/src/core/lv_obj.d \
./vendor/lvgl/src/core/lv_obj_class.d \
./vendor/lvgl/src/core/lv_obj_draw.d \
./vendor/lvgl/src/core/lv_obj_pos.d \
./vendor/lvgl/src/core/lv_obj_scroll.d \
./vendor/lvgl/src/core/lv_obj_style.d \
./vendor/lvgl/src/core/lv_obj_style_gen.d \
./vendor/lvgl/src/core/lv_obj_tree.d \
./vendor/lvgl/src/core/lv_refr.d \
./vendor/lvgl/src/core/lv_theme.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/src/core/%.o: ../vendor/lvgl/src/core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


