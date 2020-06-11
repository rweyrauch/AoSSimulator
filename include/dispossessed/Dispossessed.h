/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DISPOSSESSED_H
#define DISPOSSESSED_H

#include <Unit.h>
#include <UnitFactory.h>
#include <Weapon.h>

namespace Dispossessed {

    enum class Grudge : int {
        Stuck_Up,
        Speed_Merchants,
        Monstrous_Cheaters,
        Cowardly_Horders,
        Shoddy_Craftsmanship,
        Sneaky_Ambushers
    };

    class Dispossessed : public Unit {
    public:

        static std::string ValueToString(const Parameter &parameter);

        static int EnumStringToInt(const std::string &enumString);

        Dispossessed() = default;

        ~Dispossessed() override = default;

        void setGrudge(Grudge grudge);

    protected:
        Dispossessed(const std::string &name, int move, int wounds, int bravery, int save, bool fly) :
                Unit(name, move, wounds, bravery, save, fly) {}

        void computeBattleshockEffect(int roll, int &numFled, int &numAdded) const override;

        Rerolls toHitRerolls(const Weapon *weapon, const Unit *target) const override;

    private:

        Grudge m_grudge = Grudge::Stuck_Up;
    };

//
// Abilities                    Implemented
// -------------------------------------------
// Stubborn to the End              Yes
// Grudgebound:
//      Stuck-up                    Yes
//      Speed Merchants             Yes
//      Monstrous Cheaters          Yes
//      Cowardly Hordes             Yes
//      Shoddy Craftsmanship        Yes
//      Sneaky Ambushers            TODO
//

    void Init();

} // namespace Dispossessed

#endif // DISPOSSESSED_H