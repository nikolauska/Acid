#include "ButtonCompound.hpp"

namespace acid
{
	ButtonCompound::ButtonCompound(const std::vector<std::shared_ptr<IButton>> &buttons) :
		m_buttons(buttons),
		m_wasDown(false)
	{
	}

	ButtonCompound::~ButtonCompound()
	{
	}

	bool ButtonCompound::IsDown() const
	{
		for (auto &button : m_buttons)
		{
			if (button->IsDown())
			{
				return true;
			}
		}

		return false;
	}

	bool ButtonCompound::WasDown()
	{
		bool stillDown = m_wasDown && IsDown();
		m_wasDown = IsDown();
		return m_wasDown == !stillDown;
	}
}
