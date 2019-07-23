#include "../nRF.h"

static int nRF_stdw(char c, FILE *stream);

static FILE nRF_stdout = FDEV_SETUP_STREAM(nRF_stdw, NULL, _FDEV_SETUP_WRITE);

int buf_curlen = 0;
static int nRF_stdw(char c, FILE *stream) //передача информации
{
	nRF.buf[buf_curlen] = (unsigned char) c;
	buf_curlen++;
	
	if(buf_curlen >= 32)
	{
		nRF_send(buf_curlen);
		buf_curlen = 0;
	}
	return 0;
}

void nRF_write_multi(unsigned char a, unsigned char len)
{
	spi_cslow();
	spi_busSetup(SPI_PRESCALER_4, MSBFIRST, SPI_MODE0, SPI_2X);
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	spi_simpleWrite(a);
	for(unsigned char i = 0; i < 32; i++)
		spi_simpleWrite(nRF.buf[i]);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	spi_cshigh();
	SPCR = 0;
	
	for(unsigned char i = 0; i < 32; i++)
		nRF.buf[i] = 0;
}

void nRF_write(unsigned char a, unsigned char b)
{
	spi_cslow();
	spi_busSetup(SPI_PRESCALER_4, MSBFIRST, SPI_MODE0, SPI_2X);
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	spi_simpleWrite(a);
	spi_simpleWrite(b);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	spi_cshigh();
	SPCR = 0;
}

unsigned char nRF_readReg(unsigned char a)
{
	unsigned char c;
	spi_cslow();
	spi_busSetup(SPI_PRESCALER_4, MSBFIRST, SPI_MODE0, SPI_2X);
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	c = spi_simpleRead(a & 0x1F);
	c = spi_simpleRead(0x00);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	spi_cshigh();
	SPCR = 0;
		
	return c;
}

unsigned char nRF_readReg_a(unsigned char a, int length)
{
	unsigned char c;
	spi_cslow();	
	spi_busSetup(SPI_PRESCALER_4, MSBFIRST, SPI_MODE0, SPI_2X);
	nRF_CSN_port &= ~(1<<nRF_CSN_pin);
	c = spi_simpleRead(a & 0x1F);
	for(unsigned char i = 0; i < length; i++)
		nRF.buf[i] = spi_simpleRead(0x00);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	spi_cshigh();
	SPCR = 0;
	return c;
}

void nRF_stdef()
{
	stdout = &nRF_stdout;
}

// nRF_init(частота в МГц)
void nRF_init(int freq)
{
	nRF.freq = freq;
	
	// Включение РМ
	nRF_CE_ddr |= (1<<nRF_CE_pin);
	nRF_CSN_ddr |= (1<<nRF_CSN_pin);
	nRF_CSN_port |= (1<<nRF_CSN_pin);
	nRF_CE_port &= ~(1<<nRF_CE_pin);					
	
	/* Настройка CONFIG: 
	7: Резерв = 0 -> Допустим только 0,
	6: MASK_RX_DR = 0 -> Вывести прерывание RX_DR как '0' на пин IRQ, 
	5: MASK_TX_DS = 0 -> Вывести прерывание RX_DR как '0' на пин IRQ,
	4: MASK_MAX_RT = 0 -> Вывести прерывание RX_DR как '0' на пин IRQ,
	3: EN_CRC = 1 -> Включить CRC, 
	2: CRCO = 0 -> Включить РМ, 
	1: PWR_UP = 1 -> CRC 2 байта, 
	0: PRIM_RX = 0 -> TX Control */
	nRF_write_reg(0x00, 0b00001010);
	 
	/* Настройка SETUP: 
	7: CONT_WAVE = 0 -> "continuous carrier transmit when high is disabled",
	6: Резерв = 0 -> Допустим только 0,
	5: RF_DR_LOW = 0 -> Вместе с RF_DR_HIGH, скорость 2000 кбит/с,
	4: PLL_LOCK = 0 -> PLL Lock выключен, использовать только при тестировании,
	3: RF_DR_HIGH = 1 -> Вместе с RF_DR_LOW, скорость 250 кбит/с, 
	2: RF_PWR(_A) = 1 -> 0 дБм (RF_PWR(_A) | RF_PWR(_B) = 00), 
	1: RF_PWR(_B) = 1 -> 0 дБм (RF_PWR(_A) | RF_PWR(_B) = 00), 
	0: Ничто = 0 -> Без разницы */
	nRF_write_reg(0x06, 0b00001110);   	// Скорость 2000 кбит/с (RF_DR_LOW = 0, RF_DR_HIGH = 1), выходная мощность 0 дБм

	/* Настройка STATUS
	7: Резерв = 0 -> Допустим только 0,
	6: RX_DR = 1 -> Сброс прерывания принятия данных,
	5: TX_DS = 1 -> Сброс прерывания отправки данных,
	4: MAX_RT = 1 -> Сброс прерывания максимальных попыток отправки,
	3-0: Доступно только для чтения */
	nRF_write_reg(0x07, 0b01110000);	

	nRF_write_reg(0x01, 0b00111111);			// Включить AutoACK на pipe0-5
	nRF_write_reg(0x02, 0b00111111);			// Включить RX-адрес на pipe0-5
	nRF_write_reg(0x03, 0b00000011);			// Ширина адреса - 5 байт
	nRF_write_reg(0x04, 0b00011111);			// 500 мкс между повторными отправками и до 15 раз повтора
	nRF_write_reg(0x05, freq - 2400);	// Запись частотного канала

	// Запись адреса RX трубы 0
	nRF.buf[0] = 0xE7;
	nRF.buf[1] = 0xE7;
	nRF.buf[2] = 0xE7;
	nRF.buf[3] = 0xE7;
	nRF.buf[4] = 0xE7;

	nRF_write_multi(0x0A, 5);
	
	// Запись адреса RX трубы 1
	nRF.buf[0] = 0xE7;
	nRF.buf[1] = 0xE7;
	nRF.buf[2] = 0xE7;
	nRF.buf[3] = 0xE7;
	nRF.buf[4] = 0xE7;

	nRF_write_multi(0x0B, 5);
	// Конец записи адреса RX трубы 0 & 1

	// Запись адреса TX трубы 0
	nRF.buf[0] = 0xE7;
	nRF.buf[1] = 0xE7;
	nRF.buf[2] = 0xE7;
	nRF.buf[3] = 0xE7;
	nRF.buf[4] = 0xE7;

	nRF_write_multi(0x10, 5);
	// Конец записи адреса RX трубы 0
	
	nRF_write_reg(0x1C, 0b00111111);	// DYNPD на все трубы
	nRF_write_reg(0x1D, 0b00000111);	// FEATURE ALL INCLUSIVE (Динамический размер пакетов, пакеты с ACK, пакеты с NO_ACK) 

	// Оистить буферы
	nRF_write(0xE2, 0);
	nRF_write(0xE1, 0);					
	
	nRF_write_reg(0x07, 0b01110000);
}

unsigned char nRF_send(int len)
{
	nRF_write_reg(0x05, nRF.freq - 2400);	// Запись частотного канала для сброса количества потерянных пакетов
	uint8_t fifoReg = nRF_readReg(0x17);
	
	if(fifoReg & (1<<5))
		nRF_TxComplete();
	
	//nRF_write(0xE1, 0x00);
	nRF_write(0xE2, 0x00);
	nRF_write_reg(0x07, 0b01110000);
	
	nRF_write_multi(0xA0, len);
	
	if(nRF_TxComplete())
		return 1;
	return 0;
}

void nRF_send_other(char * data)
{
	int i, L, N = strlen(data);
	
	for (L = 0; L < N; L += 31)
	{
		if ((N - L) > 31)
		{
			for (i = 0; i < 31; i++)
				nRF.buf[i] = data[L + i];
			nRF.buf[31] = 0;
		}
		else
		{
			for (i = 0; i < (N - L); i++)
				nRF.buf[i] = data[L + i];
			for (i = (N - L); i < 32; i++)
				nRF.buf[i] = 0;
		}
		nRF_send(32);
	}
}

uint8_t nRF_TxComplete()
{
	uint8_t fifoReg, statusReg;
	uint8_t txBufferIsEmpty;
	uint8_t packetWasSent, packetCouldNotBeSent;
	uint8_t txAttemptCount = 0;
	uint8_t result = 0; // Флаг для не успешной передачи

	// FIFO буферов = 3
	// Для обмена с подтверждением необходимо двухсторонее сообщение
	// Максимальное количество повторных отправок = 15
	// 3 * 15 * 2 = 90
	const static uint8_t MAX_TX_ATTEMPT_COUNT = 90;

	while (txAttemptCount++ < MAX_TX_ATTEMPT_COUNT)
	{
		fifoReg = nRF_readReg(0x17);
		txBufferIsEmpty = fifoReg & (1<<4);

		if (txBufferIsEmpty)
		{
			result = 1; // Indicate success.
			break;
		}

		nRF_CE_port |= (1<<nRF_CE_pin);
		_delay_us(20);
		nRF_CE_port &= ~(1<<nRF_CE_pin);
		_delay_us(500);
		
		statusReg = nRF_readReg(0x07);
		packetWasSent = statusReg & (1<<5);
		packetCouldNotBeSent = statusReg & (1<<4);

		if (packetWasSent)
			nRF_write_reg(0x07, 0b00100000);	   // Clear TX success flag.
		
		else if (packetCouldNotBeSent)
		{
			printf("STAT NOT SEND SAD %x %x\r\n", fifoReg, statusReg);
			nRF_write(0xE1, 0);							// Clear TX buffer.
			nRF_write_reg(0x07, 0b00010000);          // Clear max retry flag.
			break;
		}
	}

	return result;
}