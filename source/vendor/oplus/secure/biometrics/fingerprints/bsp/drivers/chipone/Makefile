ifeq ($(CONFIG_OPPO_FINGERPRINT_QCOM),y)
obj-y	+= fpsensor_spi_tee_qsee.o
else ifeq ($(CONFIG_OPPO_FINGERPRINT_MTK),y)
ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat
ccflags-y += -I$(srctree)/drivers/misc/mediatek/include/mt-plat/$(MTK_PLATFORM)/include
subdir-ccflags-y += -I$(srctree)/drivers/misc/mediatek/include
subdir-ccflags-y += -I$(srctree)/drivers/spi/mediatek/$(MTK_PLATFORM)

obj-y += fpsensor_spi_tee.o
endif