/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <skaven/GreySeer.h>
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include <skaven/SkavenSpells.h>
#include "SkavenPrivate.h"

namespace Skaven {

    class Wither : public Spell {
    public:
        explicit Wither(Unit *caster) :
                Spell(caster, "Witherl", 7, 13) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override {
            if (target == nullptr) return Spell::Result::Failed;

            if (Dice::RollD6() > target->wounds()) {
                target->applyDamage({0, RAND_D3, Wounds::Source::Spell}, m_caster);
                target->buffModifier(Attribute::To_Hit_Melee, -1, defaultDuration());
            }
            return Result::Success;
        }

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 140;

    bool GreySeer::s_registered = false;

    Unit *GreySeer::Create(const ParameterList &parameters) {
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_greySeerLore[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_masterClanCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_masterClanArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new GreySeer(lore, trait, artefact, general);
    }

    void GreySeer::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    Create,
                    Skaventide::ValueToString,
                    Skaventide::EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Command Trait", g_masterClanCommandTraits[0], g_masterClanCommandTraits),
                            EnumParameter("Artefact", g_masterClanArtefacts[0], g_masterClanArtefacts),
                            EnumParameter("Lore", g_greySeerLore[0], g_greySeerLore),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {SKAVEN}
            };

            s_registered = UnitFactory::Register("Grey Seer", factoryMethod);
        }
    }

    GreySeer::GreySeer(Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            Skaventide("Grey Seer", 6, g_wounds, 6, 5, false, g_pointsPerUnit),
            m_staff(Weapon::Type::Melee, "Warpstone Staff", 2, 1, 4, 3, -1, RAND_D3) {
        m_keywords = {CHAOS, SKAVEN, SKAVENTIDE, MASTERCLAN, HERO, WIZARD, GREY_SEER};
        m_weapons = {&m_staff};
        m_battleFieldRole = Role::Leader;

        m_totalSpells = 2;
        m_totalUnbinds = 2;

        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<Wither>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    int GreySeer::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} //namespace Skaven
