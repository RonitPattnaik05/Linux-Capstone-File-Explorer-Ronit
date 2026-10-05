#ifndef CAPSTONE_IOCTL_H
#define CAPSTONE_IOCTL_H

#include <linux/ioctl.h>

#define CAPSTONE_MAGIC 'C'

#define CAPSTONE_IOCTL_GET_STATUS \
    _IOR(CAPSTONE_MAGIC, 1, int)

#define CAPSTONE_IOCTL_GET_COUNT \
    _IOR(CAPSTONE_MAGIC, 2, int)

#define CAPSTONE_IOCTL_RESET \
    _IO(CAPSTONE_MAGIC, 3)

#endif
