/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "kharadron/BjorgenThundrik.h"

namespace KharadronOverlords {
    static const int g_basesize = 40;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 0;

    bool BjorgenThundrik::s_registered = false;

    Unit *BjorgenThundrik::Create(const ParameterList &parameters) {
        auto unit = new BjorgenThundrik();

        unit->setSkyport(Skyport::Barak_Nar);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int BjorgenThundrik::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

    void BjorgenThundrik::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    BjorgenThundrik::Create,
                    KharadronBase::ValueToString,
                    KharadronBase::EnumStringToInt,
                    BjorgenThundrik::ComputePoints,
                    {
                        BoolParameter("General")
                    },
                    ORDER,
                    {KHARADRON_OVERLORDS}
            };
            s_registered = UnitFactory::Register("Bjorgen Thundrik", factoryMethod);
        }
    }

    BjorgenThundrik::BjorgenThundrik() :
            KharadronBase("Bjorgen Thundrik", 4, g_wounds, 7, 4, false),
            m_anatomiser(Weapon::Type::Missile, "Atmospheric Anatomiser", 9, RAND_3D6, 4, 4, -2, 1),
            m_instruments(Weapon::Type::Melee, "Heavy Instruments", 1, 3, 4, 4, 0, 1) {
        m_keywords = {ORDER, DUARDIN, KHARADRON_OVERLORDS, BARAK_NAR, HERO, SKYFARER, MARINE,
                      AETHER_KHEMIST, BJORGEN_THUNDRIK};
        m_weapons = {&m_anatomiser, &m_instruments};
        m_battleFieldRole = Leader;
    }

    bool BjorgenThundrik::configure() {
        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_anatomiser);
        model->addMeleeWeapon(&m_instruments);
        addModel(model);

        m_points = BjorgenThundrik::ComputePoints(1);

        return true;
    }

} // namespace KharadronOverlords
