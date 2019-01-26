/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPITEREVENANTS_H
#define SPITEREVENANTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class SpiteRevenants : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;
    static const int POINTS_PER_BLOCK = 70;
    static const int POINTS_MAX_UNIT_SIZE = 360;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SpiteRevenants();
    ~SpiteRevenants() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    static Weapon s_cruelTalonsAndFangs,
        s_cruelTalonsAndFangsShadestalker;

    static bool s_registered;
};

} // namespace Sylvaneth

#endif //SPITEREVENANTS_H