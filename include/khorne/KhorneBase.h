/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef KHORNEBASE_H
#define KHORNEBASE_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>
#include <array>

namespace Khorne {

    enum SlaughterHost {
        None = 0,
        ReapersOfVengeance,
        Bloodlords,
        Goretide,
        SkullfiendTribe
    };

    const std::array<int, 5> g_slaughterHost = {
            None,
            ReapersOfVengeance,
            Bloodlords,
            Goretide,
            SkullfiendTribe
    };

    class KhorneBase : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        KhorneBase() = default;

        ~KhorneBase() override = default;

        void setSlaughterHost(SlaughterHost host);

    protected:
        KhorneBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *unit) const override;

        Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;

    protected:

        SlaughterHost m_slaughterHost = None;
    };

    void Init();

//
// Abilities                    Implemented
// -------------------------------------------
// Locus of Fury                    Yes
// Devour the Craven                TODO
// Skull Hunters                    Yes
// Tireless Conquerors              Yes
// Slay the Mighty                  Yes
// Blood for the Blood God          TODO
//    Bloody Exemplar               TODO
//    Spelleater Curse              TODO
//    Murderlust                    TODO
//    Apoplectic Frenzy             TODO
//    Brass Skull Meteor            TODO
//    Relentless Fury               TODO
//    Crimson Rain                  TODO
//    Slaughter Triumphant          TODO
// Summon Daemons of Khorne         TODO
//

} // namespace Khorne

#endif // KHORNEBASE_H