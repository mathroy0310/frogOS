# **************************************************************************** #
#                                                              _               #
#                                                  __   ___.--'_\`.            #
#    iso.sh                                       ( _\`.' -   'o\` )           #
#                                                 _\\.'_'      _.-'            #
#    By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`             #
#                                                 \\_'-`---'\\__,              #
#    Created: 2024/08/04 01:33:04 by mathroy0310   \`        `-\\              #
#    Updated: 2024/08/12 18:26:39 by mathroy0310    `                          #
#                                                                              #
# **************************************************************************** #

#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir/boot/grub
 
cp sysroot/boot/frog-os.kernel isodir/boot/frog-os.kernel
cat > isodir/boot/grub/grub.cfg << EOF
set timeout=5
set default=0

set color_normal=light-gray/black
set color_highlight=light-green/green

menuentry "frog-os" {
	multiboot /boot/frog-os.kernel
}
menuentry "frog-os (no apic)" {
	multiboot /boot/frog-os.kernel noapic
}
menuentry "frog-os (no serial)" {
	multiboot /boot/frog-os.kernel noserial
}
menuentry "frog-os (no apic, no serial)" {
	multiboot /boot/frog-os.kernel noapic noserial
}
menuentry "Reboot" {
	echo "Rebooting..."
	reboot
}
EOF
grub-mkrescue -o frog-os.iso isodir
rm -rf isodir
