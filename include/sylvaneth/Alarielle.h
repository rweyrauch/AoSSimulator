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
    ~Alarielle() override
    {
        delete m_pSpearOfKurnoth;
        delete m_pBeetleGreatAntlers;
    }

    bool configure();
    int move() const override;
    void hero(PlayerId player) override;

protected:

    int toHitModifier(const Weapon* weapon, const Unit* unit) const override;
    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon* m_pSpearOfKurnoth = nullptr;
    Weapon* m_pBeetleGreatAntlers = nullptr;

    static Weapon s_spearOfKurnoth,
        s_talonOfDwindling,
        s_beetleGreatAntlers;
};

} // namespace Sylvaneth

#endif //ALARIELLE_H