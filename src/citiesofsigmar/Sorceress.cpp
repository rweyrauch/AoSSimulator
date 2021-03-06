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
        explicit WordOfPain(Unit *caster) :
                Spell(caster, "Word of Pain", 7, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;

            target->applyDamage({0, Dice::RollD3(), Wounds::Source::Spell}, m_caster);
            target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
            target->buffModifier(Attribute::To_Hit_Missile, -1, defaultDuration());

            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    class CommandUnderlings : public CommandAbility {
    public:
        explicit CommandUnderlings(Unit *source) :
                CommandAbility(source, "Command Underlings", 12, 12, GamePhase::Hero) {
            m_allowedTargets = Abilities::Target::SelfAndFriendly;
            m_targetKeywords = {DARKLING_COVENS};
            m_effect = Abilities::EffectType::Buff;
        }

    protected:

        bool apply(Unit *target) override {
            if (target == nullptr)
                return false;
            target->buffMovement(MovementRule::Run_And_Shoot, true, defaultDuration());
            target->buffMovement(MovementRule::Run_And_Charge, true, defaultDuration());
            return true;
        }

        bool apply(double x, double y) override { return false; }
    };

    static const int g_basesize = 25;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 90;

    bool Sorceress::s_registered = false;

    Unit *Sorceress::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new Sorceress(city, lore, drug, trait, artefact, general);
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

    Sorceress::Sorceress(City city, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Sorceress", 6, g_wounds, 7, 6, false, g_pointsPerUnit),
            m_witchstaff(Weapon::Type::Melee, "Witchstaff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {ORDER, AELF, CITIES_OF_SIGMAR, DARKLING_COVENS, HERO, WIZARD, SORCERESS};
        m_weapons = {&m_witchstaff};
        m_battleFieldRole = Role::Leader;

        m_totalUnbinds = 1;
        m_totalSpells = 1;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);
        setNarcotic(narcotic);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_witchstaff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<WordOfPain>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_commandAbilities.push_back(std::make_unique<CommandUnderlings>(this));
    }

    int Sorceress::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar