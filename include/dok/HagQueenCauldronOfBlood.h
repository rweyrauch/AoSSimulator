/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HAGQUEENCAULDRON_H
#define HAGQUEENCAULDRON_H

#include <dok/DaughterOfKhaine.h>
#include <Weapon.h>

namespace DaughtersOfKhaine {

    class HagQueenOnCauldronOfBlood : public DaughterOfKhaine {
    public:

        static Unit *Create(const ParameterList &parameters);

        static int ComputePoints(int numModels);

        static void Init();

        HagQueenOnCauldronOfBlood();

        ~HagQueenOnCauldronOfBlood() override;

        bool configure();

    protected:

        void onWounded() override;

        void onRestore() override;

        void onCharged() override;

        int idolOfWorship(const Unit *unit);

        int bloodShield(const Unit *target, const Weapon *weapon);

    private:

        int getDamageTableIndex() const;

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
// Witchbrew                        TODO
// Priestess of Khaine              TODO
// Wrath of Khaine                  TODO
// Animated                         TODO
// Idol of Worship                  Yes
//

} // namespace DaughtersOfKhaine

#endif //HAGQUEENCAULDRON_H