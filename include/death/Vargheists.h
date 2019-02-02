/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef VARGHEISTS_H
#define VARGHEISTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Death
{
class Vargheists : public Unit
{
public:

    static const int BASESIZE = 50;
    static const int WOUNDS = 4;
    static const int MIN_UNIT_SIZE = 3;
    static const int MAX_UNIT_SIZE = 12;
    static const int POINTS_PER_BLOCK = 160;
    static const int POINTS_MAX_UNIT_SIZE = 640;

    static Unit* Create(const ParameterList& parameters);

    static void Init();

    Vargheists();
    ~Vargheists() override = default;

    bool configure(int numModels);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

private:

    static Weapon s_ancientBlade,
        s_ancientBladeChampion,
        s_ancientSpear,
        s_ancientSpearChampion;

    static bool s_registered;

};

} //namespace Death

#endif //VARGHEISTS_H