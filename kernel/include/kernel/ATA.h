/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATA.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 00:03:35 by maroy             #+#    #+#             */
/*   Updated: 2024/08/26 14:23:06 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Vector.h>
#include <kernel/DiskIO.h>

namespace Kernel {

class ATADevice : public DiskDevice {
  public:
	static ATADevice *create(uint16_t io_base, uint16_t ctl_base, uint8_t slave_bit);

	virtual ~ATADevice() {}

	uint16_t            io_base() const { return m_io_base; }
	uint16_t            ctl_base() const { return m_ctl_base; }
	uint8_t             slave_bit() const { return m_slave_bit; }
	virtual const char *type() const = 0;

  protected:
	ATADevice(uint16_t io_base, uint16_t ctl_base, uint8_t slave_bit)
	    : m_io_base(io_base), m_ctl_base(ctl_base), m_slave_bit(slave_bit) {}

  private:
	const uint16_t m_io_base;
	const uint16_t m_ctl_base;
	const uint8_t  m_slave_bit;
};

class PATADevice final : public ATADevice {
  public:
	PATADevice(uint16_t io_base, uint16_t ctl_base, uint8_t slave_bit)
	    : ATADevice(io_base, ctl_base, slave_bit) {}

	virtual uint32_t    sector_size() const override { return m_sector_words * 2; }
	virtual const char *type() const override { return "PATA"; }
	virtual bool        read_sectors(uint32_t lba, uint32_t sector_count, uint8_t *buffer) override;

  protected:
	virtual bool initialize() override;

  private:
	bool read_lba28(uint32_t lba, uint8_t sector_count, uint8_t *buffer);
	bool wait_while_buzy();
	bool wait_for_transfer();
	void flush();

  private:
	bool     m_lba_48 = false;
	uint32_t m_sector_words = 256;
};

} // namespace Kernel