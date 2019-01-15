/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef COLOSSALSQUIG_H
#define COLOSSALSQUIG_H

#include <Unit.h>
#include <Weapon.h>

namespace GloomspiteGitz
{

class ColossalSquig : public Unit
{
public:
    static const int BASESIZE = 120; // oval
    static const int WOUNDS = 16;

    ColossalSquig();
    ~ColossalSquig() override
    {
        delete m_pEnormousJaws;
        delete m_pTramplingFeet;
    }

    int move() const override;

    void charge(PlayerId player) override;

    bool configure();

protected:

    void onWounded() override;
    void onSlain() override;
    int generateMortalWounds(const Weapon* weapon, const Unit* unit, const Hits& hits) const override;

    int getDamageTableIndex() const;

private:

    Weapon* m_pEnormousJaws = nullptr;
    Weapon* m_pTramplingFeet = nullptr;

    static Weapon s_puffSpores,
        s_enormousJaws,
        s_tramplingFeet;
};

} // namespace GloomspiteGitz

#endif //COLOSSALSQUIG_H