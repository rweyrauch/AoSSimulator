/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class HagQueenOnCauldronOfBlood : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(const ParameterList& parameters);

        static void Init();

        HagQueenOnCauldronOfBlood(Temple temple, Prayer prayer, CommandTrait trait, Artefact artefact, bool isGeneral);

        ~HagQueenOnCauldronOfBlood() override;

        void animate(bool animated);

        bool isAnimated() const;

    protected:

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        void onStartHero(PlayerId player) override;

        int idolOfWorship(const Unit *unit);

        int bloodShield(const Unit *target, const Weapon *weapon);

    private:

        size_t getDamageTableIndex() const;

        Weapon m_burningBlood,
                m_knives,
                m_blade,
                m_sword;

        lsignal::slot m_idolSlot, m_bloodshieldSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Bladed Impact                    Yes
// Bloodshield                      Yes
// Witchbrew                        Yes
// Priestess of Khaine              Yes
//    Rune of Khaine                Yes
//    Touch of Death                Yes
//    Wrath of Khaine               Yes
// Animated                         Yes
// Idol of Worship                  Yes
//

} // namespace DaughtersOfKhaine
