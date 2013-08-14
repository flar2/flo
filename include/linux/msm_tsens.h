/*
 * Copyright (c) 2011-2012, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */
/*
 * Qualcomm TSENS Header file
 *
 */

#ifndef __MSM_TSENS_H
#define __MSM_TSENS_H

enum platform_type {
	MSM_8660 = 0,
	MSM_8960,
	MDM_9615,
	APQ_8064,
	MSM_TYPE
};

#define TSENS_MAX_SENSORS		11

struct tsens_platform_data {
	int				slope[TSENS_MAX_SENSORS];
	int				tsens_factor;
	uint32_t			tsens_num_sensor;
	enum platform_type		hw_type;
};

struct tsens_device {
	uint32_t			sensor_num;
};

int tsens_set_tz_warm_temp_degC(int sensor_num, int temp, struct work_struct *work);
int tsens_set_tz_cool_temp_degC(int sensor_num, int temp, struct work_struct *work); 
int32_t tsens_get_temp(struct tsens_device *dev, unsigned long *temp);
int msm_tsens_early_init(struct tsens_platform_data *pdata);

#endif /*MSM_TSENS_H */
