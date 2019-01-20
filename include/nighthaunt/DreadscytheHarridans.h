/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef DREADSCYTHEHARRIDANS_H
#define DREADSCYTHEHARRIDANS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nighthaunt
{

class DreadscytheHarridans : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    DreadscytheHarridans();
    ~DreadscytheHarridans() override = default;

    bool configure(int numModels);

protected:

    int toSaveModifier(const Weapon* weapon) const override;

private:

    static Weapon s_scythedLimbs,
        s_scythedLimbsCrone;

    static bool s_registered;
};

} // namespace Nighthaunt

#endif // DREADSCYTHEHARRIDANS_H