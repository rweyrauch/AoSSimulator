/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2021 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <kruleboyz/KillabossOnGnashtoof.h>
#include <UnitFactory.h>

namespace Kruleboyz {
    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 10;
    static const int g_pointsPerUnit = 0;

    bool KillabossOnGnashtoof::s_registered = false;

    KillabossOnGnashtoof::KillabossOnGnashtoof(Warclan warclan, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Kruleboyz(warclan, "Killaboss on Great Gnashtoof", 10, g_wounds, 7, 3, false, g_pointsPerUnit) {
        m_keywords = {DESTRUCTION, ORRUK_WARCLANS, ORRUK, KRULEBOYZ, HERO, KILLABOSS, GNASHTOOF};
        m_weapons = {&m_stikka, &m_fangs};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_fangs.setMount(true);

        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_stikka);
        model->addMeleeWeapon(&m_fangs);
        addModel(model);
    }

    Unit *KillabossOnGnashtoof::Create(const ParameterList &parameters) {
        auto general = GetBoolParam("General", parameters, false);
        return new KillabossOnGnashtoof(Warclan::None, CommandTrait::None, Artefact::None, general);
    }

    void KillabossOnGnashtoof::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    KillabossOnGnashtoof::Create,
                    Kruleboyz::ValueToString,
                    Kruleboyz::EnumStringToInt,
                    KillabossOnGnashtoof::ComputePoints,
                    {
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {IRONJAWZ}
            };
            s_registered = UnitFactory::Register("Killaboss on Great Gnashtoof", factoryMethod);
        }
    }

    int KillabossOnGnashtoof::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Kruleboyz