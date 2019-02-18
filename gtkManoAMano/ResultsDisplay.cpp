/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "ResultsDisplay.h"

ResultsDisplay::ResultsDisplay()
{
}

ResultsDisplay::~ResultsDisplay()
{
}

bool ResultsDisplay::build(Glib::RefPtr<Gtk::Builder>& builder)
{
    builder->get_widget("redPoints", m_redPoints);
    builder->get_widget("bluePoints", m_bluePoints);
    builder->get_widget("redVictoryPercent", m_redVictories);
    builder->get_widget("blueVictoryPercent", m_blueVictories);
    builder->get_widget("tiesPercent", m_ties);

    return true;
}

void ResultsDisplay::setResults(int redPoints, int bluePoints, int numRounds, int numIterations, float redVictories, float blueVictories, float ties)
{
    m_redPoints->set_text(std::to_string(redPoints));
    m_bluePoints->set_text(std::to_string(bluePoints));
    m_redVictories->set_text(std::to_string(redVictories));
    m_blueVictories->set_text(std::to_string(blueVictories));
    m_ties->set_text(std::to_string(ties));
}
