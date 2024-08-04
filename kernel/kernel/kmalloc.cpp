/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   kmalloc.cpp                                  ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/04 01:34:31 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/04 01:34:32 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <kernel/kmalloc.h>
#include <kernel/kprint.h>
#include <kernel/multiboot.h>
#include <kernel/panic.h>

#include <stdint.h>
#include <string.h>

#define MB (1 << 20)

struct kmalloc_info_t {
	static constexpr size_t total_size = 1 * MB;
	void                   *base_addr = (void *) 0x00200000;
	size_t                  used;
};
static kmalloc_info_t s_kmalloc_info;

void kmalloc_initialize() {
	if (!(s_multiboot_info->flags & (1 << 6)))
		Kernel::panic("Bootloader didn't give a memory map");

	bool valid = false;
	for (size_t i = 0; i < s_multiboot_info->mmap_length;) {
		multiboot_memory_map_t *mmmt =
		    (multiboot_memory_map_t *) (s_multiboot_info->mmap_addr + i);

		if (mmmt->type == 1) {
			char  *ptr1 = (char *) mmmt->base_addr;
			char  *ptr2 = (char *) s_kmalloc_info.base_addr;
			size_t len1 = mmmt->length;
			size_t len2 = s_kmalloc_info.total_size;

			if (ptr1 <= ptr2 && ptr1 + len1 >= ptr2 + len2) {
				valid = true;
				break;
			}
		}

		i += mmmt->size + sizeof(uint32_t);
	}

	if (!valid)
		Kernel::panic("Could not find enough space for kmalloc");

	memset(s_kmalloc_info.base_addr, 0, s_kmalloc_info.total_size);

	s_kmalloc_info.used = 0;
}

void *kmalloc(size_t size) {
	if (s_kmalloc_info.total_size - s_kmalloc_info.used < size)
		Kernel::panic("Out of kernel memory");

	char *result = (char *) s_kmalloc_info.base_addr + s_kmalloc_info.used;
	s_kmalloc_info.used += size;
	return (void *) result;
}

void kfree(void *) {
}