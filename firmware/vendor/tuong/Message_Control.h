/*
 * Message_Control.h
 *
 *  Created on: May 30, 2024
 *      Author: DELL
 */

#ifndef VENDOR_TUONG_MESSAGE_CONTROL_H_
#define VENDOR_TUONG_MESSAGE_CONTROL_H_

#include "tl_common.h"
#include "proj/mcu/watchdog_i.h"
#include "vendor/common/user_config.h"
#include "drivers.h"
#include "proj_lib/ble/blt_config.h"
#include "proj_lib/ble/ll/ll.h"
#include "proj_lib/sig_mesh/app_mesh.h"

#define OPCODE1 (0xE2)

#define HEADER_SET_START_LED 0x0B10

#define OFF_ALL_LED  0x00
#define ON_ALL_LED   0x01
#define SAVE_STT_LED 0x02



int a(u8 *par, int par_len, mesh_cb_fun_par_t *cb_par);

#endif /* VENDOR_TUONG_MESSAGE_CONTROL_H_ */
