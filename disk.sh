#!/bin/sh
set -e
. ./build.sh
 
DISK_NAME=frog-os.img
DISK_SIZE=$[50 * 1024 * 1024]
MOUNT_DIR=/mnt

dd if=/dev/zero of=$DISK_NAME bs=512 count=$[$DISK_SIZE / 512]

sed -e 's/\s*\([-\+[:alnum:]]*\).*/\1/' << EOF | fdisk $DISK_NAME
  g     # gpt
  n     # new partition
  1     # partition number 1
        # default (from the beginning of the disk)
  +1MiB # bios boot partiton size
  n	  # new partition
  3	  # partition number 3
	  # default (right after bios boot partition)
  +10Mib# partition size
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
  x	  # expert menu
  n     # partition name
  3	  # ... of partition 3
  frog-mount
  n	  # partition name
  2	  # ... of partition 2
  frog-root
  r	  # back to main menu
  w     # write changes
EOF

LOOP_DEV=$(sudo losetup -f --show $DISK_NAME)
sudo partprobe $LOOP_DEV

PARTITION1=${LOOP_DEV}p1
PARTITION2=${LOOP_DEV}p2
PARTITION3=${LOOP_DEV}p3

sudo mkfs.ext2 $PARTITION2

# Create the mount point directory if it doesn't exist
sudo mkdir -p $MOUNT_DIR

sudo mount $PARTITION2 $MOUNT_DIR

sudo cp -r ${SYSROOT}/* ${MOUNT_DIR}/
sudo mkdir -p ${MOUNT_DIR}/usr/share/
sudo cp -r fonts ${MOUNT_DIR}/usr/share/

sudo grub-install --no-floppy --target=i386-pc --modules="normal ext2 multiboot" --boot-directory=${MOUNT_DIR}/boot $LOOP_DEV

sudo cp assets/frogbg.jpg ${MOUNT_DIR}/boot/grub/background.jpg

echo -e '
set timeout=5
set default=0

set color_normal=light-gray/black
set color_highlight=light-green/green

if loadfont /boot/grub/fonts/unicode.pf2; then
    set gfxmode=auto
    insmod gfxterm
    insmod vbe
    terminal_output gfxterm
fi

insmod jpeg
background_image /boot/grub/background.jpg

menuentry "frog-os" {
      set gfxpayload=keep
	multiboot /boot/frog-os.kernel
}
menuentry "frog-os (no serial)" {
      set gfxpayload=keep
	multiboot /boot/frog-os.kernel noserial
}
menuentry "frog-os (no apic)" {
      set gfxpayload=keep
	multiboot /boot/frog-os.kernel noapic
}
menuentry "frog-os (no apic, no serial)" {
      set gfxpayload=keep
	multiboot /boot/frog-os.kernel noapic noserial
}

menuentry "Exit" {
      halt
}

'  | sudo tee ${MOUNT_DIR}/boot/grub/grub.cfg

sudo umount $MOUNT_DIR

sudo mkfs.ext2 $PARTITION3
sudo mount $PARTITION3 $MOUNT_DIR
echo 'hello' | sudo tee ${MOUNT_DIR}/hello.txt
sudo umount $MOUNT_DIR

sudo losetup -d $LOOP_DEV
