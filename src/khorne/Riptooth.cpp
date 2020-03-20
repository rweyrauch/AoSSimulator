/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/Riptooth.h>
#include <UnitFactory.h>

namespace Khorne {
    static const int BASESIZE = 32;
    static const int WOUNDS = 2;
    static const int POINTS_PER_UNIT = 40;

    bool Riptooth::s_registered = false;

    Unit *Riptooth::Create(const ParameterList &parameters) {
        auto unit = new Riptooth();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void Riptooth::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Riptooth::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    Riptooth::ComputePoints,
                    {
                            {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None,
                             KhorneBase::SkullfiendTribe, 1}
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Riptooth", factoryMethod);
        }
    }

    Riptooth::Riptooth() :
            KhorneBase("Riptooth", 8, WOUNDS, 10, 5, false),
            m_claws(Weapon::Type::Melee, "Blood-dark Claws", 1, 4, 3, 4, 0, 1) {
        m_keywords = {CHAOS, DAEMON, GORETIDE, KHORNE, FLESH_HOUND, RIPTOOTH};
        m_weapons = {&m_claws};

        // Collar of Khorne
        m_totalUnbinds = 1;
    }

    bool Riptooth::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int Riptooth::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Khorne
