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

    class RideEmAllDown : public CommandAbility {
    public:
        explicit RideEmAllDown(Unit *source);

    protected:
        bool apply(Unit *target) override;

        bool apply(double x, double y) override { return false; }
    };

    RideEmAllDown::RideEmAllDown(Unit *source) :
            CommandAbility(source, "Ride 'Em All Down", 18, 18, GamePhase::Charge) {
        m_allowedTargets = Abilities::Target::Friendly;
        m_effect = Abilities::EffectType::Buff;
        m_targetKeywords.push_back(SPIDERFANG);
        m_targetKeywords.push_back(GROT);
    }

    bool RideEmAllDown::apply(Unit *target) {
        if (target == nullptr) {
            return false;
        }

        target->buffReroll(Attribute::Charge_Distance, Rerolls::Failed,
                           {GamePhase::Charge, m_round, m_source->owningPlayer()});

        // TODO: only buff Crooked Spear
        target->buffReroll(Attribute::To_Hit_Melee, Rerolls::Failed,
                           {GamePhase::Combat, m_round, m_source->owningPlayer()});

        return true;
    }

    static const int g_basesize = 60;
    static const int g_wounds = 6;
    static const int g_pointsPerUnit = 100;

    bool ScuttlebossOnGiganticSpider::s_registered = false;

    ScuttlebossOnGiganticSpider::ScuttlebossOnGiganticSpider(Allegiance allegiance, CommandTrait trait, Artefact artefact, bool isGeneral) :
            GloomspiteGitzBase(allegiance, "Scuttleboss on Gigantic Spider", 8, g_wounds, 6, 4, true, g_pointsPerUnit),
            m_spear(Weapon::Type::Melee, "Envenomed Spear", 2, 4, 4, 4, -1, 1),
            m_fangs(Weapon::Type::Melee, "Gigantic Fangs", 1, 4, 4, 3, -1, 1) {
        m_keywords = {DESTRUCTION, GROT, GLOOMSPITE_GITZ, SPIDERFANG, HERO, SCUTTLEBOSS};
        m_weapons = {&m_spear, &m_fangs};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_fangs.setMount(true);
        s_globalBraveryMod.connect(this, &ScuttlebossOnGiganticSpider::ululatingBattleCry, &m_battleCryConnection);

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_spear);
        model->addMeleeWeapon(&m_fangs);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<RideEmAllDown>(this));
    }


    ScuttlebossOnGiganticSpider::~ScuttlebossOnGiganticSpider() {
        m_battleCryConnection.disconnect();
    }

    Unit *ScuttlebossOnGiganticSpider::Create(const ParameterList &parameters) {
        auto allegiance = (Allegiance) GetEnumParam("Allegiance", parameters, g_allegiance[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_marksOfTheSpiderGodsFavour[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_venomousValuables[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new ScuttlebossOnGiganticSpider(allegiance, trait, artefact, general);
    }

    void ScuttlebossOnGiganticSpider::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    ScuttlebossOnGiganticSpider::Create,
                    GloomspiteGitzBase::ValueToString,
                    GloomspiteGitzBase::EnumStringToInt,
                    ScuttlebossOnGiganticSpider::ComputePoints,
                    {
                            EnumParameter("Command Trait", g_marksOfTheSpiderGodsFavour[0],
                                          g_marksOfTheSpiderGodsFavour),
                            EnumParameter("Artefact", g_venomousValuables[0], g_venomousValuables),
                            BoolParameter("General"),
                            EnumParameter("Allegiance", g_allegiance[0], g_allegiance),
                    },
                    DESTRUCTION,
                    {GLOOMSPITE_GITZ}
            };
            s_registered = UnitFactory::Register("Scuttleboss on Gigantic Spider", factoryMethod);
        }
    }

    Wounds ScuttlebossOnGiganticSpider::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll,
                                                     int woundRoll) const {
        // Spider Venom
        int threshold = inLightOfTheBadMoon() ? 5 : 6;
        if ((hitRoll >= threshold) && (weapon->name() == m_fangs.name())) {
            if (m_commandTrait == CommandTrait::Monstrous_Mount) {
                return {0, 2, Wounds::Source::Weapon_Melee};
            }
            return {0, 1, Wounds::Source::Weapon_Melee};
        }
        return GloomspiteGitzBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int ScuttlebossOnGiganticSpider::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int ScuttlebossOnGiganticSpider::ululatingBattleCry(const Unit *unit) {
        if (isGeneral() && (m_commandTrait == CommandTrait::Ulutating_Battle_Cry) && (distanceTo(unit) < 9.0)) {
            return -1;
        }
        return 0;
    }

} // namespace GloomspiteGitz

