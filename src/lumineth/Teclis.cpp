/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include "lumineth/Teclis.h"
#include <UnitFactory.h>
#include <spells/MysticShield.h>

namespace LuminethRealmLords {

    static const int BASESIZE = 100;
    static const int WOUNDS = 16;
    static const int POINTS_PER_UNIT = 660;

    bool ArchmageTeclis::s_registered = false;

    Unit *ArchmageTeclis::Create(const ParameterList &parameters) {
        auto unit = new ArchmageTeclis();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int ArchmageTeclis::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void ArchmageTeclis::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    LuminethBase::ValueToString,
                    LuminethBase::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    ORDER,
                    {LUMINETH_REALM_LORDS}
            };

            s_registered = UnitFactory::Register("Archmage Teclis", factoryMethod);
        }
    }

    ArchmageTeclis::ArchmageTeclis() :
            LuminethBase("Archmage Teclis", 14, WOUNDS, 10, 3, true) {
    }

    bool ArchmageTeclis::configure() {
        auto model = new Model(BASESIZE, wounds());
        addModel(model);

        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = ComputePoints(1);

        return true;
    }

} // namespace LuminethRealmLords