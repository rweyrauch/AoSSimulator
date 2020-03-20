/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/KorghosKhul.h>
#include <UnitFactory.h>

namespace Khorne {
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 180;

    bool KorghosKhul::s_registered = false;

    KorghosKhul::KorghosKhul() :
            KhorneBase("Korghos Khul", 5, WOUNDS, 9, 3, false),
            m_axeOfKhorne(Weapon::Type::Melee, "Axe of Khorne", 1, 3, 3, 3, -1, RAND_D3),
            m_clawsAndFangs(Weapon::Type::Melee, "Claws and Fangs", 1, 4, 3, 4, -1, 1) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, GORETIDE, HERO, MIGHTY_LORD_OF_KHORNE, KORGHOS_KHUL};
        m_weapons = {&m_axeOfKhorne, &m_clawsAndFangs};

        // Collar of Khorne
        m_totalUnbinds = 1;

        // Aqshy's Bane
        m_pileInMove = 8;
    }

    bool KorghosKhul::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_axeOfKhorne);
        model->addMeleeWeapon(&m_clawsAndFangs);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *KorghosKhul::Create(const ParameterList &parameters) {
        auto unit = new KorghosKhul();

        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, KhorneBase::None);
        unit->setSlaughterHost(host);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void KorghosKhul::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KorghosKhul::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    KorghosKhul::ComputePoints,
                    {
                            {ParamType::Enum, "Slaughter Host", KhorneBase::None, KhorneBase::None,
                             KhorneBase::SkullfiendTribe, 1}
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Korghos Khul", factoryMethod);
        }
    }

    Rerolls KorghosKhul::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        // Favoured of Khorne
        return RerollFailed;
    }

    int KorghosKhul::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }


} // namespace Khorne