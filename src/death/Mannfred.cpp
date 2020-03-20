/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/Mannfred.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 120; // x92 oval
    static const int WOUNDS = 11;
    static const int POINTS_PER_UNIT = 340;

    bool MannfredMortarchOfNight::s_registered = false;

    Unit *MannfredMortarchOfNight::Create(const ParameterList &parameters) {
        auto unit = new MannfredMortarchOfNight();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int MannfredMortarchOfNight::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void MannfredMortarchOfNight::Init() {
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
            s_registered = UnitFactory::Register("Mannfred, Mortarch of Night", factoryMethod);
        }
    }

    MannfredMortarchOfNight::MannfredMortarchOfNight() :
            LegionOfNagashBase("Mannfred, Mortarch of Night", 16, WOUNDS, 10, 4, true),
            m_gheistvor(Weapon::Type::Melee, "Gheistvor", 1, 4, 3, 3, -1, RAND_D3),
            m_glaive(Weapon::Type::Melee, "Sickle-glaive", 2, 2, 3, 3, -1, 2),
            m_ebonClaws(Weapon::Type::Melee, "Ashigaroth's Ebon Claws", 1, 6, 4, 3, -2, 2),
            m_clawsAndDaggers(Weapon::Type::Melee, "Spirits' Spectral Claws and Daggers", 1, 6, 5, 4, 0, 1) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, DEATHLORDS, MONSTER, HERO, WIZARD, MORTARCH, MANNFRED};
        m_weapons = {&m_gheistvor, &m_glaive, &m_ebonClaws, &m_clawsAndDaggers};
    }

    bool MannfredMortarchOfNight::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_gheistvor);
        model->addMeleeWeapon(&m_glaive);
        model->addMeleeWeapon(&m_ebonClaws);
        model->addMeleeWeapon(&m_clawsAndDaggers);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void MannfredMortarchOfNight::onWounded() {
        Unit::onWounded();
    }

    void MannfredMortarchOfNight::onRestore() {
        Unit::onRestore();
    }

    int MannfredMortarchOfNight::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
