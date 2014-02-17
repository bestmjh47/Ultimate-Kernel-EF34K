#!/bin/bash
###############################################################################
#
#                           Kernel Build Script 
#
###############################################################################
# 2011-10-24 effectivesky : modified
# 2010-12-29 allydrop     : created
###############################################################################
##############################################################################
# set toolchain
##############################################################################
export ARCH=arm
#export CROSS_COMPILE=/home/moon/linaro-4.7.4/bin/arm-cortex_a8-linux-gnueabi-
#export CROSS_COMPILE=/home/moon/linaro-4.8.1/bin/arm-cortex_a8-linux-gnueabi-
export CROSS_COMPILE=/home/moon/arm-linux-androideabi-4.7/bin/arm-linux-androideabi-
##############################################################################
# make zImage
##############################################################################
mkdir -p ./obj/KERNEL_OBJ/
make O=./obj/KERNEL_OBJ/ msm8660-perf-EF34K_defconfig
make -j4 O=./obj/KERNEL_OBJ/

##############################################################################
# Copy Kernel Image
##############################################################################
cp -f ./obj/KERNEL_OBJ/arch/arm/boot/zImage .


