/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LORDARCANUMONTAURALON_H
#define LORDARCANUMONTAURALON_H

#include <stormcast/StormcastEternals.h>
#include <stormcast/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class LordArcanumOnTauralon : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        LordArcanumOnTauralon();

        ~LordArcanumOnTauralon() override = default;

        bool configure(Lore lore);

    protected:

        void onCharged() override;

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

    private:

        bool m_shatteredFlasks = false;

        Weapon m_aetherstave,
                m_hornsAndHooves;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Comet Trail                      TODO
// Cycle of the Storm               TODO
// Meteoric Strike                  Yes
// Spirit Flask                     Yes
// Lightning Orb                    Yes
// Prime Electrids                  Yes
// Swift of Wing                    TODO

} // namespace StormcastEternals

#endif //LORDARCANUMONTAURALON_H