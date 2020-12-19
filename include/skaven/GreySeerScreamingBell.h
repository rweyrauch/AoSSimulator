/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GREYSEERSCREAMINGBELL_H
#define GREYSEERSCREAMINGBELL_H

#include <skaven/Skaventide.h>
#include <Weapon.h>

namespace Skaven {

    class GreySeerOnScreamingBell : public Skaventide {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        GreySeerOnScreamingBell();

        ~GreySeerOnScreamingBell() override;

        bool configure(Lore lore);

    protected:

        Wounds applyWoundSave(const Wounds &wounds, Unit* attackingUnit) override;

        void onWounded() override;

        void onRestore() override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        void onStartHero(PlayerId player) override;

        int getDamageTableIndex() const;

        int altarOfTheHornedRat(const Unit *unit);

        int wallOfUnholySound(const Unit *attacker, const Weapon *weapon, const Unit *unit);

    private:

        Weapon m_staff,
                m_clawsAndFangs,
                m_spikes;

        lsignal::slot m_connection;
        lsignal::slot m_unholySoundConnection;
        bool m_unholySoundActive = false;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Altar of the Horned Rat          Yes
// Protection of the Horned Rat     Yes
// Pushed into Battle               Partial/TODO
// Peal of Doom
//    Magical Backlash              Yes
//    Unholy Clamour                Yes
//    Deafening Peals               Yes
//    Avalanche of Energy           Yes
//    Apocalyptic Doom              Yes
//    Wall of Unholy Sound          Yes
//    A Stirring Beyond the Veil    Partial
// Cracks Call                      TODO
//

} // namespace Skaven

#endif //GREYSEERSCREAMINGBELL_H