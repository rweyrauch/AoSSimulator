/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#ifndef SPIRITOFDURTHU_H
#define SPIRITOFDURTHU_H

#include <Unit.h>
#include <Weapon.h>

namespace Sylvaneth
{

class SpiritOfDurthu : public Unit
{
public:

    static const int BASESIZE = 105; // x70 oval
    static const int WOUNDS = 12;
    static const int POINTS_PER_UNIT = 380;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SpiritOfDurthu();
    ~SpiritOfDurthu() override = default;

    bool configure();

    void hero(PlayerId id) override;

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:

    void onWounded() override;
    int getDamageTableIndex() const;

private:

    Weapon m_verdantBlast,
        m_guardianSword,
        m_massiveImpalingTalons;

    static bool s_registered;
};

} // namespace Sylvaneth

#endif //SPIRITOFDURTHU_H