/* Includes ------------------------------------------------------------------*/
#include "boot_common.h"
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
pFunction Jump_To_Application;
u32 JumpAddress;
u32 BlockNbr = 0, UserMemoryMask = 0;
__IO u32 FlashProtection = 0;
extern u32 FlashDestination;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Convert an Integer to a string
  * @param  str: The string
  * @param  intnum: The intger to be converted
  * @retval None
  */
void Int2Str(u8* str, int intnum)
{
    u32 i, Div = 1000000000, j = 0, Status = 0;

    for (i = 0; i < 10; i++)
    {
        str[j++] = (intnum / Div) + 48;

        intnum = intnum % Div;
        Div /= 10;
        if ((str[j - 1] == '0') & (Status == 0))
        {
            j = 0;
        }
        else
        {
            Status++;
        }
    }
}

/**
  * @brief  Convert a string to an integer
  * @param  inputstr: The string to be converted
  * @param  intnum: The intger value
  * @retval 1: Correct
  *         0: Error
  */
u32 Str2Int(u8* inputstr, int* intnum)
{
    u32 i = 0, res = 0;
    u32 val = 0;

    if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
    {
        if (inputstr[2] == '\0')
        {
            return 0;
        }
        for (i = 2; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1; */
                res = 1;
                break;
            }
            if (ISVALIDHEX(inputstr[i]))
            {
                val = (val << 4) + CONVERTHEX(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* over 8 digit hex --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }
    else /* max 10-digit decimal input */
    {
        for (i = 0; i < 11; i++)
        {
            if (inputstr[i] == '\0')
            {
                *intnum = val;
                /* return 1 */
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
            {
                val = val << 10;
                *intnum = val;
                res = 1;
                break;
            }
            else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
            {
                val = val << 20;
                *intnum = val;
                res = 1;
                break;
            }
            else if (ISVALIDDEC(inputstr[i]))
            {
                val = val * 10 + CONVERTDEC(inputstr[i]);
            }
            else
            {
                /* return 0, Invalid input */
                res = 0;
                break;
            }
        }
        /* Over 10 digit decimal --invalid */
        if (i >= 11)
        {
            res = 0;
        }
    }

    return res;
}

/**
  * @brief  Get an integer from the HyperTerminal
  * @param  num: The inetger
  * @retval 1: Correct
  *         0: Error
  */
u32 GetIntegerInput(int* num)
{
    u8 inputstr[16];

    while (1)
    {
        GetInputString(inputstr);
        if (inputstr[0] == '\0')
            continue;
        if ((inputstr[0] == 'a' || inputstr[0] == 'A') && inputstr[1] == '\0')
        {
            SerialPutString("User Cancelled \r\n");
            return 0;
        }

        if (Str2Int(inputstr, num) == 0)
        {
            SerialPutString("Error, Input again: \r\n");
        }
        else
        {
            return 1;
        }
    }
}

/**
  * @brief  Test to see if a key has been pressed on the HyperTerminal
  * @param  key: The key pressed
  * @retval 1: Correct
  *         0: Error
  */
u32 SerialKeyPressed(u8* key)
{
    if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != RESET)
    {
        *key = (u8)USART1->DR;
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
  * @brief  Get a key from the HyperTerminal
  * @param  None
  * @retval The Key Pressed
  */
u8 GetKey(void)
{
    u8 key = 0;

    /* Waiting for user input */
    while (1)
    {
        if (SerialKeyPressed((u8*)&key))
            break;
    }
    return key;
}

/**
  * @brief  Print a character on the HyperTerminal
  * @param  c: The character to be printed
  * @retval None
  */
void SerialPutChar(u8 c)
{
    USART_SendData(USART1, c);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) {}
}

/**
  * @brief  Print a string on the HyperTerminal
  * @param  s: The string to be printed
  * @retval None
  */
void Serial_PutString(u8* s)
{
    while (*s != '\0')
    {
        SerialPutChar(*s);
        s++;
    }
}

/**
  * @brief  Get Input string from the HyperTerminal
  * @param  buffP: The input string
  * @retval None
  */
void GetInputString(u8* buffP)
{
    u32 bytes_read = 0;
    u8 c = 0;
    do
    {
        c = GetKey();
        if (c == '\r')
            break;
        if (c == '\b') /* Backspace */
        {
            if (bytes_read > 0)
            {
                SerialPutString("\b \b");
                bytes_read--;
            }
            continue;
        }
        if (bytes_read >= CMD_STRING_SIZE)
        {
            SerialPutString("Command string size overflow\r\n");
            bytes_read = 0;
            continue;
        }
        if (c >= 0x20 && c <= 0x7E)
        {
            buffP[bytes_read++] = c;
            SerialPutChar(c);
        }
    } while (1);
    SerialPutString(("\n\r"));
    buffP[bytes_read] = '\0';
}

/**
  * @brief  Calculate the number of pages
  * @param  Size: The image size
  * @retval The number of pages
  */
u32 FLASH_PagesMask(__IO u32 Size)
{
    u32 pagenumber = 0x0;
    u32 size = Size;

    if ((size % PAGE_SIZE) != 0)
    {
        pagenumber = (size / PAGE_SIZE) + 1;
    }
    else
    {
        pagenumber = size / PAGE_SIZE;
    }
    return pagenumber;
}

/**
  * @brief  Disable the write protection of desired pages
  * @param  None
  * @retval None
  */
void FLASH_DisableWriteProtectionPages(void)
{
    u32 useroptionbyte = 0, WRPR = 0;
    u16 var1 = OB_IWDG_SW, var2 = OB_STOP_NoRST, var3 = OB_STDBY_NoRST;
    FLASH_Status status = FLASH_BUSY;

    WRPR = FLASH_GetWriteProtectionOptionByte();

    /* Test if user memory is write protected */
    if ((WRPR & UserMemoryMask) != UserMemoryMask)
    {
        useroptionbyte = FLASH_GetUserOptionByte();

        UserMemoryMask |= WRPR;

        status = FLASH_EraseOptionBytes();

        if (UserMemoryMask != 0xFFFFFFFF)
        {
            status = FLASH_EnableWriteProtection((u32)~UserMemoryMask);
        }

        /* Test if user Option Bytes are programmed */
        if ((useroptionbyte & 0x07) != 0x07)
        {
            /* Restore user Option Bytes */
            if ((useroptionbyte & 0x01) == 0x0)
            {
                var1 = OB_IWDG_HW;
            }
            if ((useroptionbyte & 0x02) == 0x0)
            {
                var2 = OB_STOP_RST;
            }
            if ((useroptionbyte & 0x04) == 0x0)
            {
                var3 = OB_STDBY_RST;
            }

            FLASH_UserOptionByteConfig(var1, var2, var3);
        }

        if (status == FLASH_COMPLETE)
        {
            SerialPutString("Write Protection disabled...\r\n");

            SerialPutString("...and a System Reset will be generated to re-load the new option bytes\r\n");

            /* Generate System Reset to load the new option byte values */
            NVIC_SystemReset();
        }
        else
        {
            SerialPutString("Error: Flash write unprotection failed...\r\n");
        }
    }
    else
    {
        SerialPutString("Flash memory not write protected\r\n");
    }
}

/**
  * @brief  Display the Main Menu on to HyperTerminal
  * @param  None
  * @retval None
  */
void Main_Menu(void)
{
    u8 key = 0;

    /* Get the number of block (4 or 2 pages) from where the user program will be loaded */
    BlockNbr = (FlashDestination - 0x08000000) >> 12;

    /* Compute the mask to test if the Flash memory, where the user program will be
     loaded, is write protected */
    if (BlockNbr < 62)
    {
        UserMemoryMask = ((u32) ~((1 << BlockNbr) - 1));
    }
    else
    {
        UserMemoryMask = ((u32)0x80000000);
    }

    /* Test if any page of Flash memory where program user will be loaded is write protected */
    if ((FLASH_GetWriteProtectionOptionByte() & UserMemoryMask) != UserMemoryMask)
    {
        FlashProtection = 1;
    }
    else
    {
        FlashProtection = 0;
    }

    while (1)
    {
        uart_send_str(USART1, "\r\n================== Main Menu ============================\r\n\n");
        uart_send_str(USART1, "  1. Download Image\r\n\n");
        uart_send_str(USART1, "  2. Execute The Program\r\n\n");
        uart_send_str(USART1, "  3. Display The Configuration Data\r\n\n");
        if (FlashProtection == 1)
        {
            uart_send_str(USART1, "  4. Disable the write protection\r\n\n");
        }
        uart_send_str(USART1, "\r\n=========================================================\r\n\n");

        key = GetKey();

        if (key == 0x31)
        {
            /* Download user application in the Flash */
            SerialDownload();
        }
        else if (key == 0x32)
        {
            JumpAddress = *(__IO u32 *)(ApplicationAddress + 4);

            /* Jump to user application */
            Jump_To_Application = (pFunction)JumpAddress;
            /* Initialize user application's Stack Pointer */
            __set_MSP(*(__IO u32 *)ApplicationAddress);
            Jump_To_Application();
        }
        else if (key == 0x33)
        {
        }
        else if ((key == 0x34) && (FlashProtection == 1))
        {
            /* Disable the write protection of desired pages */
            FLASH_DisableWriteProtectionPages();
        }
        else
        {
            if (FlashProtection == 0)
            {
                SerialPutString("Invalid Number ! ==> The number should be either 1, 2 or 3\r");
            }
            else
            {
                SerialPutString("Invalid Number ! ==> The number should be either 1, 2, 3 or 4\r");
            }
        }
    }
}