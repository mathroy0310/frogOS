/* ************************************************************************** */
/*                                                             _              */
/*                                                 __   ___.--'_\`.           */
/*   en_ca.h                                      ( _\`.' -   'o\` )          */
/*                                                _\\.'_'      _.-'           */
/*   By: mathroy0310 <maroy0310@gmail.com>       ( \`. )    //\\\`            */
/*                                                \\_'-`---'\\__,             */
/*   Created: 2024/08/05 01:45:17 by mathroy0310   \`        `-\\             */
/*   Updated: 2024/08/09 09:40:38 by mathroy0310    `                         */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <kernel/Input.h>

namespace Input {
constexpr Key scan_code_to_key_extended[0xFF]{
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::PreviousTrack, Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::NextTrack,     Key::INVALID,
    Key::INVALID,    Key::NumpadEnter,   Key::RightCtrl,
    Key::INVALID,    Key::INVALID,       Key::Mute,
    Key::Calculator, Key::PlayPause,     Key::INVALID,
    Key::Stop,       Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::VolumeDown,    Key::INVALID,
    Key::VolumeUp,   Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::NumpadDiv,
    Key::INVALID,    Key::INVALID,       Key::RightAlt,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::Home,
    Key::Up,         Key::PageUp,        Key::INVALID,
    Key::Left,       Key::INVALID,       Key::Right,
    Key::INVALID,    Key::End,           Key::Down,
    Key::PageDown,   Key::Insert,        Key::Delete,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::Super,         Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,    Key::INVALID,       Key::INVALID,
    Key::INVALID,
};

constexpr Key scan_code_to_key_altgr[0xFF]{
    Key::INVALID, Key::Escape,   Key::None,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::Backspace,   Key::Tab,
    Key::None,    Key::None,     Key::Euro,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::Enter,   Key::LeftCtrl, Key::None,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::LeftShift,   Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::None,        Key::None,
    Key::None,    Key::None,     Key::RightShift,  Key::NumpadMult,
    Key::LeftAlt, Key::Space,    Key::CapsLock,    Key::F1,
    Key::F2,      Key::F3,       Key::F4,          Key::F5,
    Key::F6,      Key::F7,       Key::F8,          Key::F9,
    Key::F10,     Key::NumLock,  Key::ScrollLock,  Key::Numpad7,
    Key::Numpad8, Key::Numpad9,  Key::NumpadMinus, Key::Numpad4,
    Key::Numpad5, Key::Numpad6,  Key::NumpadPlus,  Key::Numpad1,
    Key::Numpad2, Key::Numpad3,  Key::Numpad0,     Key::NumpadSep,
    Key::INVALID, Key::INVALID,  Key::BackSlash,   Key::F11,
    Key::F12,
};

constexpr Key scan_code_to_key_shift[0xFF]{
    Key::INVALID,    Key::Escape,      Key::ExclamationMark,
    Key::At,         Key::Hashtag,     Key::Dollar,
    Key::Percent,    Key::Caret,       Key::Ampersand,
    Key::Asterix,    Key::OpenParen,   Key::CloseParen,
    Key::Underscore, Key::Plus,        Key::Backspace,
    Key::Tab,        Key::Q,           Key::W,
    Key::E,          Key::R,           Key::T,
    Key::Y,          Key::U,           Key::I,
    Key::O,          Key::P,           Key::OpenBrace,
    Key::CloseBrace, Key::Enter,       Key::LeftCtrl,
    Key::A,          Key::S,           Key::D,
    Key::F,          Key::G,           Key::H,
    Key::J,          Key::K,           Key::L,
    Key::Colon,      Key::DoubleQuote, Key::Tilde,
    Key::LeftShift,  Key::Pipe,        Key::Z,
    Key::X,          Key::C,           Key::V,
    Key::B,          Key::N,           Key::M,
    Key::LessThan,   Key::MoreThan,    Key::QuestionMark,
    Key::RightShift, Key::NumpadMult,  Key::LeftAlt,
    Key::Space,      Key::CapsLock,    Key::F1,
    Key::F2,         Key::F3,          Key::F4,
    Key::F5,         Key::F6,          Key::F7,
    Key::F8,         Key::F9,          Key::F10,
    Key::NumLock,    Key::ScrollLock,  Key::Home,
    Key::Up,         Key::PageUp,      Key::NumpadMinus,
    Key::Left,       Key::None,        Key::Right,
    Key::NumpadPlus, Key::End,         Key::Down,
    Key::PageDown,   Key::Insert,      Key::Delete,
    Key::INVALID,    Key::INVALID,     Key::INVALID,
    Key::F11,        Key::F12,
};

constexpr Key scan_code_to_key_normal[0xFF]{
    Key::INVALID,     Key::Escape,   Key::_1,          Key::_2,
    Key::_3,          Key::_4,       Key::_5,          Key::_6,
    Key::_7,          Key::_8,       Key::_9,          Key::_0,
    Key::Hyphen,      Key::Equals,   Key::Backspace,   Key::Tab,
    Key::Q,           Key::W,        Key::E,           Key::R,
    Key::T,           Key::Y,        Key::U,           Key::I,
    Key::O,           Key::P,        Key::OpenBracket, Key::CloseBracket,
    Key::Enter,       Key::LeftCtrl, Key::A,           Key::S,
    Key::D,           Key::F,        Key::G,           Key::H,
    Key::J,           Key::K,        Key::L,           Key::Semicolon,
    Key::SingleQuote, Key::BackTick, Key::LeftShift,   Key::BackSlash,
    Key::Z,           Key::X,        Key::C,           Key::V,
    Key::B,           Key::N,        Key::M,           Key::Comma,
    Key::Period,      Key::Slash,    Key::RightShift,  Key::NumpadMult,
    Key::LeftAlt,     Key::Space,    Key::CapsLock,    Key::F1,
    Key::F2,          Key::F3,       Key::F4,          Key::F5,
    Key::F6,          Key::F7,       Key::F8,          Key::F9,
    Key::F10,         Key::NumLock,  Key::ScrollLock,  Key::Numpad7,
    Key::Numpad8,     Key::Numpad9,  Key::NumpadMinus, Key::Numpad4,
    Key::Numpad5,     Key::Numpad6,  Key::NumpadPlus,  Key::Numpad1,
    Key::Numpad2,     Key::Numpad3,  Key::Numpad0,     Key::NumpadSep,
    Key::INVALID,     Key::INVALID,  Key::BackSlash,   Key::F11,
    Key::F12,
};

} // namespace Input