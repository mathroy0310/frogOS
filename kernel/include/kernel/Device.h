/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Device.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/21 15:34:32 by maroy             #+#    #+#             */
/*   Updated: 2024/09/21 16:06:06 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/Vector.h>
#include <kernel/SpinLock.h>

namespace Kernel {

class Device {
  public:
	enum class Type { BlockDevice, CharacterDevice, DeviceController };
	
	virtual ~Device() {}
	virtual Type type() const = 0;
	virtual void update() {}
};

class BlockDevice : public Device {
  public:
	virtual Type type() const override { return Type::BlockDevice; }
};

class CharacterDevice : public Device {
  public:
	virtual Type                type() const override { return Type::CharacterDevice; }
	virtual FROG::ErrorOr<void> read(FROG::Span<uint8_t>) = 0;
};

class DeviceManager {
	FROG_NON_COPYABLE(DeviceManager);
	FROG_NON_MOVABLE(DeviceManager);

  public:
	static DeviceManager  &get();
	
	void                   update();
	void                   add_device(Device *);
	
	FROG::Vector<Device *> devices() { return m_devices; }

  private:
	DeviceManager() = default;

  private:
	SpinLock               m_lock;
	FROG::Vector<Device *> m_devices;
};

} // namespace Kernel