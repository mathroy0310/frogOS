/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATAController.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 01:29:12 by maroy             #+#    #+#             */
/*   Updated: 2024/09/20 14:29:49 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Errors.h>
#include <kernel/Storage/StorageController.h>

namespace Kernel {

struct ATABus;
class ATAController;

class ATADevice : public StorageDevice {
  public:
	virtual FROG::ErrorOr<void> read_sectors(uint64_t, uint8_t, uint8_t *) override;
	virtual FROG::ErrorOr<void> write_sectors(uint64_t, uint8_t, const uint8_t *) override;
	virtual uint32_t            sector_size() const override { return sector_words * 2; }
	virtual uint64_t            total_size() const override { return lba_count * sector_size(); }

  private:
	enum class Type {
		Unknown,
		ATA,
		ATAPI,
	};

	Type     type;
	uint8_t  slave_bit; // 0x00 for master, 0x10 for slave
	uint16_t signature;
	uint16_t capabilities;
	uint32_t command_set;
	uint32_t sector_words;
	uint64_t lba_count;
	char     model[41];

	ATABus        *bus;
	ATAController *controller;

	friend class ATAController;
};

struct ATABus {
	uint16_t  base;
	uint16_t  ctrl;
	ATADevice devices[2];

	uint8_t             read(uint16_t);
	void                read_buffer(uint16_t, uint16_t *, size_t);
	void                write(uint16_t, uint8_t);
	void                write_buffer(uint16_t, const uint16_t *, size_t);
	FROG::ErrorOr<void> wait(bool);
	FROG::Error         error();
};

class ATAController : public StorageController {
  public:
	static FROG::ErrorOr<ATAController *> create(const PCIDevice &);

  private:
	ATAController(const PCIDevice &device) : m_pci_device(device) {}
	FROG::ErrorOr<void> initialize();

	FROG::ErrorOr<void> read(ATADevice *, uint64_t, uint8_t, uint8_t *);
	FROG::ErrorOr<void>  write(ATADevice *, uint64_t, uint8_t, const uint8_t *);

  private:
	ATABus           m_buses[2];
	const PCIDevice &m_pci_device;

	friend class ATADevice;
};

} // namespace Kernel