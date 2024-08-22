/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   VesaTerminalDriver.cpp                       ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/12 03:01:24 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/13 00:22:27 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#include <FROG/Errors.h>
#include <kernel/Debug.h>
#include <kernel/MMU.h>
#include <kernel/VesaTerminalDriver.h>
#include <kernel/multiboot.h>

extern const struct bitmap_font font;

VesaTerminalDriver *VesaTerminalDriver::create() {
	if (!(g_multiboot_info->flags & MULTIBOOT_FLAGS_FRAMEBUFFER)) {
		dprintln("Bootloader did not provide framebuffer");
		return nullptr;
	}

	auto &framebuffer = g_multiboot_info->framebuffer;

	if (framebuffer.type == MULTIBOOT_FRAMEBUFFER_TYPE_GRAPHICS) {
		if (framebuffer.bpp != 24 && framebuffer.bpp != 32) {
			dprintln("Unsupported bpp {}", framebuffer.bpp);
			return nullptr;
		}
		dprintln("Graphics Mode {}x{} ({} bpp)", framebuffer.width, framebuffer.height, framebuffer.bpp);
	} else if (framebuffer.type == MULTIBOOT_FRAMEBUFFER_TYPE_TEXT) {
		dprintln("Text Mode is currently not supported");
		return nullptr;
	} else {
		dprintln("Unknown framebuffer type {}", framebuffer.type);
		return nullptr;
	}

	MMU::get().allocate_range(framebuffer.addr, framebuffer.pitch * framebuffer.height);

	auto *driver = new VesaTerminalDriver(framebuffer.width, framebuffer.height, framebuffer.pitch,
	                                      framebuffer.bpp, framebuffer.addr, font);
	driver->set_cursor_position(0, 0);
	driver->clear(TerminalColor::BLACK);
	return driver;
}

VesaTerminalDriver::~VesaTerminalDriver() {
	MMU::get().unallocate_range(m_address, m_pitch * m_height);
}

void VesaTerminalDriver::set_pixel(uint32_t offset, Color color) {
	uint32_t *pixel = (uint32_t *) (m_address + offset);
	switch (m_bpp) {
	case 24:
		*pixel = (*pixel & 0xFF000000) | (color.rgb & 0x00FFFFFF);
		break;
	case 32:
		*pixel = color.rgb;
		break;
	}
}

void VesaTerminalDriver::putchar_at(uint16_t ch, uint32_t x, uint32_t y, Color fg, Color bg) {
	uint32_t glyph_index = 0;
	for (uint32_t i = 0; i < m_font.Chars; i++) {
		if (m_font.Index[i] == ch) {
			glyph_index = i;
			break;
		}
	}

	const uint8_t *glyph = m_font.Bitmap + glyph_index * m_font.Height;

	x *= m_font.Width;
	y *= m_font.Height;

	uint32_t row_offset = y * m_pitch + x * m_bpp / 8;
	for (uint32_t dy = 0; dy < m_font.Height && y + dy < m_height; dy++) {
		uint32_t pixel_offset = row_offset;
		for (uint32_t dx = 0; dx < m_font.Width && x + dx < m_width; dx++) {
			uint8_t bitmask = 1 << (font.Width - dx - 1);
			set_pixel(pixel_offset, glyph[dy] & bitmask ? fg : bg);
			pixel_offset += m_bpp / 8;
		}
		row_offset += m_pitch;
	}
}

void VesaTerminalDriver::clear(Color color) {
	if (m_bpp == 32) {
		uint32_t cells_per_row = m_pitch / 4;
		for (uint32_t y = 0; y < m_height; y++)
			for (uint32_t x = 0; x < m_width; x++)
				((uint32_t *) m_address)[y * cells_per_row + x] = color.rgb;
		return;
	}

	uint32_t row_offset = 0;
	for (uint32_t y = 0; y < m_height; y++) {
		uint32_t pixel_offset = row_offset;
		for (uint32_t x = 0; x < m_width; x++) {
			set_pixel(pixel_offset, color);
			pixel_offset += m_bpp / 8;
		}
		row_offset += m_pitch;
	}
}

void VesaTerminalDriver::set_cursor_position(uint32_t x, uint32_t y) {
	ASSERT(m_font.Height == 16 && m_font.Width == 8);
	constexpr uint8_t cursor[] = {
	    ________, ________, ________, ________, ________, ________, ________, ________,
	    ________, ________, ________, ________, ________, XXXXXXXX, XXXXXXXX, ________,
	};

	x *= m_font.Width;
	y *= m_font.Height;

	uint32_t row_offset = y * m_pitch + x * m_bpp / 8;
	for (uint32_t dy = 0; dy < m_font.Height && y + dy < m_height; dy++) {
		uint32_t pixel_offset = row_offset;
		for (uint32_t dx = 0; dx < m_font.Width && x + dx < m_width; dx++) {
			uint8_t bitmask = 1 << (font.Width - dx - 1);
			if (cursor[dy] & bitmask) set_pixel(pixel_offset, s_cursor_color);
			pixel_offset += m_bpp / 8;
		}
		row_offset += m_pitch;
	}
}