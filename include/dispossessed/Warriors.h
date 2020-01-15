/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef WARRIORS_H
#define WARRIORS_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Warriors : public Dispossessed
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 40;
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 280;

    enum WeaponOptions
    {
        DuardinAxeOrHammer,
        DoubleHandedDuardinAxe
    };

    enum StandardOptions
    {
        None,
        RunicIcon,
        ClanBanner
    };

    static Unit* Create(const ParameterList& parameters);
    static std::string ValueToString(const Parameter &parameter);
    static int EnumStringToInt(const std::string &enumString);
    static int ComputePoints(int numModels);
    static void Init();

    Warriors();
    ~Warriors() override = default;

    bool configure(int numModels, WeaponOptions weapons, bool duardinShields, StandardOptions standard, bool hornblowers);

protected:

    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    Rerolls toWoundRerolls(const Weapon *weapon, const Unit *target) const override;
    void onStartCombat(PlayerId player) override;
    int rollRunDistance() const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;

private:

    bool m_duardinShields = false;
    StandardOptions m_standard = None;
    bool m_hornblowers = false;

    bool m_opponentsCombat = false;

    Weapon m_duardinAxeOrHammer,
        m_duardinAxeOrHammerVeteran,
        m_doubleHandedAxe,
        m_doubleHandedAxeVeteran;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Resolute in Defence              Yes
// Duardin Shields                  Yes
// Runic Icon                       No
// Clan Banner                      Yes
// Hornblowers                      Yes
//

} // namespace Dispossessed

#endif //WARRIORS_H