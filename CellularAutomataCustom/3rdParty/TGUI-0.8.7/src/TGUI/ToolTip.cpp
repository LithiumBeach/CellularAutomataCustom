/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2019 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/ToolTip.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Time ToolTip::m_initialDelay = sf::milliseconds(500);
    Vector2f ToolTip::m_distanceToMouse = {5, 20};

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TGUI_REMOVE_DEPRECATED_CODE
    void ToolTip::setTimeToDisplay(const sf::Time& timeToDisplay)
    {
        m_initialDelay = timeToDisplay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Time ToolTip::getTimeToDisplay()
    {
        return m_initialDelay;
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ToolTip::setInitialDelay(const sf::Time& delay)
    {
        m_initialDelay = delay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::Time ToolTip::getInitialDelay()
    {
        return m_initialDelay;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ToolTip::setDistanceToMouse(const Vector2f& distance)
    {
        m_distanceToMouse = distance;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ToolTip::getDistanceToMouse()
    {
        return m_distanceToMouse;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
