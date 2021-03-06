﻿#include "UiInputText.hpp"

#include "Inputs/Keyboard.hpp"
#include "Maths/Visual/DriverSlide.hpp"
#include "Scenes/Scenes.hpp"

namespace acid
{
	const float UiInputText::CHANGE_TIME = 0.1f;
	const float UiInputText::FONT_SIZE = 1.7f;
	const Vector2 UiInputText::DIMENSION = Vector2(0.36f, 0.05f);
	const float UiInputText::SCALE_NORMAL = 1.0f;
	const float UiInputText::SCALE_SELECTED = 1.1f;

	UiInputText::UiInputText(UiObject *parent, const Vector3 &position, const std::string &prefix, const std::string &value, const int32_t &maxLength) :
		UiObject(parent, UiBound(Vector2(0.5f, 0.5f), "Centre", true, true, Vector2(1.0f, 1.0f))),
		m_text(new Text(this, UiBound(position, "Centre", true), FONT_SIZE, prefix + value, FontType::Resource("Fonts/ProximaNova", "Regular"), JUSTIFY_CENTRE, DIMENSION.m_x)),
		m_background(new Gui(this, UiBound(position, "Centre", true, true, DIMENSION), Texture::Resource("Guis/Button.png"))),
		m_prefix(prefix),
		m_value(value),
		m_maxLength(maxLength),
		m_inputDelay(UiInputDelay()),
		m_lastKey(0),
		m_selected(false),
		m_mouseOver(false),
		m_actionChange(nullptr)
	{
		m_background->SetColourOffset(Colour());
	}

	UiInputText::~UiInputText()
	{
		delete m_text;
		delete m_background;
	}

	void UiInputText::UpdateObject()
	{
		if (m_selected)
		{
			int32_t key = Keyboard::Get()->GetChar();

			if (m_value.length() < m_maxLength && key != 0 && Keyboard::Get()->GetKey((Key) toupper(key)))
			{
				m_inputDelay.Update(true);

				if (m_lastKey != key || m_inputDelay.CanInput())
				{
					m_value += static_cast<char>(key);
					m_text->SetString(m_prefix + m_value);

					if (m_actionChange != nullptr)
					{
						m_actionChange();
					}

					m_lastKey = key;
				}
			}
			else if (Keyboard::Get()->GetKey(KEY_BACKSPACE))
			{
				m_inputDelay.Update(true);

				if (m_lastKey != 8 || m_inputDelay.CanInput())
				{
					m_value = m_value.substr(0, m_value.length() - 1);
					m_text->SetString(m_prefix + m_value);

					if (m_actionChange != nullptr)
					{
						m_actionChange();
					}

					m_lastKey = 8;
				}
			}
			else if (Keyboard::Get()->GetKey(KEY_ENTER) && m_lastKey != 13)
			{
				m_inputDelay.Update(true);

				m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
				m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
				m_selected = false;
			}
			else
			{
				m_inputDelay.Update(false);
				m_lastKey = 0;
			}
		}

		// Click updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && GetAlpha() == 1.0f &&
			Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT))
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_selected = true;

			Uis::Get()->GetSelector().CancelWasEvent();
		}
		else if (Uis::Get()->GetSelector().WasDown(MOUSE_BUTTON_LEFT) && m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_selected = false;
		}

		// Mouse over updates.
		if (Uis::Get()->GetSelector().IsSelected(*m_text) && !m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_SELECTED, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_SELECTED, CHANGE_TIME);
			m_mouseOver = true;
		}
		else if (!Uis::Get()->GetSelector().IsSelected(*m_text) && m_mouseOver && !m_selected)
		{
			m_background->SetScaleDriver<DriverSlide>(m_background->GetScale(), SCALE_NORMAL, CHANGE_TIME);
			m_text->SetScaleDriver<DriverSlide>(m_text->GetScale(), FONT_SIZE * SCALE_NORMAL, CHANGE_TIME);
			m_mouseOver = false;
		}
	}

	void UiInputText::SetPrefix(const std::string &prefix)
	{
		m_prefix = prefix;
		m_text->SetString(prefix + m_value);
	}

	void UiInputText::SetValue(const std::string &value)
	{
		m_value = value;
		m_text->SetString(m_prefix + value);
	}
}
