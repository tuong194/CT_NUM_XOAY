################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/mi_api/i2c_msc.c \
../vendor/common/mi_api/on_action_invoke.c \
../vendor/common/mi_api/on_event_occur.c \
../vendor/common/mi_api/on_property_change.c \
../vendor/common/mi_api/on_property_get.c \
../vendor/common/mi_api/on_property_request.c \
../vendor/common/mi_api/on_property_set.c \
../vendor/common/mi_api/telink_sdk_mesh_api.c \
../vendor/common/mi_api/telink_sdk_mible_api.c 

OBJS += \
./vendor/common/mi_api/i2c_msc.o \
./vendor/common/mi_api/on_action_invoke.o \
./vendor/common/mi_api/on_event_occur.o \
./vendor/common/mi_api/on_property_change.o \
./vendor/common/mi_api/on_property_get.o \
./vendor/common/mi_api/on_property_request.o \
./vendor/common/mi_api/on_property_set.o \
./vendor/common/mi_api/telink_sdk_mesh_api.o \
./vendor/common/mi_api/telink_sdk_mible_api.o 

C_DEPS += \
./vendor/common/mi_api/i2c_msc.d \
./vendor/common/mi_api/on_action_invoke.d \
./vendor/common/mi_api/on_event_occur.d \
./vendor/common/mi_api/on_property_change.d \
./vendor/common/mi_api/on_property_get.d \
./vendor/common/mi_api/on_property_request.d \
./vendor/common/mi_api/on_property_set.d \
./vendor/common/mi_api/telink_sdk_mesh_api.d \
./vendor/common/mi_api/telink_sdk_mible_api.d 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/mi_api/%.o: ../vendor/common/mi_api/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Andes C Compiler'
	$(CROSS_COMPILE)gcc -D__TLSR_RISCV_EN__=1 -DCHIP_TYPE=CHIP_TYPE_9518 -D__PROJECT_MESH__=1 -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware" -I../drivers/B91 -I../vendor/Common -I../common -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/libs" -I"/cygdrive/C/TelinkSDK_9xxx/My_Code/CT_NUM_XOAY-master/firmware/vendor/common/mi_api/mijia_ble_api" -O2 -flto -g3 -Wall -mcpu=d25f -ffunction-sections -fdata-sections -mext-dsp -c -fmessage-length=0 -fno-builtin -fomit-frame-pointer -fno-strict-aliasing -fshort-wchar -fuse-ld=bfd -fpack-struct -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d) $(@:%.o=%.o)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


