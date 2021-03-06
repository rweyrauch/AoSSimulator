/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <fyreslayers/AuricRuneson.h>
#include <Board.h>
#include <UnitFactory.h>
#include "FyreslayerPrivate.h"

namespace Fyreslayers {

    class DauntlessAssault : public CommandAbility {
    public:
        explicit DauntlessAssault(Unit *source) :
                CommandAbility(source, "Dauntless Assault", 12, 12, GamePhase::Combat) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit *target) override {
            auto units = Board::Instance()->getUnitsWithin(m_source, m_source->owningPlayer(), m_rangeGeneral);
            for (auto unit : units) {
                unit->buffModifier(Attribute::To_Wound_Melee, 1, {GamePhase::Combat, m_round, m_source->owningPlayer()});
            }
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 100;

    bool AuricRuneson::s_registered = false;

    AuricRuneson::AuricRuneson(Lodge lodge, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Fyreslayer(lodge, "Auric Runeson", 4, g_wounds, 7, 4, false, g_pointsPerUnit),
            m_throwingAxe(Weapon::Type::Missile, "Fyresteel Throwing Axe", 8, 1, 5, 5, 0, 1),
            m_javelin(Weapon::Type::Missile, "Wyrmslayer Javelin", 12, 1, 4, 3, -1, RAND_D3),
            m_warAxe(Weapon::Type::Melee, "Ancestral War-axe", 1, 3, 3, 4, 0, RAND_D3),
            m_javelinMelee(Weapon::Type::Melee, "Wyrmslayer Javelin", 3, 1, 3, 3, -1, 1) {
        m_keywords = {ORDER, DUARDIN, FYRESLAYERS, HERO, AURIC_RUNESON};
        m_weapons = {&m_throwingAxe, &m_javelin, &m_warAxe, &m_javelinMelee};
        m_battleFieldRole = Role::Leader;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_throwingAxe);
        model->addMissileWeapon(&m_javelin);
        model->addMeleeWeapon(&m_warAxe);
        model->addMeleeWeapon(&m_javelinMelee);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<DauntlessAssault>(this));
    }

    Unit *AuricRuneson::Create(const ParameterList &parameters) {
        auto lodge = (Lodge) GetEnumParam("Lodge", parameters, g_lodge[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_fatherSonTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_heirloomArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new AuricRuneson(lodge, trait, artefact, general);
    }

    void AuricRuneson::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    AuricRuneson::Create,
                    Fyreslayer::ValueToString,
                    Fyreslayer::EnumStringToInt,
                    AuricRuneson::ComputePoints,
                    {
                            EnumParameter("Lodge", g_lodge[0], g_lodge),
                            EnumParameter("Artefact", g_heirloomArtefacts[0], g_heirloomArtefacts),
                            EnumParameter("Command Trait", g_fatherSonTraits[0], g_fatherSonTraits),
                            BoolParameter("General")
                    },
                    ORDER,
                    {FYRESLAYERS}
            };
            s_registered = UnitFactory::Register("Auric Runeson", factoryMethod);
        }
    }

    Rerolls AuricRuneson::toHitRerolls(const Weapon *weapon, const Unit *target) const {
        auto units = Board::Instance()->getUnitsWithin(this, owningPlayer(), 6.0);
        for (auto unit : units) {
            if (unit->hasKeyword(AURIC_RUNESON)) return Rerolls::Failed;
        }
        return Fyreslayer::toHitRerolls(weapon, target);
    }

    Wounds AuricRuneson::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Wyrmslayer Javelins
        if ((weapon->name() == m_javelin.name()) && target->hasKeyword(MONSTER)) {
            return {weapon->damage() + 2, 0};
        }
        return Fyreslayer::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int AuricRuneson::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace Fyreslayers