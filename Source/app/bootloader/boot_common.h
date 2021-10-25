/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __BOOT_COMMON_H__
#define __BOOT_COMMON_H__

/* Includes ------------------------------------------------------------------*/
#include "hw.h"
#include "ymodem.h"

/* Exported types ------------------------------------------------------------*/
typedef void (*pFunction)(void);

/* Exported constants --------------------------------------------------------*/
/* Constants used by Serial Command Line Mode */
#define CMD_STRING_SIZE                             (128)

#define ApplicationAddress                          (0x8003000)

#define PAGE_SIZE                                   (0x800)     /* 2 KByte */
#define FLASH_SIZE                                  (0x80000)   /* 512 KByte */

/* Compute the FLASH upload image size */
#define FLASH_IMAGE_SIZE                            (u32)(FLASH_SIZE - (ApplicationAddress - 0x08000000))

/* Exported macro ------------------------------------------------------------*/
/* Common routines */
#define IS_AF(c)                                    ((c >= 'A') && (c <= 'F'))
#define IS_af(c)                                    ((c >= 'a') && (c <= 'f'))
#define IS_09(c)                                    ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)                               IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)                               IS_09(c)
#define CONVERTDEC(c)                               (c - '0')

#define CONVERTHEX_alpha(c)                         (IS_AF(c) ? (c - 'A' + 10) : (c - 'a' + 10))
#define CONVERTHEX(c)                               (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

#define SerialPutString(x)                          Serial_PutString((u8*)(x))

/* Exported functions ------------------------------------------------------- */
void Int2Str(u8* str, int intnum);
u32 Str2Int(u8* inputstr, int* intnum);
u32 GetIntegerInput(int* num);
u32 SerialKeyPressed(u8* key);
u8 GetKey(void);
void SerialPutChar(u8 c);
void Serial_PutString(u8* s);
void GetInputString(u8* buffP);
u32 FLASH_PagesMask(__IO u32 Size);
void FLASH_DisableWriteProtectionPages(void);
void Main_Menu(void);
void SerialDownload(void);
void SerialUpload(void);

#endif /* __BOOT_COMMON_H__ */