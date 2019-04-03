/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef FLESHEATERCOURTS_H
#define FLESHEATERCOURTS_H

#include <Unit.h>
#include <Weapon.h>

namespace FleshEaterCourt
{

class FleshEaterCourts : public Unit
{
public:

    enum Delusion
    {
        None,
        CrusadingArmy,
        TheRoyalHunt,
        TheFeastDay,
        AMatterOfHonour,
        TheGrandTournament,
        DefendersOfTheRealm
    };

    FleshEaterCourts() = default;
    ~FleshEaterCourts() override = default;

    void setCourtsOfDelusion(Delusion delusion)
    {
        m_delusion = delusion;
    }

protected:
    FleshEaterCourts(const std::string& name, int move, int wounds, int bravery, int save, bool fly) :
        Unit(name, move, wounds, bravery, save, fly) {}

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    int moveModifier() const override;
    int runModifier() const override;
    int chargeModifier() const override;
    Wounds applyWoundSave(const Wounds &wounds) override;

protected:

    Delusion m_delusion = None;
};

void Init();

} // namespace FleshEaterCourt

#endif // FLESHEATERCOURTS_H