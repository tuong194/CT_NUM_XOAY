################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.c \
../vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.c \
../vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.c \
../vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.c \
../vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.c \
../vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.c \
../vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.c \
../vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.c \
../vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.c 

OBJS += \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.o \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.o \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.o \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.o \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.o \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.o \
./vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.o \
./vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.o \
./vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.o 

C_DEPS += \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_alpha16.d \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_argb.d \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_chroma_keyed.d \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_indexed16.d \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb.d \
./vendor/lvgl/demos/benchmark/assets/img_benchmark_cogwheel_rgb565a8.d \
./vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_12_compr_az.c.d \
./vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_16_compr_az.c.d \
./vendor/lvgl/demos/benchmark/assets/lv_font_bechmark_montserrat_28_compr_az.c.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/lvgl/demos/benchmark/assets/%.o: ../vendor/lvgl/demos/benchmark/assets/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


