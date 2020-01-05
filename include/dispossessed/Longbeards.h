/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef LONGBEARDS_H
#define LONGBEARDS_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Longbeards : public Dispossessed
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 270;

    enum WeaponOptions
    {
        AncestralAxesOrHammers,
        AncestralGreatAxe
    };

    static Unit* Create(const ParameterList& parameters);

    static std::string ValueToString(const Parameter &parameter);

    static int EnumStringToInt(const std::string &enumString);

    static void Init();

    Longbeards();
    ~Longbeards() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool gromrilShields, bool standardBearer, bool musician);

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int rollRunDistance() const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;

private:

    bool m_gromrilShields = false;
    bool m_standardBearer = false;
    bool m_musician = false;

    Weapon m_ancestralAxeHammer,
        m_ancestralGreatAxe,
        m_ancestralAxeHammerOldGuard,
        m_ancestralGreatAxeOldGuard;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Gromril Shields                  Yes
// Old Grumblers                    No
// Standard Bearer                  Yes
// Musician                         Yes
//

} // namespace Dispossessed

#endif //LONGBEARDS_H