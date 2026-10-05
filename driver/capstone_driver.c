#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#include "../include/capstone_ioctl.h"

#define DEVICE_NAME "capstone_monitor"
#define CLASS_NAME "capstone"

static dev_t device_number;
static struct cdev capstone_cdev;
static struct class *capstone_class;

static int operation_count = 0;
static int driver_status = 1;


/* =========================
   OPEN
   ========================= */

static int capstone_open(struct inode *inode, struct file *file)
{
    operation_count++;

    pr_info("capstone_driver: device opened\n");

    return 0;
}


/* =========================
   RELEASE
   ========================= */

static int capstone_release(struct inode *inode, struct file *file)
{
    pr_info("capstone_driver: device closed\n");

    return 0;
}


/* =========================
   READ
   ========================= */

static ssize_t capstone_read(
    struct file *file,
    char __user *buffer,
    size_t length,
    loff_t *offset)
{
    char message[128];

    int size;

    size = snprintf(
        message,
        sizeof(message),
        "Capstone Driver Status: %s\nOperations: %d\n",
        driver_status ? "ACTIVE" : "INACTIVE",
        operation_count
    );

    if (*offset >= size)
        return 0;

    if (length < size)
        size = length;

    if (copy_to_user(buffer, message, size))
        return -EFAULT;

    *offset += size;

    operation_count++;

    return size;
}


/* =========================
   IOCTL
   ========================= */

static long capstone_ioctl(
    struct file *file,
    unsigned int command,
    unsigned long argument)
{
    int value;

    switch (command)
    {
        case CAPSTONE_IOCTL_GET_STATUS:

            value = driver_status;

            if (copy_to_user(
                    (int __user *)argument,
                    &value,
                    sizeof(value)))
            {
                return -EFAULT;
            }

            break;


        case CAPSTONE_IOCTL_GET_COUNT:

            value = operation_count;

            if (copy_to_user(
                    (int __user *)argument,
                    &value,
                    sizeof(value)))
            {
                return -EFAULT;
            }

            break;


        case CAPSTONE_IOCTL_RESET:

            operation_count = 0;

            pr_info(
                "capstone_driver: operation counter reset\n"
            );

            break;


        default:

            return -EINVAL;
    }

    return 0;
}


/* =========================
   FILE OPERATIONS
   ========================= */

static const struct file_operations capstone_fops =
{
    .owner = THIS_MODULE,
    .open = capstone_open,
    .release = capstone_release,
    .read = capstone_read,
    .unlocked_ioctl = capstone_ioctl
};


/* =========================
   DRIVER INITIALIZATION
   ========================= */

static int __init capstone_init(void)
{
    int result;


    result = alloc_chrdev_region(
        &device_number,
        0,
        1,
        DEVICE_NAME
    );

    if (result < 0)
    {
        pr_err(
            "capstone_driver: failed to allocate device number\n"
        );

        return result;
    }


    cdev_init(
        &capstone_cdev,
        &capstone_fops
    );


    result = cdev_add(
        &capstone_cdev,
        device_number,
        1
    );

    if (result < 0)
    {
        unregister_chrdev_region(
            device_number,
            1
        );

        return result;
    }


    capstone_class = class_create(
        CLASS_NAME
    );

    if (IS_ERR(capstone_class))
    {
        cdev_del(&capstone_cdev);

        unregister_chrdev_region(
            device_number,
            1
        );

        return PTR_ERR(capstone_class);
    }


    if (IS_ERR(
        device_create(
            capstone_class,
            NULL,
            device_number,
            NULL,
            DEVICE_NAME
        )))
    {
        class_destroy(capstone_class);

        cdev_del(&capstone_cdev);

        unregister_chrdev_region(
            device_number,
            1
        );

        return -EINVAL;
    }


    pr_info(
        "capstone_driver: loaded successfully\n"
    );

    return 0;
}


/* =========================
   DRIVER CLEANUP
   ========================= */

static void __exit capstone_exit(void)
{
    device_destroy(
        capstone_class,
        device_number
    );

    class_destroy(
        capstone_class
    );

    cdev_del(
        &capstone_cdev
    );

    unregister_chrdev_region(
        device_number,
        1
    );

    pr_info(
        "capstone_driver: unloaded\n"
    );
}


module_init(capstone_init);
module_exit(capstone_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dhruv Mohapatra");
MODULE_DESCRIPTION(
    "Linux Character Device Driver for Capstone File Explorer"
);
MODULE_VERSION("1.0");
