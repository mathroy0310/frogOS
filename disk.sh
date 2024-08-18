#!/bin/sh
set -e
. ./build.sh
 
DISK_NAME=frog-os.img
DISK_SIZE=$[50 * 1024 * 1024]
MOUNT_DIR=/mnt

dd if=/dev/zero of=$DISK_NAME bs=512 count=$[$DISK_SIZE / 512]

sed -e 's/\s*\([\+0-9a-zA-Z]*\).*/\1/' << EOF | fdisk $DISK_NAME > /dev/null
  g     # gpt
  n     # new partition
  1     # partition number 1
        # default (from the beginning of the disk)
  +1MiB # bios boot partiton size
  n     # new partition
  2     # partition number 2
        # default (right after bios boot partition)
		# default (to the end of disk)
  t     # set type
  1     # ... of partition 1
  4     # bios boot partition
  t     # set type
  2     # ... of partition 2
  20    # Linux filesystem
  w     # write changes
EOF

LOOP_DEV=$(sudo losetup -f --show $DISK_NAME)
sudo partprobe $LOOP_DEV

PARTITION1=${LOOP_DEV}p1
PARTITION2=${LOOP_DEV}p2

sudo mkfs.ext2 $PARTITION2

# Create the mount point directory if it doesn't exist
sudo mkdir -p $MOUNT_DIR

sudo mount $PARTITION2 $MOUNT_DIR

sudo cp -r sysroot/* ${MOUNT_DIR}/

sudo grub-install --no-floppy --target=i386-pc --modules="normal ext2 multiboot" --boot-directory=${MOUNT_DIR}/boot $LOOP_DEV

echo -e '
set timeout=5
set default=0

set color_normal=light-gray/black
set color_highlight=light-green/green

menuentry "frog-os" {
	multiboot /boot/frog-os.kernel
}
menuentry "frog-os (no serial)" {
	multiboot /boot/frog-os.kernel noserial
}
menuentry "frog-os (no apic)" {
	multiboot /boot/frog-os.kernel noapic
}
menuentry "frog-os (no apic, no serial)" {
	multiboot /boot/frog-os.kernel noapic noserial
}
'  | sudo tee ${MOUNT_DIR}/boot/grub/grub.cfg

sudo umount $MOUNT_DIR
sudo losetup -d $LOOP_DEV
