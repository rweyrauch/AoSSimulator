/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/Sorceress.h"
#include "CitiesOfSigmarPrivate.h"
#include "CoSLore.h"

namespace CitiesOfSigmar {

    class WordOfPain : public Spell {
    public:
        WordOfPain(Unit* caster) :
                Spell(caster, "Word of Pain", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit* target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->applyDamage( {0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            target->buffModifier(To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(To_Hit_Missile, -1, defaultDuration());

            return Spell::Result::Success;
        }
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override { return Spell::Result::Failed; }
    };

    class CommandUnderlings : public CommandAbility {
    public:
        CommandUnderlings(Unit *source) :
                CommandAbility(source, "Command Underlings", 12, 12, Phase::Hero) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {DARKLING_COVENS};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit* target) override {
            if (target == nullptr)
                return false;
            target->buffMovement(Run_And_Shoot, true, defaultDuration());
            target->buffMovement(Run_And_Charge, true, defaultDuration());
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool Sorceress::s_registered = false;

    Unit *Sorceress::Create(const ParameterList &parameters) {
        auto unit = new Sorceress();

        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        unit->setCity(city);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto drug = (Narcotic)GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        unit->setNarcotic(drug);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        bool ok = unit->configure(lore);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string Sorceress::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int Sorceress::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void Sorceress::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Sorceress::Create,
                    Sorceress::ValueToString,
                    Sorceress::EnumStringToInt,
                    Sorceress::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Sorceress", factoryMethod);
        }
    }

    Sorceress::Sorceress() :
            CitizenOfSigmar("Sorceress", 6, g_wounds, 7, 6, false),
            m_witchstaff(Weapon::Type::Melee, "Witchstaff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, HERO, WIZARD, SORCERESS};
        m_weapons = {&m_witchstaff};
        m_battleFieldRole = Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;
    }

    bool Sorceress::configure(Lore lore) {
        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_witchstaff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<WordOfPain>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<CommandUnderlings>(this));

        m_points = g_pointsPerUnit;

        return true;
    }

    int Sorceress::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar