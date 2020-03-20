/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Neferata.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 340;

    bool NeferataMortarchOfBlood::s_registered = false;

    Unit *NeferataMortarchOfBlood::Create(const ParameterList &parameters) {
        auto unit = new NeferataMortarchOfBlood();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int NeferataMortarchOfBlood::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void NeferataMortarchOfBlood::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DEATH,
                    {SOULBLIGHT, DEATHLORDS}
            };
            s_registered = UnitFactory::Register("Neferata, Mortarch of Blood", factoryMethod);
        }
    }

    NeferataMortarchOfBlood::NeferataMortarchOfBlood() :
            LegionOfNagashBase("Neferata, Mortarch of Blood", 16, WOUNDS, 10, 4, true),
            m_akmetHar(Weapon::Type::Melee, "Akmet-har", 1, 5, 2, 3, -1, 1),
            m_akenSeth(Weapon::Type::Melee, "Aken-seth", 1, 2, 2, 3, -2, 2),
            m_skeletalClaws(Weapon::Type::Melee, "Nagadron's Skeletal Claws", 1, 6, 4, 3, -2, 2),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spirits' Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, NEFERATA};
        m_weapons = {&m_akmetHar, &m_akenSeth, &m_skeletalClaws, &m_clawsAndDaggers};
    }

    bool NeferataMortarchOfBlood::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_akmetHar);
        model->addMeleeWeapon(&m_akenSeth);
        model->addMeleeWeapon(&m_skeletalClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void NeferataMortarchOfBlood::onWounded() {
        Unit::onWounded();
    }

    void NeferataMortarchOfBlood::onRestore() {
        Unit::onRestore();
    }

    int NeferataMortarchOfBlood::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
