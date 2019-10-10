/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "bonesplitterz/BoarboyManiaks.h"

namespace Bonesplitterz
{
static FactoryMethod factoryMethod = {
    SavageBoarboyManiaks::Create,
    nullptr,
    nullptr,
    {
    },
    DESTRUCTION,
    BONESPLITTERZ
};

bool SavageBoarboyManiaks::s_registered = false;

Unit *SavageBoarboyManiaks::Create(const ParameterList &parameters)
{
    return nullptr;
}

void SavageBoarboyManiaks::Init()
{
    if (!s_registered)
    {
        s_registered = UnitFactory::Register("Savage Boarboy Maniaks", factoryMethod);
    }
}

SavageBoarboyManiaks::SavageBoarboyManiaks() :
    Unit("Savage Boarboy Maniaks", 12, WOUNDS, 6, 6, false),
    m_chompas(Weapon::Type::Melee, "Pair of Chompas", 1, 4, 4, 3, 0, 1),
    m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1),
    m_chompasBoss(Weapon::Type::Melee, "Pair of Chompas (Boss)", 1, 5, 4, 3, 0, 1)
{
    m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, BOARBOYS, SAVAGE_BOARBOY_MANIAKS};
}

bool SavageBoarboyManiaks::configure(int numModels, bool boarThumper, bool totemBearer)
{
    return false;
}

void SavageBoarboyManiaks::visitWeapons(std::function<void(const Weapon *)> &visitor)
{
    visitor(&m_chompas);
    visitor(&m_chompasBoss);
    visitor(&m_tusksAndHooves);
}

} // namespace Bonesplitterz
