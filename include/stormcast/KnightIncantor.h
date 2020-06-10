/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KNIGHTINCANTOR_H
#define KNIGHTINCANTOR_H

#include <stormcast/StormcastEternals.h>
#include <spells/LoreOfTheStorm.h>
#include <Weapon.h>

namespace StormcastEternals {

    class KnightIncantor : public StormcastEternal {
    public:

        static Unit *Create(const ParameterList &parameters);

        static void Init();

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        static int ComputePoints(int numModels);

        KnightIncantor();

        ~KnightIncantor() override = default;

        bool configure(Lore lore);

    protected:

        void onStartCombat(PlayerId player) override;

        void onRestore() override { m_shatteredFlasks = false; }

    private:

        bool m_shatteredFlasks = false;

        Weapon m_staff;

        static bool s_registered;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Voidstorm Scroll                 TODO
// Spirit Flask                     Yes
// Spirit Storm                     Yes
//

} // namespace StormcastEternals

#endif //KNIGHTINCANTOR_H