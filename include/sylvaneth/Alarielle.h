/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef ALARIELLE_H
#define ALARIELLE_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class Alarielle : public Unit
{
public:

    static const int BASESIZE = 160;
    static const int WOUNDS = 16;

    Alarielle();
    ~Alarielle() override = default;

    bool configure();

    void hero(PlayerId player) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;

    int getDamageTableIndex() const;
private:

    static Weapon s_spearOfKurnoth,
        s_talonOfDwindling,
        s_beetleGreatAntlers;
};

} // namespace Sylvaneth

#endif //ALARIELLE_H