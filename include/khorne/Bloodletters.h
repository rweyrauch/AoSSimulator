/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef BLOODLETTERS_H
#define BLOODLETTERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Khorne
{

class Bloodletters : public Unit
{
public:
    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 10;
    static const int MAX_UNIT_SIZE = 30;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    Bloodletters();
    ~Bloodletters() override
    {
        delete m_pHellblade;
        delete m_pHellbladeReaper;
    }

    bool configure(int numModels, bool iconBearer, bool standardBearer, bool hornblowers);

protected:

    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

private:

    bool m_iconBearer = false;
    bool m_standarBearer = false;
    bool m_hornblower = false;

    Weapon* m_pHellblade = nullptr;
    Weapon* m_pHellbladeReaper = nullptr;

    static Weapon s_hellblade;

    static bool s_registered;
};

} // namespace Khorne

#endif //BLOODLETTERS_H