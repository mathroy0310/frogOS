/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StorageController.h                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 01:29:29 by maroy             #+#    #+#             */
/*   Updated: 2024/08/28 01:29:33 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/PCI.h>
#include <kernel/Storage/StorageDevice.h>

namespace Kernel {

class StorageController {
  public:
	FROG::Vector<StorageDevice *>       &devices() { return m_devices; }
	const FROG::Vector<StorageDevice *> &devices() const { return m_devices; }

  protected:
	FROG::Vector<StorageDevice *> m_devices;
};

} // namespace Kernel