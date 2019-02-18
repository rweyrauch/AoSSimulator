/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef RESULTSDISPLAY_H
#define RESULTSDISPLAY_H

#include <gtkmm.h>

class ResultsDisplay
{
public:
    ResultsDisplay();
    ~ResultsDisplay();

    bool build(Glib::RefPtr<Gtk::Builder>& builder);

    void setResults(int redPoints, int bluePoints, int numRounds, int numIterations,
        float redVictories, float blueVictories, float ties);

protected:
    Gtk::Label* m_redPoints = nullptr;
    Gtk::Label* m_bluePoints = nullptr;
    Gtk::Label* m_redVictories = nullptr;
    Gtk::Label* m_blueVictories = nullptr;
    Gtk::Label* m_ties = nullptr;
};

#endif //RESULTSDISPLAY_H
