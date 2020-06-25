/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <algorithm>
#include <gloomspitegitz/Loonboss.h>
#include <UnitFactory.h>
#include <Board.h>
#include "GloomspitePrivate.h"

namespace GloomspiteGitz {
    static const int BASESIZE = 32;
    static const int WOUNDS = 5;
    static const int POINTS_PER_UNIT = 70;

    bool Loonboss::s_registered = false;

    Loonboss::Loonboss() :
            GloomspiteGitzBase("Loonboss", 5, WOUNDS, 5, 5, false),
            m_moonslicer(Weapon::Type::Melee, "Moon-slicer", 1, 3, 3, 3, -1, RAND_D3) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, MOONCLAN, HERO, LOONBOSS};
        m_weapons = {&m_moonslicer};
    }

    bool Loonboss::configure() {
        auto model = new Model(BASESIZE, wounds());
        model->addMeleeWeapon(&m_moonslicer);
        addModel(model);

        m_points = POINTS_PER_UNIT;

        return true;
    }

    Unit *Loonboss::Create(const ParameterList &parameters) {
        auto unit = new Loonboss();

        auto trait = (CommandTrait)GetEnumParam("Command Trait", parameters, g_blessingsOfTheBadMoon[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact)GetEnumParam("Artefact", parameters, g_troglodyticTreasures[0]);
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

    void Loonboss::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Loonboss::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    Loonboss::ComputePoints,
                    {
                        EnumParameter("Command Trait", g_blessingsOfTheBadMoon[0], g_blessingsOfTheBadMoon),
                        EnumParameter("Artefact", g_troglodyticTreasures[0], g_troglodyticTreasures),
                        BoolParameter("General")
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Loonboss", factoryMethod);
        }
    }

    int Loonboss::targetHitModifier(const Weapon *weapon, const Unit *attacker) const {
        int modifier = GloomspiteGitzBase::targetHitModifier(weapon, attacker);
        // Dead Tricksy
        modifier -= 1;
        return modifier;
    }

    int Loonboss::ComputePoints(int /*numModels*/) {
        return POINTS_PER_UNIT;
    }

} // namespace GloomspiteGitz
