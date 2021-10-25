/* Includes ------------------------------------------------------------------*/
#include "ymodem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 file_name[FILE_NAME_LENGTH];
u32 FlashDestination = ApplicationAddress; /* Flash user program offset */
u16 PageSize = PAGE_SIZE;
u32 EraseCounter = 0x0;
u32 NbrOfPage = 0;
FLASH_Status FLASHStatus = FLASH_COMPLETE;
u32 RamSource;
extern u8 tab_1024[1024];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Receive byte from sender
  * @param  c: Character
  * @param  timeout: Timeout
  * @retval 0: Byte received
  *         -1: Timeout
  */
static int Receive_Byte(u8* c, u32 timeout)
{
    while (timeout-- > 0)
    {
        if (SerialKeyPressed(c) == 1)
        {
            return 0;
        }
    }
    return -1;
}

/**
  * @brief  Send a byte
  * @param  c: Character
  * @retval 0: Byte sent
  */
static u32 Send_Byte(u8 c)
{
    SerialPutChar(c);
    return 0;
}

/**
  * @brief  Receive a packet from sender
  * @param  data
  * @param  length
  * @param  timeout
  *     0: end of transmission
  *    -1: abort by sender
  *    >0: packet length
  * @retval 0: normally return
  *        -1: timeout or packet error
  *         1: abort by user
  */
static int Receive_Packet(u8* data, int* length, u32 timeout)
{
    u16 i, packet_size;
    u8 c;
    *length = 0;
    if (Receive_Byte(&c, timeout) != 0)
    {
        return -1;
    }
    switch (c)
    {
    case SOH:
        packet_size = PACKET_SIZE;
        break;
    case STX:
        packet_size = PACKET_1K_SIZE;
        break;
    case EOT:
        return 0;
    case CA:
        if ((Receive_Byte(&c, timeout) == 0) && (c == CA))
        {
            *length = -1;
            return 0;
        }
        else
        {
            return -1;
        }
    case ABORT1:
    case ABORT2:
        return 1;
    default:
        return -1;
    }
    *data = c;
    for (i = 1; i < (packet_size + PACKET_OVERHEAD); i++)
    {
        if (Receive_Byte(data + i, timeout) != 0)
        {
            return -1;
        }
    }
    if (data[PACKET_SEQNO_INDEX] != ((data[PACKET_SEQNO_COMP_INDEX] ^ 0xff) & 0xff))
    {
        return -1;
    }
    *length = packet_size;
    return 0;
}

/**
  * @brief  Receive a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int Ymodem_Receive(u8* buf)
{
    u8 packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD], file_size[FILE_SIZE_LENGTH], *file_ptr, *buf_ptr;
    int i, j, packet_length, session_done, file_done, packets_received, errors, session_begin, size = 0;

    /* Initialize FlashDestination variable */
    FlashDestination = ApplicationAddress;

    for (session_done = 0, errors = 0, session_begin = 0;;)
    {
        for (packets_received = 0, file_done = 0, buf_ptr = buf;;)
        {
            switch (Receive_Packet(packet_data, &packet_length, NAK_TIMEOUT))
            {
            case 0:
                errors = 0;
                switch (packet_length)
                {
                /* Abort by sender */
                case -1:
                    Send_Byte(ACK);
                    return 0;
                /* End of transmission */
                case 0:
                    Send_Byte(ACK);
                    file_done = 1;
                    break;
                /* Normal packet */
                default:
                    if ((packet_data[PACKET_SEQNO_INDEX] & 0xff) != (packets_received & 0xff))
                    {
                        Send_Byte(NAK);
                    }
                    else
                    {
                        if (packets_received == 0)
                        {
                            /* Filename packet */
                            if (packet_data[PACKET_HEADER] != 0)
                            {
                                /* Filename packet has valid data */
                                for (i = 0, file_ptr = packet_data + PACKET_HEADER; (*file_ptr != 0) && (i < FILE_NAME_LENGTH);)
                                {
                                    file_name[i++] = *file_ptr++;
                                }
                                file_name[i++] = '\0';
                                for (i = 0, file_ptr++; (*file_ptr != ' ') && (i < FILE_SIZE_LENGTH);)
                                {
                                    file_size[i++] = *file_ptr++;
                                }
                                file_size[i++] = '\0';
                                Str2Int(file_size, &size);

                                /* Test the size of the image to be sent */
                                /* Image size is greater than Flash size */
                                if (size > (FLASH_SIZE - 1))
                                {
                                    /* End session */
                                    Send_Byte(CA);
                                    Send_Byte(CA);
                                    return -1;
                                }

                                /* Erase the needed pages where the user application will be loaded */
                                /* Define the number of page to be erased */
                                NbrOfPage = FLASH_PagesMask(size);

                                /* Erase the FLASH pages */
                                for (EraseCounter = 0; (EraseCounter < NbrOfPage) && (FLASHStatus == FLASH_COMPLETE); EraseCounter++)
                                {
                                    FLASHStatus = FLASH_ErasePage(FlashDestination + (PageSize * EraseCounter));
                                }
                                Send_Byte(ACK);
                                Send_Byte(CRC16);
                            }
                            /* Filename packet is empty, end session */
                            else
                            {
                                Send_Byte(ACK);
                                file_done = 1;
                                session_done = 1;
                                break;
                            }
                        }
                        /* Data packet */
                        else
                        {
                            memcpy(buf_ptr, packet_data + PACKET_HEADER, packet_length);
                            RamSource = (u32)buf;
                            for (j = 0; (j < packet_length) && (FlashDestination < ApplicationAddress + size); j += 4)
                            {
                                /* Program the data received into STM32F10x Flash */
                                FLASH_ProgramWord(FlashDestination, *(u32*)RamSource);

                                if (*(u32*)FlashDestination != *(u32*)RamSource)
                                {
                                    /* End session */
                                    Send_Byte(CA);
                                    Send_Byte(CA);
                                    return -2;
                                }
                                FlashDestination += 4;
                                RamSource += 4;
                            }
                            Send_Byte(ACK);
                        }
                        packets_received++;
                        session_begin = 1;
                    }
                }
                break;
            case 1:
                Send_Byte(CA);
                Send_Byte(CA);
                return -3;
            default:
                if (session_begin > 0)
                {
                    errors++;
                }
                if (errors > MAX_ERRORS)
                {
                    Send_Byte(CA);
                    Send_Byte(CA);
                    return 0;
                }
                Send_Byte(CRC16);
                break;
            }
            if (file_done != 0)
            {
                break;
            }
        }
        if (session_done != 0)
        {
            break;
        }
    }

    return (int)size;
}

/**
  * @brief  check response using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
int Ymodem_CheckResponse(u8 c)
{
    return 0;
}

/**
  * @brief  Prepare the first block
  * @param  timeout
  *     0: end of transmission
  */
void Ymodem_PrepareIntialPacket(u8* data, const u8* fileName, u32* length)
{
    u16 i, j;
    u8 file_ptr[10];

    /* Make first three packet */
    data[0] = SOH;
    data[1] = 0x00;
    data[2] = 0xff;

    /* Filename packet has valid data */
    for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH); i++)
    {
        data[i + PACKET_HEADER] = fileName[i];
    }

    data[i + PACKET_HEADER] = 0x00;

    Int2Str(file_ptr, *length);
    for (j = 0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0';)
    {
        data[i++] = file_ptr[j++];
    }

    for (j = i; j < PACKET_SIZE + PACKET_HEADER; j++)
    {
        data[j] = 0;
    }
}

/**
  * @brief  Prepare the data packet
  * @param  timeout
  *     0: end of transmission
  */
void Ymodem_PreparePacket(u8* SourceBuf, u8* data, u8 pktNo, u32 sizeBlk)
{
    u16 i, size, packetSize;
    u8* file_ptr;

    /* Make first three packet */
    packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;
    size = sizeBlk < packetSize ? sizeBlk : packetSize;
    if (packetSize == PACKET_1K_SIZE)
    {
        data[0] = STX;
    }
    else
    {
        data[0] = SOH;
    }
    data[1] = pktNo;
    data[2] = (~pktNo);
    file_ptr = SourceBuf;

    /* Filename packet has valid data */
    for (i = PACKET_HEADER; i < size + PACKET_HEADER; i++)
    {
        data[i] = *file_ptr++;
    }
    if (size <= packetSize)
    {
        for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
        {
            data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
        }
    }
}

/**
  * @brief  Update CRC16 for input byte
  * @param  CRC input value 
  * @param  input byte
   * @retval None
  */
u16 UpdateCRC16(u16 crcIn, u8 byte)
{
    u32 crc = crcIn;
    u32 in = byte | 0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if (in & 0x100)
            ++crc;
        if (crc & 0x10000)
            crc ^= 0x1021;
    } while (!(in & 0x10000));
    return crc & 0xffffu;
}

/**
  * @brief  Cal CRC16 for YModem Packet
  * @param  data
  * @param  length
   * @retval None
  */
u16 Cal_CRC16(const u8* data, u32 size)
{
    u32 crc = 0;
    const u8* dataEnd = data + size;
    while (data < dataEnd)
        crc = UpdateCRC16(crc, *data++);

    crc = UpdateCRC16(crc, 0);
    crc = UpdateCRC16(crc, 0);
    return crc & 0xffffu;
}

/**
  * @brief  Cal Check sum for YModem Packet
  * @param  data
  * @param  length
   * @retval None
  */
u8 CalChecksum(const u8* data, u32 size)
{
    u32 sum = 0;
    const u8* dataEnd = data + size;
    while (data < dataEnd)
        sum += *data++;
    return sum & 0xffu;
}

/**
  * @brief  Transmit a data packet using the ymodem protocol
  * @param  data
  * @param  length
   * @retval None
  */
void Ymodem_SendPacket(u8* data, u16 length)
{
    u16 i;
    i = 0;
    while (i < length)
    {
        Send_Byte(data[i]);
        i++;
    }
}

/**
  * @brief  Transmit a file using the ymodem protocol
  * @param  buf: Address of the first byte
  * @retval The size of the file
  */
u8 Ymodem_Transmit(u8* buf, const u8* sendFileName, u32 sizeFile)
{

    u8 packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD];
    u8 FileName[FILE_NAME_LENGTH];
    u8* buf_ptr, tempCheckSum;
    u16 tempCRC, blkNumber;
    u8 receivedC[2], CRC16_F = 0, i;
    u32 errors, ackReceived, size = 0, pktSize;

    errors = 0;
    ackReceived = 0;
    for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
    {
        FileName[i] = sendFileName[i];
    }
    CRC16_F = 1;

    /* Prepare first block */
    Ymodem_PrepareIntialPacket(&packet_data[0], FileName, &sizeFile);

    do
    {
        /* Send Packet */
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC or Check Sum based on CRC16_F */
        if (CRC16_F)
        {
            tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
            Send_Byte(tempCRC >> 8);
            Send_Byte(tempCRC & 0xFF);
        }
        else
        {
            tempCheckSum = CalChecksum(&packet_data[3], PACKET_SIZE);
            Send_Byte(tempCheckSum);
        }

        /* Wait for Ack and 'C' */
        if (Receive_Byte(&receivedC[0], 10000) == 0)
        {
            if (receivedC[0] == ACK)
            {
                /* Packet transfered correctly */
                ackReceived = 1;
            }
        }
        else
        {
            errors++;
        }
    } while (!ackReceived && (errors < 0x0A));

    if (errors >= 0x0A)
    {
        return errors;
    }
    buf_ptr = buf;
    size = sizeFile;
    blkNumber = 0x01;
    /* Here 1024 bytes package is used to send the packets */

    /* Resend packet if NAK  for a count of 10 else end of commuincation */
    while (size)
    {
        /* Prepare next packet */
        Ymodem_PreparePacket(buf_ptr, &packet_data[0], blkNumber, size);
        ackReceived = 0;
        receivedC[0] = 0;
        errors = 0;
        do
        {
            /* Send next packet */
            if (size >= PACKET_1K_SIZE)
            {
                pktSize = PACKET_1K_SIZE;
            }
            else
            {
                pktSize = PACKET_SIZE;
            }
            Ymodem_SendPacket(packet_data, pktSize + PACKET_HEADER);
            /* Send CRC or Check Sum based on CRC16_F */
            /* Send CRC or Check Sum based on CRC16_F */
            if (CRC16_F)
            {
                tempCRC = Cal_CRC16(&packet_data[3], pktSize);
                Send_Byte(tempCRC >> 8);
                Send_Byte(tempCRC & 0xFF);
            }
            else
            {
                tempCheckSum = CalChecksum(&packet_data[3], pktSize);
                Send_Byte(tempCheckSum);
            }

            /* Wait for Ack */
            if ((Receive_Byte(&receivedC[0], 100000) == 0) && (receivedC[0] == ACK))
            {
                ackReceived = 1;
                if (size > pktSize)
                {
                    buf_ptr += pktSize;
                    size -= pktSize;
                    if (blkNumber == (FLASH_IMAGE_SIZE / 1024))
                    {
                        return 0xFF; /*  error */
                    }
                    else
                    {
                        blkNumber++;
                    }
                }
                else
                {
                    buf_ptr += pktSize;
                    size = 0;
                }
            }
            else
            {
                errors++;
            }
        } while (!ackReceived && (errors < 0x0A));
        /* Resend packet if NAK  for a count of 10 else end of commuincation */

        if (errors >= 0x0A)
        {
            return errors;
        }
    }
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;
    do
    {
        Send_Byte(EOT);
        /* Send (EOT); */
        /* Wait for Ack */
        if ((Receive_Byte(&receivedC[0], 10000) == 0) && receivedC[0] == ACK)
        {
            ackReceived = 1;
        }
        else
        {
            errors++;
        }
    } while (!ackReceived && (errors < 0x0A));

    if (errors >= 0x0A)
    {
        return errors;
    }

    /* Last packet preparation */
    ackReceived = 0;
    receivedC[0] = 0x00;
    errors = 0;

    packet_data[0] = SOH;
    packet_data[1] = 0;
    packet_data[2] = 0xFF;

    for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
    {
        packet_data[i] = 0x00;
    }

    do
    {
        /* Send Packet */
        Ymodem_SendPacket(packet_data, PACKET_SIZE + PACKET_HEADER);
        /* Send CRC or Check Sum based on CRC16_F */
        tempCRC = Cal_CRC16(&packet_data[3], PACKET_SIZE);
        Send_Byte(tempCRC >> 8);
        Send_Byte(tempCRC & 0xFF);

        /* Wait for Ack and 'C' */
        if (Receive_Byte(&receivedC[0], 10000) == 0)
        {
            if (receivedC[0] == ACK)
            {
                /* Packet transfered correctly */
                ackReceived = 1;
            }
        }
        else
        {
            errors++;
        }

    } while (!ackReceived && (errors < 0x0A));
    /* Resend packet if NAK  for a count of 10  else end of commuincation */
    if (errors >= 0x0A)
    {
        return errors;
    }

    do
    {
        Send_Byte(EOT);
        /* Send (EOT); */
        /* Wait for Ack */
        if ((Receive_Byte(&receivedC[0], 10000) == 0) && receivedC[0] == ACK)
        {
            ackReceived = 1;
        }
        else
        {
            errors++;
        }
    } while (!ackReceived && (errors < 0x0A));

    if (errors >= 0x0A)
    {
        return errors;
    }
    return 0; /* file trasmitted successfully */
}