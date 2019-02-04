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
    static const int POINTS_PER_BLOCK = 80;
    static const int POINTS_MAX_UNIT_SIZE = 420;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    TreeRevenants();
    ~TreeRevenants() override = default;

    bool configure(int numModels, bool scionGlaive, bool gladeBanners, bool waypipes);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    bool m_gladeBanners = false;
    bool m_waypipes = false;

private:

    Weapon m_enchantedBlade,
        m_enchantedBladeScion,
        m_protectorGlaive;

    static bool s_registered;
};

} // namespace Sylvaneth

#endif //TREEREVENANTS_H