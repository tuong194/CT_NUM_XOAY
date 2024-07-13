################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_arc.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_bg.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_composite.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_img.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_label.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_layer.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_line.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_mask.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_polygon.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_rect.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.c \
../vendor/lvgl/src/draw/sdl/lv_draw_sdl_utils.c 

OBJS += \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_arc.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_bg.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_composite.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_img.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_label.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_layer.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_line.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_mask.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_polygon.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_rect.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.o \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_utils.o 

C_DEPS += \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_arc.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_bg.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_composite.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_img.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_label.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_layer.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_line.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_mask.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_polygon.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_rect.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_stack_blur.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_texture_cache.d \
./vendor/lvgl/src/draw/sdl/lv_draw_sdl_utils.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/src/draw/sdl/%.o: ../vendor/lvgl/src/draw/sdl/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


