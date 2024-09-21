/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ACPI.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 00:55:42 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 00:55:52 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <FROG/Errors.h>
namespace Kernel
{
	class ACPI
	{
	public:
		struct SDTHeader
		{
			uint8_t signature[4];
			uint32_t length;
			uint8_t revision;
			uint8_t checksum;
			uint8_t oemid[6];
			uint64_t oem_table_id;
			uint32_t oem_revision;
			uint32_t creator_id;
			uint32_t creator_revision;
		} __attribute__((packed));
	public:
		static FROG::ErrorOr<void> initialize();
		static ACPI& get();
		FROG::ErrorOr<const SDTHeader*> get_header(const char[4]);
		void unmap_header(const SDTHeader*);
	private:
		ACPI() = default;
		FROG::ErrorOr<void> initialize_impl();
		const SDTHeader* get_header_from_index(size_t);
	private:
		uintptr_t m_header_table = 0;
		uint32_t m_entry_size = 0;
		uint32_t m_entry_count = 0;
	};
}