/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Font.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maroy <maroy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 01:56:58 by maroy             #+#    #+#             */
/*   Updated: 2024/09/20 01:49:29 by maroy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <FROG/HashMap.h>
#include <FROG/Span.h>
#include <FROG/StringView.h>

namespace Kernel {

class Font {
  public:
	static FROG::ErrorOr<Font> load(FROG::StringView);
	static FROG::ErrorOr<Font> prefs();

	uint32_t width() const { return m_width; }
	uint32_t height() const { return m_height; }
	uint32_t pitch() const { return m_pitch; }

	bool           has_glyph(uint32_t) const;
	const uint8_t *glyph(uint32_t) const;

  private:
	static FROG::ErrorOr<Font> parse_psf1(FROG::Span<const uint8_t>);
	static FROG::ErrorOr<Font> parse_psf2(FROG::Span<const uint8_t>);

  private:
	FROG::HashMap<uint32_t, uint32_t> m_glyph_offsets;
	FROG::Vector<uint8_t>             m_glyph_data;
	uint32_t                          m_width = 0;
	uint32_t                          m_height = 0;
	uint32_t                          m_pitch = 0;
};

} // namespace Kernel