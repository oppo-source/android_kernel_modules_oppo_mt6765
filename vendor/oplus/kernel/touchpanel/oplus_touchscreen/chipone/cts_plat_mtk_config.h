#ifndef CTS_PLAT_MTK_CONFIG_H
#define CTS_PLAT_MTK_CONFIG_H

#define CFG_CTS_MAX_I2C_XFER_SIZE       (128)

#define CFG_CTS_MAX_SPI_XFER_SIZE           (1400u)

#define CTS_FW_LOG_REDIRECT_SIGN            0x60
#define CTS_FW_LOG_BUF_LEN                  128

/* Swap X and Y cordinate */
//#define CFG_CTS_SWAP_XY

#ifdef CONFIG_MTK_LCM_PHYSICAL_ROTATION_HW
#ifdef CONFIG_TOUCHSCREEN_PHYSICAL_ROTATION_WITH_LCM
//#define CFG_CTS_WRAP_X
//#define CFG_CTS_WRAP_Y
#else   /* CONFIG_TOUCHSCREEN_PHYSICAL_ROTATION_WITH_LCM */
//#define CFG_CTS_WRAP_X
//#define CFG_CTS_WRAP_Y
#endif  /* CONFIG_TOUCHSCREEN_PHYSICAL_ROTATION_WITH_LCM */
#else   /* CONFIG_MTK_LCM_PHYSICAL_ROTATION_HW */
#ifdef CONFIG_TOUCHSCREEN_PHYSICAL_ROTATION_WITH_LCM
//#define CFG_CTS_WRAP_X
//#define CFG_CTS_WRAP_Y
#else   /* CONFIG_TOUCHSCREEN_PHYSICAL_ROTATION_WITH_LCM */
//#define CFG_CTS_WRAP_X
//#define CFG_CTS_WRAP_Y
#endif  /* CONFIG_TOUCHSCREEN_PHYSICAL_ROTATION_WITH_LCM */
#endif  /* CONFIG_MTK_LCM_PHYSICAL_ROTATION_HW */

#define CFG_CTS_DEVICE_NAME         "chipone-tddi"
#define CFG_CTS_DRIVER_NAME         "chipone-tddi"

#ifdef CONFIG_OF
#define CONFIG_CTS_OF
#endif
#ifdef CONFIG_CTS_OF
#define CFG_CTS_OF_DEVICE_ID_NAME		"chipone-tddi"
#define CFG_CTS_OF_INT_GPIO_NAME		"chipone,irq-gpio"
#define CFG_CTS_OF_RST_GPIO_NAME		"chipone,rst-gpio"
#endif /* CONFIG_CTS_OF */



#ifdef CONFIG_CTS_OF
#ifdef CFG_CTS_MANUAL_CS
#define CFG_CTS_OF_CS_GPIO_NAME``   `   "chipone,cs-gpio"
#endif
#endif

#if CFG_CTS_MAX_I2C_XFER_SIZE < 8
#error "I2C transfer size should large than 8"
#endif

#endif /* CTS_PLAT_MTK_CONFIG_H */

