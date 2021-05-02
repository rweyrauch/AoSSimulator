/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <khorne/LordOfKhorneOnJuggernaut.h>
#include <UnitFactory.h>
#include <Board.h>
#include "KhornePrivate.h"

namespace Khorne {

    class BloodStampede : public CommandAbility {
    public:
        explicit BloodStampede(Unit *source) :
                CommandAbility(source, "Blood Stampede", 16, 16, GamePhase::Combat) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {KHORNE, MORTAL};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit *target) override {
            auto units = Board::Instance()->getUnitsWithin(m_source, m_source->owningPlayer(), m_rangeGeneral);
            int unitCount = 0;
            for (auto unit : units) {
                if (unit->charged()) {
                    unit->buffReroll(Attribute::To_Wound_Melee, Rerolls::Ones,
                                     {GamePhase::Combat, m_round, m_source->owningPlayer()});
                    unitCount++;
                }
                if (unitCount > 3) break;
            }
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 90; // x52 oval
    static const int g_wounds = 8;
    static const int g_pointsPerUnit = 160;

    bool LordOfKhorneOnJuggernaut::s_registered = false;

    LordOfKhorneOnJuggernaut::LordOfKhorneOnJuggernaut(SlaughterHost host, CommandTrait trait, Artefact artefact, bool isGeneral) :
            KhorneBase("Lord of Khorne on Juggernaut", 5, g_wounds, 9, 3, false, g_pointsPerUnit) {
        m_keywords = {CHAOS, MORTAL, KHORNE, BLOODBOUND, HERO, LORD_OF_KHORNE_ON_JUGGERNAUT};
        m_weapons = {&m_wrathforgedAxe, &m_brazenHooves};
        m_battleFieldRole = Role::Leader;
        m_hasMount = true;
        m_brazenHooves.setMount(true);

        setSlaughterHost(host);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_wrathforgedAxe);
        model->addMeleeWeapon(&m_brazenHooves);
        addModel(model);

        m_commandAbilities.push_back(std::make_unique<BloodStampede>(this));

        m_points = g_pointsPerUnit;
    }

    Unit *LordOfKhorneOnJuggernaut::Create(const ParameterList &parameters) {
        auto host = (SlaughterHost) GetEnumParam("Slaughter Host", parameters, g_slaughterHost[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortalbloodboundCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_mortalArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);

        return new LordOfKhorneOnJuggernaut(host, trait, artefact, general);
    }

    void LordOfKhorneOnJuggernaut::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LordOfKhorneOnJuggernaut::Create,
                    KhorneBase::ValueToString,
                    KhorneBase::EnumStringToInt,
                    LordOfKhorneOnJuggernaut::ComputePoints,
                    {
                            EnumParameter("Slaughter Host", g_slaughterHost[0], g_slaughterHost),
                            EnumParameter("Command Trait", g_mortalbloodboundCommandTraits[0],
                                          g_mortalbloodboundCommandTraits),
                            EnumParameter("Artefact", g_mortalArtefacts[0], g_mortalArtefacts),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {KHORNE}
            };
            s_registered = UnitFactory::Register("Lord of Khorne on Juggernaut", factoryMethod);
        }
    }

    void LordOfKhorneOnJuggernaut::onCharged() {
        // Slaughterous Charge
        if (m_meleeTarget && (distanceTo(m_meleeTarget) <= 1.0)) {
            int roll = Dice::RollD6();
            if (roll >= 2) {
                Wounds wounds = {0, Dice::RollD3()};

                PLOG_INFO.printf("%s Murderous Charge inflicted %d mortal wounds on %s\n",
                                 name().c_str(), wounds.mortal, m_meleeTarget->name().c_str());

                m_meleeTarget->applyDamage(wounds, this);
            }
        }
        KhorneBase::onCharged();
    }

    Wounds
    LordOfKhorneOnJuggernaut::weaponDamage(const Model* attackingModel, const Weapon *weapon, const Unit *target, int hitRoll, int woundRoll) const {
        // Daemonic Axe
        if ((hitRoll == 6) && (weapon->name() == m_wrathforgedAxe.name())) {
            return {RAND_D3, 0};
        }
        return KhorneBase::weaponDamage(attackingModel, weapon, target, hitRoll, woundRoll);
    }

    int LordOfKhorneOnJuggernaut::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    Wounds LordOfKhorneOnJuggernaut::applyWoundSave(const Wounds &wounds, Unit *attackingUnit) {
        auto totalWounds = KhorneBase::applyWoundSave(wounds, attackingUnit);

        // Brass-clad Shield
        if (totalWounds.source == Wounds::Source::Spell) {
            totalWounds = ignoreWounds(totalWounds, 5);
        }
        return totalWounds;
    }


} // namespace Khorne
