/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <gloomspitegitz/ScuttlebossOnSpider.h>
#include <UnitFactory.h>
#include <Board.h>
#include <Roster.h>
#include <iostream>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int BASESIZE = 60;
    static const int WOUNDS = 6;
    static const int POINTS_PER_UNIT = 100;

    bool ScuttlebossOnGiganticSpider::s_registered = false;

    ScuttlebossOnGiganticSpider::ScuttlebossOnGiganticSpider() :
            GloomspiteGitzBase("Scuttleboss on Gigantic Spider", 8, WOUNDS, 6, 4, true),
            m_spear(Weapon::Type::Melee, "Envenomed Spear", 2, 4, 4, 4, -1, 1),
            m_fangs(Weapon::Type::Melee, "Gigantic Fangs", 1, 4, 4, 3, -1, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, SPIDERFANG, HERO, SCUTTLEBOSS};
        m_weapons = {&m_spear, &m_fangs};
        m_hasMount = true;
    }

    bool ScuttlebossOnGiganticSpider::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_fangs);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *ScuttlebossOnGiganticSpider::Create(const ParameterList &parameters) {
        auto unit = new ScuttlebossOnGiganticSpider();

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_marksOfTheSpiderGodsFavour[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_venomousValuables[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure();
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    void ScuttlebossOnGiganticSpider::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ScuttlebossOnGiganticSpider::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ScuttlebossOnGiganticSpider::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_marksOfTheSpiderGodsFavour[0], g_marksOfTheSpiderGodsFavour),
                            EnumParameter("Artefact", g_venomousValuables[0], g_venomousValuables),
                            BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Scuttleboss on Gigantic Spider", factoryMethod);
        }
    }

    Wounds ScuttlebossOnGiganticSpider::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Spider Venom
        int threshold = inLightOfTheBadMoon() ? 5 : 6;
        if ((hitRoll >= threshold) && (weapon->name() == m_fangs.name())) {
            return {0, 1};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int ScuttlebossOnGiganticSpider::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }


} // namespace GloomspiteGitz

