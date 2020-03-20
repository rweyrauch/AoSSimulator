/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SERAPHON_H
#define SERAPHON_H

#include <Unit.h>
#include <Weapon.h>

namespace Seraphon {

    class SeraphonBase : public Unit {
    public:

        enum WayOfTheSeraphon {
            Starborne,
            Coalesced
        };

        enum Constellation {
            None,
            KoatlsClaw,        // Coalesced
            ThunderLizard,     // Coalesced
            DracothionsTail,   // Starborne
            FangsOfSotek,      // Starborne
        };

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        SeraphonBase() = default;

        ~SeraphonBase() override = default;

        void setWayOfTheSeraphon(WayOfTheSeraphon way, Constellation constellation);

    protected:

        SeraphonBase(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

    protected:

        int moveModifier() const override;

        int toHitModifier(const Weapon *weapon, const Unit *target) const override;

        int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;

        int braveryModifier() const override;

        Wounds targetAttackDamageModifier(const Wounds &wounds, const Unit *attacker, int hitRoll,
                                          int woundRoll) const override;

        int woundModifier() const override;

    protected:

        WayOfTheSeraphon m_way = Coalesced;
        Constellation m_constellation = ThunderLizard;

    };

//
// Abilities                    Implemented
// -------------------------------------------
// Contemplations of the Ancient Ones   TODO
// Sacred Asterisms                 TODO
//   The Great Drake                TODO
//   The Hunter's Steed             TODO
//   The Sage's Staff               TODO
// Cold Blooded                     TODO
// Predatory Fighters               Yes
// Primeval Domain                  TODO
// Scaly Skin                       Yes
// Unfeeling                        Yes
// Celestial Conjuration            TODO
// Lords of Space and Time          TODO
// Appear on Command                TODO
// First to Battle                  Yes
// Savagery Incarnate               Yes
// Mighty Beasts of War             Yes
//

    void Init();

} // namespace Seraphon

#endif //SERAPHON_H