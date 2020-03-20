/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "bonesplitterz/ManiakWeirdnob.h"

namespace Bonesplitterz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 120;

    bool ManiakWeirdnob::s_registered = false;

    Unit *ManiakWeirdnob::Create(const ParameterList &parameters) {
        auto unit = new ManiakWeirdnob();

        auto warclan = (Warclan) GetEnumParam("Warclan", parameters, Bonesplitterz::Bonegrinz);
        unit->setWarclan(warclan);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ManiakWeirdnob::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Bonesplitterz::ValueToString,
                    Bonesplitterz::EnumStringToInt,
                    ComputePoints,
                    {
                            {ParamType::Enum, "Warclan", Bonesplitterz::Bonegrinz, Bonesplitterz::Bonegrinz,
                             Bonesplitterz::Icebone, 1},
                    },
                    DESTRUCTION,
                    {BONESPLITTERZ}
            };

            s_registered = UnitFactory::Register("Maniak Weirdnob", factoryMethod);
        }
    }

    ManiakWeirdnob::ManiakWeirdnob() :
            Bonesplitterz("Maniak Weirdnob", 12, WOUNDS, 7, 6, false),
            m_bonebeastStaff(Weapon::Type::Melee, "Bonebeast Staff", 1, 3, 4, 3, 0, RAND_D3),
            m_tusksAndHooves(Weapon::Type::Melee, "Tusks and Hooves", 1, 2, 4, 4, 0, 1) {
        m_keywords = {DESTRUCTION, ORRUK, BONESPLITTERZ, HERO, WIZARD, MANIAK_WEIRDNOB};
        m_weapons = {&m_bonebeastStaff, &m_tusksAndHooves};

        m_totalSpells = 1;
        m_totalUnbinds = 1;
    }

    bool ManiakWeirdnob::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_bonebeastStaff);
        model->addMeleeWeapon(&m_tusksAndHooves);

        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = POINTS_PER_UNIT;

        return true;
    }

    int ManiakWeirdnob::ComputePoints(int numModels) {
        return POINTS_PER_UNIT;
    }

} // namespace Bonesplitterz
