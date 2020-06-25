/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include "skaven/SkritchSpiteclaw.h"

namespace Skaven {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 70;

    bool SkritchSpiteclaw::s_registered = false;

    Unit *SkritchSpiteclaw::Create(const ParameterList &parameters) {
        auto unit = new SkritchSpiteclaw();

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    int SkritchSpiteclaw::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

    void SkritchSpiteclaw::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Skritch Spiteclaw", factoryMethod);
        }
    }

    SkritchSpiteclaw::SkritchSpiteclaw() :
            Skaventide("Skritch Spiteclaw", 6, WOUNDS, 6, 4, false),
            m_halberd(Weapon::Type::Melee, "Wicked Halberd", 2, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, CLANS_VERMINUS, HERO, CLAWLORD, SKRITCH_SPITECLAW};
        m_weapons = {&m_halberd};
    }

    bool SkritchSpiteclaw::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_halberd);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

} // namespace Skaven

