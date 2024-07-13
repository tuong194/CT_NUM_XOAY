################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/examples/assets/animimg001.c \
../vendor/lvgl/examples/assets/animimg002.c \
../vendor/lvgl/examples/assets/animimg003.c \
../vendor/lvgl/examples/assets/img_caret_down.c \
../vendor/lvgl/examples/assets/img_cogwheel_alpha16.c \
../vendor/lvgl/examples/assets/img_cogwheel_argb.c \
../vendor/lvgl/examples/assets/img_cogwheel_chroma_keyed.c \
../vendor/lvgl/examples/assets/img_cogwheel_indexed16.c \
../vendor/lvgl/examples/assets/img_cogwheel_rgb.c \
../vendor/lvgl/examples/assets/img_hand.c \
../vendor/lvgl/examples/assets/img_skew_strip.c \
../vendor/lvgl/examples/assets/img_star.c \
../vendor/lvgl/examples/assets/imgbtn_left.c \
../vendor/lvgl/examples/assets/imgbtn_mid.c \
../vendor/lvgl/examples/assets/imgbtn_right.c 

OBJS += \
./vendor/lvgl/examples/assets/animimg001.o \
./vendor/lvgl/examples/assets/animimg002.o \
./vendor/lvgl/examples/assets/animimg003.o \
./vendor/lvgl/examples/assets/img_caret_down.o \
./vendor/lvgl/examples/assets/img_cogwheel_alpha16.o \
./vendor/lvgl/examples/assets/img_cogwheel_argb.o \
./vendor/lvgl/examples/assets/img_cogwheel_chroma_keyed.o \
./vendor/lvgl/examples/assets/img_cogwheel_indexed16.o \
./vendor/lvgl/examples/assets/img_cogwheel_rgb.o \
./vendor/lvgl/examples/assets/img_hand.o \
./vendor/lvgl/examples/assets/img_skew_strip.o \
./vendor/lvgl/examples/assets/img_star.o \
./vendor/lvgl/examples/assets/imgbtn_left.o \
./vendor/lvgl/examples/assets/imgbtn_mid.o \
./vendor/lvgl/examples/assets/imgbtn_right.o 

C_DEPS += \
./vendor/lvgl/examples/assets/animimg001.d \
./vendor/lvgl/examples/assets/animimg002.d \
./vendor/lvgl/examples/assets/animimg003.d \
./vendor/lvgl/examples/assets/img_caret_down.d \
./vendor/lvgl/examples/assets/img_cogwheel_alpha16.d \
./vendor/lvgl/examples/assets/img_cogwheel_argb.d \
./vendor/lvgl/examples/assets/img_cogwheel_chroma_keyed.d \
./vendor/lvgl/examples/assets/img_cogwheel_indexed16.d \
./vendor/lvgl/examples/assets/img_cogwheel_rgb.d \
./vendor/lvgl/examples/assets/img_hand.d \
./vendor/lvgl/examples/assets/img_skew_strip.d \
./vendor/lvgl/examples/assets/img_star.d \
./vendor/lvgl/examples/assets/imgbtn_left.d \
./vendor/lvgl/examples/assets/imgbtn_mid.d \
./vendor/lvgl/examples/assets/imgbtn_right.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/examples/assets/%.o: ../vendor/lvgl/examples/assets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


