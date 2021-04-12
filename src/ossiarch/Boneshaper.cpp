/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "ossiarch/Boneshaper.h"
#include "OssiarchBonereaperPrivate.h"

namespace OssiarchBonereapers {

    class ShardStorm : public Spell {
    public:
        explicit ShardStorm(Unit *caster);

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override;

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x, double y) override {
            return Result::Failed;
        }
    };

    ShardStorm::ShardStorm(Unit *caster) :
            Spell(caster, "Shard-storm", 5, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result ShardStorm::apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        Dice::RollResult roll;
        Dice::RollD6(target->remainingModels(), roll);

        int totalWounds = roll.rollsGE(6);
        target->applyDamage({0, totalWounds, Wounds::Source::Spell}, m_caster);

        return Spell::Result::Success;
    }

    static const int g_basesize = 32;
    static const int g_wounds = 5;
    static const int g_pointsPerUnit = 130;

    bool MortisanBoneshaper::s_registered = false;

    Unit *MortisanBoneshaper::Create(const ParameterList &parameters) {
        auto legion = (Legion) GetEnumParam("Legion", parameters, g_legion[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_mortisanCommandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_boneshaperArtefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);

        return new MortisanBoneshaper(legion, lore, trait, artefact, general);
    }

    void MortisanBoneshaper::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    MortisanBoneshaper::Create,
                    OssiarchBonereaperBase::ValueToString,
                    OssiarchBonereaperBase::EnumStringToInt,
                    MortisanBoneshaper::ComputePoints,
                    {
                            EnumParameter("Legion", g_legion[0], g_legion),
                            EnumParameter("Lore", g_lore[0], g_lore),
                            EnumParameter("Command Trait", g_mortisanCommandTraits[0], g_mortisanCommandTraits),
                            EnumParameter("Artefact", g_boneshaperArtefacts[0], g_boneshaperArtefacts),
                            BoolParameter("General")
                    },
                    DEATH,
                    {OSSIARCH_BONEREAPERS}
            };
            s_registered = UnitFactory::Register("Mortisan Boneshaper", factoryMethod);
        }
    }

    MortisanBoneshaper::MortisanBoneshaper(Legion legion, Lore lore, CommandTrait trait, Artefact artefact, bool isGeneral) :
            OssiarchBonereaperBase("Mortisan Boneshaper", 5, g_wounds, 10, 4, false) {
        m_keywords = {DEATH, OSSIARCH_BONEREAPERS, MORTISAN, HERO, WIZARD, MORTISAN_BONESHAPER};
        m_weapons = {&m_talons};
        m_battleFieldRole = Role::Leader;

        setLegion(legion);
        setCommandTrait(trait);
        setArtefact(artefact);
        setGeneral(isGeneral);

        m_totalSpells = 1;
        m_totalUnbinds = 1;

        auto model = new Model(g_basesize, wounds());
        model->addMeleeWeapon(&m_talons);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<ShardStorm>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));

        m_points = g_pointsPerUnit;
    }

    int MortisanBoneshaper::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} // namespace OssiarchBonereapers
