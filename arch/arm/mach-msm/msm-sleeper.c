/*
 * ElementalX msm-sleeper by flar2 <asegaert@gmail.com>
 * 
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/earlysuspend.h>
#include <linux/workqueue.h>
#include <linux/cpu.h>
#include <linux/module.h>
#include <linux/cpufreq.h>
#include <mach/cpufreq.h>

#define MSM_SLEEPER_MAJOR_VERSION	1
#define MSM_SLEEPER_MINOR_VERSION	2

extern uint32_t maxscroff;
extern uint32_t maxscroff_freq;
extern uint32_t ex_max_freq;
static int limit_set = 0;

#ifdef CONFIG_HAS_EARLYSUSPEND
static void msm_sleeper_early_suspend(struct early_suspend *h)
{
	int cpu;

	if (maxscroff) {
		for_each_possible_cpu(cpu) {
			msm_cpufreq_set_freq_limits(cpu, MSM_CPUFREQ_NO_LIMIT, maxscroff_freq);
			pr_info("msm-sleeper: limit max frequency to: %d\n", maxscroff_freq);
		}
		limit_set = 1;
	}
	return; 
}

static void msm_sleeper_late_resume(struct early_suspend *h)
{
	int cpu;

	if (!limit_set)
		return;

	for_each_possible_cpu(cpu) {
		msm_cpufreq_set_freq_limits(cpu, MSM_CPUFREQ_NO_LIMIT, ex_max_freq);
		pr_info("msm-sleeper: restore max frequency.\n");
	}
	limit_set = 0;
	return; 
}

static struct early_suspend msm_sleeper_early_suspend_driver = {
	.level = EARLY_SUSPEND_LEVEL_BLANK_SCREEN + 10,
	.suspend = msm_sleeper_early_suspend,
	.resume = msm_sleeper_late_resume,
};
#endif

static int __init msm_sleeper_init(void)
{
	pr_info("msm-sleeper version %d.%d\n",
		 MSM_SLEEPER_MAJOR_VERSION,
		 MSM_SLEEPER_MINOR_VERSION);

#ifdef CONFIG_HAS_EARLYSUSPEND
	register_early_suspend(&msm_sleeper_early_suspend_driver);
#endif
	return 0;
}

MODULE_AUTHOR("flar2 <asegaert at gmail.com>");
MODULE_DESCRIPTION("'msm-sleeper' - Limit max frequency while screen is off");
MODULE_LICENSE("GPL");

late_initcall(msm_sleeper_init);

