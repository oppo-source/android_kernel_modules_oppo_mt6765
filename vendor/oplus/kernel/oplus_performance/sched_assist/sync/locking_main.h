/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2020-2022 Oplus. All rights reserved.
 */


#ifndef _OPLUS_LOCKING_MAIN_H_
#define _OPLUS_LOCKING_MAIN_H_

#include <linux/futex.h>

#define cond_trace_printk(cond, fmt, ...)	\
do {										\
	if (cond)								\
		trace_printk(fmt, ##__VA_ARGS__);	\
} while (0)

#define LK_MUTEX_ENABLE (1 << 0)
#define LK_RWSEM_ENABLE (1 << 1)
#define LK_FUTEX_ENABLE (1 << 2)

#define LK_DEBUG_TRACE  (1 << 0)
#define LK_DEBUG_FTRACE (1 << 1)

extern unsigned int g_opt_enable;
extern unsigned int g_opt_debug;

extern atomic64_t futex_inherit_set_times;
extern atomic64_t futex_inherit_unset_times;
extern atomic64_t futex_inherit_useless_times;
extern atomic64_t futex_low_count;
extern atomic64_t futex_high_count;

static inline bool locking_opt_enable(unsigned int enable)
{
	return g_opt_enable & enable;
}

static inline bool locking_opt_debug(int debug)
{
	return g_opt_debug & debug;
}

void locking_vh_do_futex_handler(void *unused, int cmd,
		  unsigned int *flags, u32 __user *uaddr2);
void locking_vh_futex_wait_start_handler(void *unused, unsigned int flags,
		  u32 bitset);
void locking_vh_futex_wait_end_handler(void *unused, unsigned int flags,
		  u32 bitset);
void locking_vh_alter_futex_plist_add_handler(void *unused, struct plist_node *node,
		  struct plist_head *head, bool *already_on_hb);
void locking_vh_futex_sleep_start_handler(void *unused,
		  struct task_struct *p);
void locking_vh_futex_wake_traverse_plist_handler(void *unused, struct plist_head *chain,
		  int *target_nr, union futex_key key, u32 bitset);
void locking_vh_futex_wake_this_handler(void *unused, int ret, int nr_wake, int target_nr,
		  struct task_struct *p);
void locking_vh_futex_wake_up_q_finish_handler(void *unused, int nr_wake,
		  int target_nr);

void lk_sysfs_init(void);
void lk_sysfs_exit(void);
#endif /* _OPLUS_LOCKING_MAIN_H_ */
