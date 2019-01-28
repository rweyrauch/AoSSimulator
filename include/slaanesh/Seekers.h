/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SEEKERS_H
#define SEEKERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Slaanesh
{

class Seekers : public Unit
{
public:

    static const int BASESIZE = 60; // x32 oval
    static const int WOUNDS = 2;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 120;
    static const int POINTS_MAX_UNIT_SIZE = 480;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Seekers();
    ~Seekers() override = default;

    bool configure(int numModels, bool iconBearer, bool standardBearer, bool hornblower);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    int runModifier() const override;
    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    Hits applyHitModifiers(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

protected:

    bool m_iconBearer = false;
    bool m_standardBearer = false;
    bool m_hornblower = false;

private:

    static Weapon s_piercingClaws,
        s_piercingClawsHeartseeker,
        s_poisonedTongue;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Sadistic Killers                 Yes
// Locus of Swiftness               No
// Quicksilver Speed                Yes

} // Slannesh

#endif //SEEKERS_H