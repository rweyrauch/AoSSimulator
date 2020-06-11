/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SLAANESHBASE_H
#define SLAANESHBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Slaanesh {

    enum Host {
        Invaders = 0,
        Pretenders,
        Godseekers,
    };

    const std::array<int, 3> g_host = {Invaders,Pretenders,Godseekers};

    class SlaaneshBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SlaaneshBase() = default;

        ~SlaaneshBase() override = default;

        void setHost(Host host);

    protected:
        SlaaneshBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

        int chargeModifier() const override;

    protected:

        Host m_host = Godseekers;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Locus of Diversion               TODO
// Euphoric Killers                 Yes
// Heir to the Throne               Yes
// Thundering Cavalcade             Yes
//

    void Init();

} // namespace Slaanesh

#endif //SLAANESHBASE_H