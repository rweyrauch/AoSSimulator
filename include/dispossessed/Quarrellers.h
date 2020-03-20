/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef QUARRELLERS_H
#define QUARRELLERS_H

#include <dispossessed/Dispossessed.h>
#include <Weapon.h>

namespace Dispossessed
{

class Quarrellers : public Dispossessed
{
public:

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

    Quarrellers();
    ~Quarrellers() override = default;

    bool configure(int numModels, bool duardinBucklers, StandardOptions standard, bool drummer);

protected:

    int extraAttacks(const Model *attackingModel, const Weapon *weapon, const Unit *target) const override;
    Rerolls toSaveRerolls(const Weapon *weapon) const override;
    int rollRunDistance() const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;

private:

    bool m_duardinBucklers = false;
    StandardOptions m_standard = None;
    bool m_drummer = false;

    Weapon m_duardinCrossbow,
        m_duardinCrossbowVeteran,
        m_rangersAxe;

    static bool s_registered;
};

//
// Abilities                    Implemented
// -------------------------------------------
// Volley Fire                      Yes
// Runic Icon                       TODO
// Clan Banner                      Yes
// Duardin Bucklers                 Yes
// Drummers                         Yes
//

} // namespace Dispossessed

#endif //QUARRELLERS_H