// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2020 Oplus. All rights reserved.
 */
#include <linux/sched.h>
#include <linux/sysctl.h>
#include <linux/topology.h>
#include <../kernel/sched/sched.h>

#include <linux/version.h>
#include "../uifirst/uifirst_sched_common.h"
#include "sched_assist_common.h"
#include "sched_assist_slide.h"

#define UX_LOAD_WINDOW 8000000
u64 ux_task_load[NR_CPUS] = {0};
u64 ux_load_ts[NR_CPUS] = {0};

int sysctl_slide_boost_enabled = 0;
int sysctl_boost_task_threshold = 51;
int sysctl_frame_rate = 60;

bool slide_boost_entrance(struct task_struct *p, int cpu)
{
	return (sysctl_uifirst_enabled && (sched_assist_scene(SA_SLIDE) || sched_assist_scene(SA_INPUT) || sysctl_animation_type == LAUNCHER_SI_START) &&
	(is_heavy_ux_task(p) || test_dynamic_ux(p, DYNAMIC_UX_BINDER)) && oplus_task_misfit(p, cpu));
}

u64 calc_freq_ux_load(struct task_struct *p, u64 wallclock)
{
	unsigned int maxtime = 0, factor = 0;
	unsigned int window_size = sched_ravg_window / NSEC_PER_MSEC;
	u64 timeline = 0, freq_exec_load = 0, freq_ravg_load = 0;
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 4, 0)
	u64 wakeclock = p->wts.last_wake_ts;
#else
	u64 wakeclock = p->last_wake_ts;
#endif
	if (wallclock < wakeclock)
		return 0;

	if (sysctl_frame_rate <= 90)
		maxtime = 5;
	else if (sysctl_frame_rate <= 120)
		maxtime = 4;
	else
		maxtime = 3;

	timeline = wallclock - wakeclock;
	factor = window_size / maxtime;
	freq_exec_load = timeline * factor;

	if (freq_exec_load > sched_ravg_window)
		freq_exec_load = sched_ravg_window;
#if LINUX_VERSION_CODE > KERNEL_VERSION(5, 4, 0)
	freq_ravg_load = (p->wts.prev_window + p->wts.curr_window) << 1;
#else
	freq_ravg_load = (p->ravg.prev_window + p->ravg.curr_window) << 1;
#endif
	if (freq_ravg_load > sched_ravg_window)
		freq_ravg_load = sched_ravg_window;

	return max(freq_exec_load, freq_ravg_load);
}

void _slide_find_start_cpu(struct root_domain *rd, struct task_struct *p, int *start_cpu)
{
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
	if (task_util(p) >= sysctl_boost_task_threshold ||
	scale_demand(p->wts.sum) >= sysctl_boost_task_threshold) {
	*start_cpu = rd->wrd.mid_cap_orig_cpu == -1 ?
	rd->wrd.max_cap_orig_cpu : rd->wrd.mid_cap_orig_cpu;
	}
#else
	if (task_util(p) >= sysctl_boost_task_threshold ||
	scale_demand(p->ravg.sum) >= sysctl_boost_task_threshold) {
	*start_cpu = rd->mid_cap_orig_cpu == -1 ?
	rd->max_cap_orig_cpu : rd->mid_cap_orig_cpu;
	 }
#endif
}

bool _slide_task_misfit(struct task_struct *p, int cpu)
{
int num_mincpu = cpumask_weight(topology_core_cpumask(0));
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(5, 4, 0))
	if ((scale_demand(p->wts.sum) >= sysctl_boost_task_threshold ||
	     task_util(p) >= sysctl_boost_task_threshold) && cpu < num_mincpu)
#else
	if ((scale_demand(p->ravg.sum) >= sysctl_boost_task_threshold ||
		task_util(p) >= sysctl_boost_task_threshold) && cpu < num_mincpu)
#endif
		return true;

	return false;
}

u64 _slide_get_boost_load(int cpu) {
	u64 wallclock = sched_ktime_clock();
	u64 timeline = 0;
	u64 ret_load = 0;

	if ((sched_assist_scene(SA_SLIDE) || sched_assist_scene(SA_INPUT) ||
		sysctl_animation_type == LAUNCHER_SI_START) && ux_task_load[cpu]) {
		timeline = wallclock - ux_load_ts[cpu];
		if  (timeline >= UX_LOAD_WINDOW)
			ux_task_load[cpu] = 0;
		ret_load = ux_task_load[cpu];
	}
	sched_assist_systrace(ret_load, "ux_load_%d", cpu);
	return ret_load;
}

void slide_calc_boost_load(struct rq *rq, unsigned int *flag, int cpu) {
	u64 wallclock = sched_ktime_clock();
	adjust_sched_assist_input_ctrl();
	if (sched_assist_scene(SA_SLIDE) || sched_assist_scene(SA_INPUT) || sysctl_animation_type == LAUNCHER_SI_START) {
		if(rq->curr && (is_heavy_ux_task(rq->curr) || rq->curr->sched_class == &rt_sched_class) && !oplus_task_misfit(rq->curr, rq->cpu)) {
			ux_task_load[cpu] = calc_freq_ux_load(rq->curr, wallclock);
			ux_load_ts[cpu] = wallclock;
			*flag |= (SCHED_CPUFREQ_WALT | SCHED_CPUFREQ_BOOST);
		}
		else if (ux_task_load[cpu] != 0) {
			ux_task_load[cpu] = 0;
			ux_load_ts[cpu] = wallclock;
			*flag |= (SCHED_CPUFREQ_WALT | SCHED_CPUFREQ_RESET);
		}
	} else {
		ux_task_load[cpu] = 0;
		ux_load_ts[cpu] = 0;
	}
}

int sched_frame_rate_handler(struct ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;
	if (write && *ppos)
		*ppos = 0;
	ret = proc_dointvec(table, write, buffer, lenp, ppos);

	if(write)
		sched_assist_systrace(sysctl_frame_rate, "ux frate");
	return ret;
}

/* Put it here, because the compilation error*/
void adjust_sched_assist_input_ctrl_v2(void)
{
	if (!sysctl_input_boost_enabled)
		return;
	if(sysctl_slide_boost_enabled){
		sysctl_input_boost_enabled = 0;
		sched_assist_input_boost_duration = 0;
		return;
	}
	if(jiffies_to_msecs(jiffies) < sched_assist_input_boost_duration) {
		return;
	}else{
		sched_assist_input_boost_duration = 0;
		sysctl_input_boost_enabled = 0;
	}
}

void adjust_sched_assist_input_ctrl()
{
	if (!sysctl_input_boost_enabled)
		return;
	if(sysctl_slide_boost_enabled) {
		sysctl_input_boost_enabled = 0;
		sched_assist_input_boost_duration = 0;
		return;
	}
	if(jiffies_to_msecs(jiffies) < sched_assist_input_boost_duration) {
		return;
	}
	else {
		sched_assist_input_boost_duration = 0;
		sysctl_input_boost_enabled = 0;
	}
}

int sysctl_sched_animation_type_handler(struct ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;
	if (write && *ppos)
		*ppos = 0;
	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write)
		sched_assist_systrace(sysctl_animation_type, "ux anima");
	return ret;
}

int sysctl_sched_boost_task_threshold_handler(struct ctl_table *table, int write, void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;
	if (write && *ppos)
		*ppos = 0;
	ret = proc_dointvec(table, write, buffer, lenp, ppos);
	if (write)
		sched_assist_systrace(sysctl_boost_task_threshold, "ux threshold");
	return ret;
}

