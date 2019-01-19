/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef TREEREVENANTS_H
#define TREEEREVENANTS_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class TreeRevenants : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 30;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    TreeRevenants();
    ~TreeRevenants() override = default;

    bool configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes);

protected:


private:

    static Weapon s_enchantedBlade,
        s_enchantedBladeScion,
        s_protectorGlaive;

    static bool s_registered;
};

} // namespace Sylvaneth

#endif //TREEREVENANTS_H