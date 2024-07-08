/*
 * switch.h
 *
 *  Created on: May 7, 2024
 *      Author: DELL
 */

#ifndef VENDOR_TUONG_SWITCH_H_
#define VENDOR_TUONG_SWITCH_H_

#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#define SW1 GPIO_PB0
#define SW2 GPIO_PB1
#define SW3 GPIO_PB5
#define SW4 GPIO_PD0

#define SW_NOT_PRESS '\0'

void Pin_SW_Conf(void);
char ReadSW(void);

#endif /* VENDOR_TUONG_SWITCH_H_ */
