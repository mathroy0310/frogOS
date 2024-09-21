/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:39:15 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 15:39:29 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/Device.h>
#include <kernel/LockGuard.h>

namespace Kernel {
    
DeviceManager &DeviceManager::get() {
	static DeviceManager instance;
	return instance;
}

void DeviceManager::update() {
	LockGuard _(m_lock);
	for (Device *device : m_devices)
		device->update();
}

void DeviceManager::add_device(Device *device) {
	LockGuard _(m_lock);
	MUST(m_devices.push_back(device));
}

} // namespace Kernel