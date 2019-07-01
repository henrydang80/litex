#ifndef __SPI_H
#define __SPI_H

#include <generated/csr.h>

/* SPI master register set */
#define SPI_RX0     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x00)) // Data RX0
#define SPI_RX1     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x04)) // Data RX1
#define SPI_RX2     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x08)) // Data RX2
#define SPI_RX3     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x0C)) // Data RX3
#define SPI_TX0     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x00)) // Data TX0
#define SPI_TX1     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x04)) // Data TX1
#define SPI_TX2     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x08)) // Data TX2
#define SPI_TX3     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x0C)) // Data TX3
#define SPI_CTRL    (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x10)) // Control register
#define SPI_DIV     (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x14)) // Clock divider
#define SPI_SS      (*(volatile uint32_t *)(SPI_MASTER_BASE + 0x18)) // Slave select register 

#ifdef SPI_MASTER_BASE
void spi_init(void);
#endif

#endif /* __SPI_H */