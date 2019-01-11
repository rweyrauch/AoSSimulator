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

    static const int BASESIZE = 105;
    static const int WOUNDS = 12;

    SpiritOfDurthu();
    ~SpiritOfDurthu() override = default;

    bool configure();

    void hero(PlayerId id) override;

protected:

    int getDamageTableIndex() const;

private:

    static Weapon s_verdantBlast,
        s_guardianSword,
        s_massiveImpalingTalons;
};

} // namespace Sylvaneth

#endif //SPIRITOFDURTHU_H