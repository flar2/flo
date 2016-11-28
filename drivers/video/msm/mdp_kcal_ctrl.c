/*
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/module.h>

#include "msm_fb.h"

static ssize_t kcal_store(struct device *dev, struct device_attribute *attr,
						const char *buf, size_t count)
{
	int kcal_r, kcal_g, kcal_b;
	struct mdp_pcc_info *pcc_info = dev_get_drvdata(dev);

	if (count > 12)
		return -EINVAL;

	sscanf(buf, "%d %d %d", &kcal_r, &kcal_g, &kcal_b);

	if (kcal_r < 0 || kcal_r > 256)
		return -EINVAL;

	if (kcal_g < 0 || kcal_g > 256)
		return -EINVAL;

	if (kcal_b < 0 || kcal_b > 256)
		return -EINVAL;

	pcc_info->red = kcal_r;
	pcc_info->green = kcal_g;
	pcc_info->blue = kcal_b;

	mdp_set_kcal(pcc_info);

	return count;
}

static ssize_t kcal_show(struct device *dev, struct device_attribute *attr,
								char *buf)
{
	struct mdp_pcc_info pcc_info = mdp_get_kcal();

	return sprintf(buf, "%d %d %d\n", pcc_info.red, pcc_info.green,
		pcc_info.blue);
}

static DEVICE_ATTR(kcal, 0644, kcal_show, kcal_store);

static int kcal_ctrl_probe(struct platform_device *pdev)
{
	int ret;
	struct mdp_pcc_info *pcc_info;

	pcc_info = kzalloc(sizeof(*pcc_info), GFP_KERNEL);
	if (!pcc_info) {
		pr_err("%s: failed to allocate memory for pcc_info\n",
			__func__);
		return -ENOMEM;
	}

	platform_set_drvdata(pdev, pcc_info);

	ret = device_create_file(&pdev->dev, &dev_attr_kcal);
	if (ret)
		pr_err("%s: unable to create sysfs entries\n", __func__);

	return ret;
}

static int kcal_ctrl_remove(struct platform_device *pdev)
{
	struct mdp_pcc_info *pcc_info = platform_get_drvdata(pdev);

	device_remove_file(&pdev->dev, &dev_attr_kcal);
	kfree(pcc_info);

	return 0;
}

static struct platform_driver kcal_ctrl_driver = {
	.probe = kcal_ctrl_probe,
	.remove = kcal_ctrl_remove,
	.driver = {
		.name = "kcal_ctrl",
	},
};

static struct platform_device kcal_ctrl_device = {
	.name = "kcal_ctrl",
};

static int __init kcal_ctrl_init(void)
{
	if (platform_driver_register(&kcal_ctrl_driver))
		return -ENODEV;

	if (platform_device_register(&kcal_ctrl_device))
		return -ENODEV;

	pr_info("%s: registered\n", __func__);

	return 0;
}

static void __exit kcal_ctrl_exit(void)
{
	platform_device_unregister(&kcal_ctrl_device);
	platform_driver_unregister(&kcal_ctrl_driver);
}

late_initcall(kcal_ctrl_init);
module_exit(kcal_ctrl_exit);

MODULE_DESCRIPTION("MDSS KCAL PCC Interface Driver");
