/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <death/PrinceVhordrai.h>
#include <UnitFactory.h>

namespace Death {
    static const int BASESIZE = 130;
    static const int WOUNDS = 14;
    static const int POINTS_PER_UNIT = 480;

    bool PrinceVhordrai::s_registered = false;

    Unit *PrinceVhordrai::Create(const ParameterList &parameters) {
        auto unit = new PrinceVhordrai();

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int PrinceVhordrai::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

    void PrinceVhordrai::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LegionOfNagashBase::ValueToString,
                    LegionOfNagashBase::EnumStringToInt,
                    ComputePoints,
                    {
                    },
                    DEATH,
                    {SOULBLIGHT}
            };
            s_registered = UnitFactory::Register("Prince Vhordrai", factoryMethod);
        }
    }

    PrinceVhordrai::PrinceVhordrai() :
            LegionOfNagashBase("Prince Vhordrai", 14, WOUNDS, 10, 3, true),
            m_bloodlance(Weapon::Type::Melee, "Bloodlance", 2, 4, 3, 3, -2, 2),
            m_maw(Weapon::Type::Melee, "Shordemaire's Maw", 3, 3, 4, 3, -2, RAND_D6),
            m_claws(Weapon::Type::Melee, "Shordemaire's Sword-like Claws", 2, 7, 4, 3, -1, 2) {
        m_keywords = {DEATH, VAMPIRE, SOULBLIGHT, ZOMBIE_DRAGON, MONSTER, HERO, WIZARD, PRINCE_VHORDRAI};
        m_weapons = {&m_bloodlance, &m_maw, &m_claws};
    }

    bool PrinceVhordrai::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bloodlance);
        model->addMeleeWeapon(&m_maw);
        model->addMeleeWeapon(&m_claws);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    void PrinceVhordrai::onWounded() {
        Unit::onWounded();
    }

    void PrinceVhordrai::onRestore() {
        Unit::onRestore();
    }

    int PrinceVhordrai::getDamageTableIndex() const {
        return 0;
    }
} // namespace Death
