/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Bloodstoker.h>
#include <UnitFactory.h>
#include <Board.h>

namespace Khorne {
    static const int BASESIZE = 40;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 80;

    bool Bloodstoker::s_registered = false;

    Bloodstoker::Bloodstoker() :
            KhorneBase("Bloodstoker", 6, WOUNDS, 8, 4, false),
            m_tortureBlade(Weapon::Type::Melee, "Torture Blade", 1, 3, 3, 3, 0, 1),
            m_bloodWhip(Weapon::Type::Melee, "Blood Whip", 3, 3, 3, 4, 0, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, BLOODSTOKER};
        m_weapons = {&m_tortureBlade, &m_bloodWhip};
    }

    bool Bloodstoker::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_tortureBlade);
        model->addMeleeWeapon(&m_bloodWhip);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Bloodstoker::Create(const ParameterList &parameters) {
        auto unit = new Bloodstoker();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Bloodstoker::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Bloodstoker::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Bloodstoker::ComputePoints,
                    {
                            {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None,
                             KhorneBase::SkullfiendTribe, 1}
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Bloodstoker", factoryMethod);
        }
    }

    int Bloodstoker::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne