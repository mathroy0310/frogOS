/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MMU.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/12 18:05:24 by mathroy0310       #+#    #+#             */
/*   Updated: 2024/08/30 16:19:31 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <kernel/MMU.h>
#include <kernel/kmalloc.h>

#define PAGE_SIZE 0x1000
#define PAGE_MASK ~(PAGE_SIZE - 1)

#define CLEANUP_STRUCTURE(s)                          \
	for (uint64_t i = 0; i < 512; i++)                \
		if (s[i] & Flags::Present) goto cleanup_done; \
	kfree(s)

static MMU *s_instance = nullptr;

void MMU::initialize() {
	ASSERT(s_instance == nullptr);
	s_instance = new MMU();
}

MMU &MMU::get() {
	ASSERT(s_instance);
	return *s_instance;
}

static uint64_t *allocate_page_aligned_page() {
	void *page = kmalloc(PAGE_SIZE, PAGE_SIZE);
	ASSERT(page);
	memset(page, 0, PAGE_SIZE);
	return (uint64_t *) page;
}

MMU::MMU() {
	// Identity map from 4 KiB -> 6 MiB
	m_highest_paging_struct = allocate_page_aligned_page();

	uint64_t *pdpt = allocate_page_aligned_page();
	m_highest_paging_struct[0] = (uint64_t) pdpt | Flags::ReadWrite | Flags::Present;

	uint64_t *pd = allocate_page_aligned_page();
	pdpt[0] = (uint64_t) pd | Flags::ReadWrite | Flags::Present;

	for (uint32_t i = 0; i < 3; i++) {
		uint64_t *pt = allocate_page_aligned_page();
		for (uint64_t j = 0; j < 512; j++)
			pt[j] = (i << 21) | (j << 12) | Flags::ReadWrite | Flags::Present;
		pd[i] = (uint64_t) pt | Flags::ReadWrite | Flags::Present;
	}

	// Unmap 0 -> 4 KiB
	uint64_t *pt1 = (uint64_t *) (pd[0] & PAGE_MASK);
	pt1[0] = 0;

	// Load the new pml4
	asm volatile("movq %0, %%cr3" ::"r"(m_highest_paging_struct));
}

MMU::~MMU() {
	uint64_t *pml4 = m_highest_paging_struct;
	for (uint32_t pml4e = 0; pml4e < 512; pml4e++) {
		if (!(pml4[pml4e] & Flags::Present)) continue;
		uint64_t *pdpt = (uint64_t *) (pml4[pml4e] & PAGE_MASK);
		for (uint32_t pdpte = 0; pdpte < 512; pdpte++) {
			if (!(pdpt[pdpte] & Flags::Present)) continue;
			uint64_t *pd = (uint64_t *) (pdpt[pdpte] & PAGE_MASK);
			for (uint32_t pde = 0; pde < 512; pde++) {
				if (!(pd[pde] & Flags::Present)) continue;
				kfree((void *) (pd[pde] & PAGE_MASK));
			}
			kfree(pd);
		}
		kfree(pdpt);
	}
	kfree(pml4);
}

void MMU::allocate_page(uintptr_t address, uint8_t flags) {
	ASSERT((address >> 48) == 0);

	ASSERT(flags & Flags::Present);
	bool should_invalidate = false;

	address &= PAGE_MASK;

	uint64_t pml4e = (address >> 39) & 0x1FF;
	uint64_t pdpte = (address >> 30) & 0x1FF;
	uint64_t pde = (address >> 21) & 0x1FF;
	uint64_t pte = (address >> 12) & 0x1FF;

	uint64_t *pml4 = m_highest_paging_struct;
	if ((pml4[pml4e] & flags) != flags) {
		if (!(pml4[pml4e] & Flags::Present)) pml4[pml4e] = (uint64_t) allocate_page_aligned_page();
		pml4[pml4e] = (pml4[pml4e] & PAGE_MASK) | flags;
		should_invalidate = true;
	}

	uint64_t *pdpt = (uint64_t *) (pml4[pml4e] & PAGE_MASK);
	if ((pdpt[pdpte] & flags) != flags) {
		if (!(pdpt[pdpte] & Flags::Present)) pdpt[pdpte] = (uint64_t) allocate_page_aligned_page();
		pdpt[pdpte] = (pdpt[pdpte] & PAGE_MASK) | flags;
		should_invalidate = true;
	}

	uint64_t *pd = (uint64_t *) (pdpt[pdpte] & PAGE_MASK);
	if ((pd[pde] & flags) != flags) {
		if (!(pd[pde] & Flags::Present)) pd[pde] = (uint64_t) allocate_page_aligned_page();
		pd[pde] = (pd[pde] & PAGE_MASK) | flags;
		should_invalidate = true;
	}

	uint64_t *pt = (uint64_t *) (pd[pde] & PAGE_MASK);
	if ((pt[pte] & flags) != flags) {
		pt[pte] = address | flags;
		should_invalidate = true;
	}

	if (should_invalidate) asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}

void MMU::allocate_range(uintptr_t address, ptrdiff_t size, uint8_t flags) {
	uintptr_t s_page = address & PAGE_MASK;
	uintptr_t e_page = (address + size - 1) & PAGE_MASK;
	for (uintptr_t page = s_page; page <= e_page; page += PAGE_SIZE)
		allocate_page(page, flags);
}

void MMU::unallocate_page(uintptr_t address) {
	ASSERT((address >> 48) == 0);

	address &= PAGE_MASK;

	uint64_t pml4e = (address >> 39) & 0x1FF;
	uint64_t pdpte = (address >> 30) & 0x1FF;
	uint64_t pde = (address >> 21) & 0x1FF;
	uint64_t pte = (address >> 12) & 0x1FF;

	uint64_t *pml4 = m_highest_paging_struct;
	if (!(pml4[pml4e] & Flags::Present)) return;

	uint64_t *pdpt = (uint64_t *) (pml4[pml4e] & PAGE_MASK);
	if (!(pdpt[pdpte] & Flags::Present)) return;

	uint64_t *pd = (uint64_t *) (pdpt[pdpte] & PAGE_MASK);
	if (!(pd[pde] & Flags::Present)) return;

	uint64_t *pt = (uint64_t *) (pd[pde] & PAGE_MASK);
	if (!(pt[pte] & Flags::Present)) return;

	pt[pte] = 0;

	CLEANUP_STRUCTURE(pt);
	pd[pde] = 0;
	CLEANUP_STRUCTURE(pd);
	pdpt[pdpte] = 0;
	CLEANUP_STRUCTURE(pdpt);
	pml4[pml4e] = 0;
cleanup_done:

	asm volatile("invlpg (%0)" ::"r"(address) : "memory");
}

void MMU::unallocate_range(uintptr_t address, ptrdiff_t size) {
	uintptr_t s_page = address & PAGE_MASK;
	uintptr_t e_page = (address + size - 1) & PAGE_MASK;
	for (uintptr_t page = s_page; page <= e_page; page += PAGE_SIZE)
		unallocate_page(page);
}