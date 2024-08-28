/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PCI.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 01:19:14 by maroy             #+#    #+#             */
/*   Updated: 2024/08/28 01:19:24 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Vector.h>

namespace Kernel
{

	class PCIDevice
	{
	public:
		const uint8_t bus;
		const uint8_t dev;
		const uint8_t func;

		const uint8_t class_code;
		const uint8_t subclass;

		uint32_t read_dword(uint8_t) const;
		uint16_t read_word(uint8_t) const;
		uint8_t  read_byte(uint8_t) const;
	};

	class PCI
	{
	public:
		static bool initialize();
		static PCI& get();

		const FROG::Vector<PCIDevice>& devices() const { return m_devices; }

	private:
		PCI() = default;
		void check_function(uint8_t bus, uint8_t dev, uint8_t func);
		void check_device(uint8_t bus, uint8_t dev);
		void check_bus(uint8_t bus);
		void check_all_buses();

	private:
		FROG::Vector<PCIDevice> m_devices;
	};

}