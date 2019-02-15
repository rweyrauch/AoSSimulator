/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DAEMONETTES_H
#define DAEMONETTES_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class Daemonettes : public Unit
{
public:

    static const int BASESIZE = 25;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 100;
    static const int POINTS_MAX_UNIT_SIZE = 270;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Daemonettes();
    ~Daemonettes() override = default;

    bool configure(int numModels, bool iconBearer, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const override;
    void computeBattleshockEffect(int roll, int& numFled, int& numAdded) const override;
    void restoreModels(int numModels) override;

protected:

    bool m_iconBearer = false;
    bool m_standardBearer = false;
    bool m_hornblower = false;

private:

    Weapon m_piercingClaws,
        m_piercingClawsAlluress;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Icon Bearers                     Yes
// Standard Bearers                 Yes
// Hornblower                       No
// Sadistic Killers                 Yes
// Lithe and Swift                  Yes
// Locus of Excruciation            No
//

} // Slannesh

#endif //DAEMONETTES_H