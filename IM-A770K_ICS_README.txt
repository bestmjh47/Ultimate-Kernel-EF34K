How to Build
    1. Get Toolchain (arm-eabi-4.4.3) and install
	(You can get arm-eabi-4.4.3 from many repos or Android source.)
    2. run build_kernel.sh
		
      $ export ARCH=arm
      $ export CROSS_COMPILE=~/your toolchain path/arm-eabi-4.4.3/bin/arm-eabi-
      $ ./build_kernel.sh

    3.Output Files
      -	kernel : IM-A760S_ICS_Kernel/obj/KERNEL_OBJ/arch/arm/boot/zImage
      -	module : IM-A770K_ICS_Kernel/obj/KERNEL_OBJ/drivers/*/*.ko
