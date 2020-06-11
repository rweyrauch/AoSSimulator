/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BEASTSOFCHAOSBASE_H
#define BEASTSOFCHAOSBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace BeastsOfChaos {

    enum class Greatfray : int {
        None,
        Allherd,
        Darkwalkers,
        Gavespawn
    };

    const std::array<int, 4> g_greatFray = {
        (int)Greatfray::None,
        (int)Greatfray::Allherd,
        (int)Greatfray::Darkwalkers,
        (int)Greatfray::Gavespawn
    };

    class BeastsOfChaosBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        BeastsOfChaosBase() = default;

        ~BeastsOfChaosBase() override = default;

        void setGreatfray(Greatfray fray);

    protected:
        BeastsOfChaosBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Wounds onEndCombat(PlayerId player) override;

    protected:

        Greatfray m_greatfray = Greatfray::None;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Brayherd Ambush                  TODO
// Creatures of the Storm           TODO
// Bloodgorge                       Yes
// Primordial Call                  TODO
// Bestial Might                    TODO
// Booming Roar                     TODO
// Dominator                        TODO
// Shadowbeasts                     TODO
// Nomadic Leader                   TODO
// Savage Encirclement              TODO
// Gift of Morghur                  TODO
// Propagator of Devolution         TODO
// Unravelling Aura                 TODO
//

    void Init();

} // namespace BeastsOfChaos

#endif //BEASTSOFCHAOSBASE_H