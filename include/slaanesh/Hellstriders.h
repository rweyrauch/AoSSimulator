/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef HELLSTRIDERS_H
#define HELLSTRIDERS_H

#include <slaanesh/SlaaneshBase.h>
#include <Weapon.h>

namespace Slaanesh {

    class Hellstriders : public SlaaneshBase {
    public:

        enum WeaponOption {
            Claw_Spear,
            Hellscourge
        };

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        Hellstriders();

        ~Hellstriders() override;

        bool configure(int numModels, WeaponOption weapons, bool iconBearer, bool bannerBearer, bool hornblower);

    protected:

        Rerolls chargeRerolls() const override;

        int braveryModifier() const override;

        Rerolls hornblowerBattleshockReroll(const Unit *unit);

    private:

        Weapon m_clawSpear,
                m_clawSpearReaver,
                m_hellscourge,
                m_hellscourgeReaver,
                m_poisonedTongue;

        lsignal::slot m_hornblowerSlot;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Standard Banner                  Yes
// Icon Banner                      Yes
// Hornblower                       Yes
// Soul Hunters                     TODO
//
} // Slannesh

#endif //HELLSTRIDERS_H