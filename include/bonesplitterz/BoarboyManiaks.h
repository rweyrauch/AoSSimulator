/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#ifndef BOARBOYMANIAKS_H
#define BOARBOYMANIAKS_H

#include "Unit.h"

namespace Bonesplitterz
{

class SavageBoarboyManiaks : public Unit
{
public:

    static const int BASESIZE = 32;
    static const int WOUNDS = 3;
    static const int MIN_UNIT_SIZE = 5;
    static const int MAX_UNIT_SIZE = 20;
    static const int POINTS_PER_BLOCK = 140;
    static const int POINTS_MAX_UNIT_SIZE = POINTS_PER_BLOCK * 4;

    static Unit* Create(const ParameterList& parameters);
    static void Init();

    SavageBoarboyManiaks();
    ~SavageBoarboyManiaks() override = default;

    bool configure(int numModels, bool boarThumper, bool totemBearer);

    void visitWeapons(std::function<void(const Weapon*)>& visitor) override;

protected:


private:

    Weapon m_chompas,
        m_tusksAndHooves,
        m_chompasBoss;

    static bool s_registered;
};

//
// TODO: abilities
// Abilities                    Implemented
// -------------------------------------------
// Thumper                          No
// Totem Bearer                     No
// Maniak Fury                      No
// Tusker Charge                    No
//

} // namespace Bonesplitterz

#endif //BOARBOYMANIAKS_H