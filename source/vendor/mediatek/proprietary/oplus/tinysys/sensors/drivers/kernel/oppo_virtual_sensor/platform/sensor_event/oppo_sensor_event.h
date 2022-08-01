/*
 * Copyright (C) 2016 MediaTek Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See http://www.gnu.org/licenses/gpl-2.0.html for more details.
 */

#ifndef _OPPO_SENSOR_EVENT_H
#define _OPPO_SENSOR_EVENT_H
#include <linux/poll.h>
#include <linux/ratelimit.h>

#define CONTINUE_SENSOR_BUF_SIZE    2048
#define BIO_SENSOR_BUF_SIZE    0x20000 /* ((512 + 1024 + 1024) * 60) */
#define OTHER_SENSOR_BUF_SIZE    1024
#define MINOR_NUM_FOR_VIRTUAL_SENSOR 0
#define MINOR_NUM_MAX 0

enum flushAction {
    DATA_ACTION,
    FLUSH_ACTION,
    BIAS_ACTION,
    CALI_ACTION,
    TEMP_ACTION,
    TEST_ACTION,
};
struct oppo_sensor_event {
    int64_t time_stamp;
    int8_t handle;
    int8_t flush_action;
    int8_t status;
    int8_t reserved;
    union {
        int32_t word[6];
        int8_t byte[0];
    };
} __packed;
ssize_t oppo_sensor_event_read(unsigned char handle, struct file *file,
    char __user *buffer,
              size_t count, loff_t *ppos);
unsigned int oppo_sensor_event_poll(unsigned char handle, struct file *file,
    poll_table *wait);
int oppo_sensor_input_event(unsigned char handle,
             const struct oppo_sensor_event *event);
unsigned int oppo_sensor_event_register(unsigned char handle);
unsigned int oppo_sensor_event_deregister(unsigned char handle);
#endif
