﻿#pragma once

#include <Uis/UiObject.hpp>
#include <Maths/Timer.hpp>
#include <Fonts/Text.hpp>

using namespace acid;

namespace test
{
	class OverlayDebug :
		public UiObject
	{
	private:
		Text *m_textInfo;
		Text *m_textFps;
		Text *m_textUps;
		Timer m_timerUpdate;
	public:
		OverlayDebug(UiObject *parent);

		~OverlayDebug();

		virtual void UpdateObject() override;

	private:
		Text *CreateStatus(const std::string &content, const float &positionX, const float &positionY, const TextJustify &justify);
	};
}
