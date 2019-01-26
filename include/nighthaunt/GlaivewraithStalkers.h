/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef GLAIVEWRAITHSTALKERS_H
#define GLAIVEWRAITHSTALKERS_H

#include <Unit.h>
#include <Weapon.h>

namespace Nighthaunt
{

class GlaivewraithStalkers : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 1;
    static const int MIN_UNIT_SIZE = 4;
    static const int MAX_UNIT_SIZE = 16;
    static const int POINTS_PER_BLOCK = 60;
    static const int POINTS_MAX_UNIT_SIZE = 320;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    GlaivewraithStalkers();
    ~GlaivewraithStalkers() override = default;

    bool configure(int numModels, bool drummer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    bool m_drummer = true;

    Rerolls toHitRerolls(const Weapon* weapon, const Unit* unit) const override;
    int toSaveModifier(const Weapon* weapon) const override;

private:

    static Weapon s_huntersGlaive;

    static bool s_registered;
};

} // namespace Nighthaunt

#endif // GLAIVEWRAITHSTALKERS_H
