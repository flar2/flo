// Copyright (C) 2013, 2014, 2015 Timur Mehrvarz

#include <linux/kobject.h>
#include <linux/sysfs.h>

extern void smb345_event_fastcharge(void);

/* ----------------------------------------- */
int usbhost_fixed_install_mode;

static ssize_t fixed_install_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", usbhost_fixed_install_mode);
}

static ssize_t fixed_install_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_fixed_install_mode);
    return count;
}

static struct kobj_attribute fixed_install_mode_attribute = 
    __ATTR(usbhost_fixed_install_mode, 0666, fixed_install_mode_show, fixed_install_mode_store);

/* ----------------------------------------- */
int usbhost_fastcharge_in_host_mode;

static ssize_t fastcharge_in_host_mode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", usbhost_fastcharge_in_host_mode);
}

static ssize_t fastcharge_in_host_mode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_fastcharge_in_host_mode);
    printk("usbhost %s usbhost_fastcharge_in_host_mode=%d\n", __func__,usbhost_fastcharge_in_host_mode);
    smb345_event_fastcharge();

    return count;
}

static struct kobj_attribute fastcharge_in_host_mode_attribute = 
    __ATTR(usbhost_fastcharge_in_host_mode, 0666, fastcharge_in_host_mode_show, fastcharge_in_host_mode_store);

/* ----------------------------------------- */
int usbhost_hostmode;

static ssize_t hostmode_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", usbhost_hostmode);
}

static ssize_t hostmode_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_hostmode);
    printk("usbhost %s usbhost_hostmode=%d\n", __func__,usbhost_hostmode);
    return count;
}

static struct kobj_attribute hostmode_attribute = 
    __ATTR(usbhost_hostmode, 0666, hostmode_show, hostmode_store);

/* ----------------------------------------- */
volatile int usbhost_charging_state;

static ssize_t charging_state_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", usbhost_charging_state);
}

static ssize_t charging_state_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_charging_state);
    return count;
}

static struct kobj_attribute charging_state_attribute = 
    __ATTR(usbhost_charging_state, 0666, charging_state_show, charging_state_store);

/* ----------------------------------------- */
volatile int usbhost_external_power;

static ssize_t external_power_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", usbhost_external_power);
}

static ssize_t external_power_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_external_power);
    return count;
}

static struct kobj_attribute external_power_attribute = 
    __ATTR(usbhost_external_power, 0666, external_power_show, external_power_store);

/* ----------------------------------------- */
int usbhost_charge_slave_devices;

static ssize_t charge_slave_devices_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    return sprintf(buf, "%d\n", usbhost_charge_slave_devices);
}

static ssize_t charge_slave_devices_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_charge_slave_devices);
    return count;
}

static struct kobj_attribute charge_slave_devices_attribute = 
    __ATTR(usbhost_charge_slave_devices, 0666, charge_slave_devices_show, charge_slave_devices_store);

/* ----------------------------------------- */
int  usbhost_power_slaves = 1;

static ssize_t power_slaves_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    printk("usbhost %s power_slaves=%d\n", __func__,usbhost_power_slaves);
    return sprintf(buf, "%d\n", usbhost_power_slaves);
}

static ssize_t power_slaves_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    sscanf(buf, "%d", &usbhost_power_slaves);
    printk("usbhost %s power_slaves=%d\n", __func__,usbhost_power_slaves);
    return count;
}

static struct kobj_attribute power_slaves_attribute = 
    __ATTR(usbhost_power_slaves, 0666, power_slaves_show, power_slaves_store);

/* ----------------------------------------- */
static struct attribute *attrs[] = {
    &fixed_install_mode_attribute.attr,
    &fastcharge_in_host_mode_attribute.attr,
    &hostmode_attribute.attr,
    &charging_state_attribute.attr,
    &external_power_attribute.attr,
    &charge_slave_devices_attribute.attr,
    &power_slaves_attribute.attr,,
    NULL,
};

static struct attribute_group attr_group = {
    .attrs = attrs,
};

static struct kobject *usbhost_kobj;

int usbhost_init(void)
{
	int retval;

	// default values
	usbhost_fixed_install_mode = 1;
	usbhost_fastcharge_in_host_mode = 1;
    usbhost_charging_state = 0;
    usbhost_external_power = 0;
    usbhost_charge_slave_devices = 0;
    usbhost_power_slaves = 0;

    printk("usbhost %s startup with FI=%d FC=%d\n", __func__, usbhost_fixed_install_mode, usbhost_fastcharge_in_host_mode);

    usbhost_kobj = kobject_create_and_add("usbhost", kernel_kobj);
    if (!usbhost_kobj) {
            return -ENOMEM;
    }
    retval = sysfs_create_group(usbhost_kobj, &attr_group);
    if (retval)
            kobject_put(usbhost_kobj);
    return retval;
}

void usbhost_exit(void)
{
	kobject_put(usbhost_kobj);
}

module_init(usbhost_init);
module_exit(usbhost_exit);

