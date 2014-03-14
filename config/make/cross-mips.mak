# Base directory of OpenWrt
OPENWRT_DIR = $(TOPDIR)/carambola

# Staging dir from OpenWrt build
STAGING_DIR = $(OPENWRT_DIR)/staging_dir
TARGET_DIR = $(STAGING_DIR)/target-mipsel_r2_uClibc-0.9.33.2

# mips-gcc PATH
CROSS_DIR = $(STAGING_DIR)/toolchain-mipsel_r2_gcc-4.7-linaro_uClibc-0.9.33.2/bin/

# Set arch to MIPS
ARCH = mips

# Set corss-compile prefix to mips-gcc
CROSS_COMPILE = $(CROSS_DIR)mipsel-openwrt-linux-
