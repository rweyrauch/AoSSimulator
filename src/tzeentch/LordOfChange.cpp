/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <spells/MysticShield.h>
#include "tzeentch/LordOfChange.h"
#include "TzeentchPrivate.h"
#include "TzeentchSpells.h"

namespace Tzeentch {

    class InfernalGateway : public Spell {
    public:
        explicit InfernalGateway(Unit *caster);

    protected:
        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) override;

        Result apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, double x,
                     double y) override { return Result::Failed; }
    };

    InfernalGateway::InfernalGateway(Unit *caster) :
            Spell(caster, "Infernal Gateway", 7, 18) {
        m_allowedTargets = Abilities::Target::Enemy;
        m_effect = Abilities::EffectType::Damage;
    }

    Spell::Result
    InfernalGateway::apply(int castingRoll, const UnmodifiedCastingRoll &unmodifiedCastingRoll, Unit *target) {
        if (target == nullptr) {
            return Spell::Result::Failed;
        }

        auto loc = dynamic_cast<LordOfChange *>(m_caster);
        if (loc == nullptr) {
            return Spell::Result::Failed;
        }

        Dice::RollResult roll;
        Dice::RollD6(9, roll);
        const int threshold = loc->getInfernalGatewayValue();
        if (roll.rollsGE(threshold) > 0) {
            target->applyDamage({0, roll.rollsGE(threshold), Wounds::Source::Spell}, m_caster);
        }

        return Spell::Result::Success;
    }

    static const int g_basesize = 100;
    static const int g_wounds = 14;
    static const int g_pointsPerUnit = 380;

    struct TableEntry {
        int m_move;
        int m_staffToWound;
        int m_infernalGateway;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {3, 6, 9, 12, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {12, 1, 3},
                    {10, 2, 4},
                    {9,  3, 4},
                    {8,  4, 4},
                    {7,  5, 5}
            };

    bool LordOfChange::s_registered = false;

    Unit *LordOfChange::Create(const ParameterList &parameters) {
        auto unit = new LordOfChange();
        auto weapon = (WeaponOption) GetEnumParam("Weapon", parameters, Baleful_Sword);

        auto coven = (ChangeCoven) GetEnumParam("Change Coven", parameters, (int) ChangeCoven::None);
        unit->setChangeCoven(coven);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_daemonCommandTraits[0]);
        unit->setCommandTrait(trait);

        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_daemonArtefacts[0]);
        unit->setArtefact(artefact);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        auto lore = (Lore) GetEnumParam("Lore", parameters, g_loreOfChange[0]);

        unit->configure(weapon, lore);
        return unit;
    }

    std::string LordOfChange::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Rod_Of_Sorcery) return "Rod of Sorcery";
            else if (parameter.intValue == Baleful_Sword) return "Baleful Sword";
            else if (parameter.intValue == Curved_Beak_And_Talons) return "Curved Beak and Talons";
        }
        return TzeentchBase::ValueToString(parameter);
    }

    int LordOfChange::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Rod of Sorcery") return Rod_Of_Sorcery;
        else if (enumString == "Baleful Sword") return Baleful_Sword;
        else if (enumString == "Curved Beak and Talons") return Curved_Beak_And_Talons;
        return TzeentchBase::EnumStringToInt(enumString);
    }

    void LordOfChange::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Baleful_Sword, Rod_Of_Sorcery, Curved_Beak_And_Talons};
            static const FactoryMethod factoryMethod = {
                    LordOfChange::Create,
                    LordOfChange::ValueToString,
                    LordOfChange::EnumStringToInt,
                    LordOfChange::ComputePoints,
                    {
                            EnumParameter("Weapon", Baleful_Sword, weapons),
                            EnumParameter("Change Coven", g_changeCoven[0], g_changeCoven),
                            EnumParameter("Command Trait", g_daemonCommandTraits[0], g_daemonCommandTraits),
                            EnumParameter("Artefact", g_daemonArtefacts[0], g_daemonArtefacts),
                            EnumParameter("Lore", g_loreOfChange[0], g_loreOfChange),
                            BoolParameter("General")
                    },
                    CHAOS,
                    {TZEENTCH}
            };
            s_registered = UnitFactory::Register("Lord of Change", factoryMethod);
        }
    }

    LordOfChange::LordOfChange() :
            TzeentchBase("Lord of Change", 12, g_wounds, 10, 4, true, g_pointsPerUnit),
            m_rodOfSorcery(Weapon::Type::Missile, "Rod of Sorcery", 18, RAND_2D6, 3, 3, -1, 1),
            m_staff(Weapon::Type::Melee, "Staff of Tzeentch", 3, 4, 3, 1, 0, 2),
            m_sword(Weapon::Type::Melee, "Baleful Sword", 1, 2, 4, 2, -2, 3),
            m_beakAndTalons(Weapon::Type::Melee, "Curved Beak and Wicked Talons", 1, 4, 4, 3, -1, 2) {
        m_keywords = {CHAOS, DAEMON, TZEENTCH, MONSTER, HERO, WIZARD, LORD_OF_CHANGE};
        m_weapons = {&m_rodOfSorcery,
                     &m_staff,
                     &m_sword,
                     &m_beakAndTalons};
        m_battleFieldRole = Role::Leader_Behemoth;

        m_totalSpells = 2;
        m_totalUnbinds = 2;
    }

    void LordOfChange::configure(WeaponOption option, Lore lore) {
        auto model = new Model(g_basesize, wounds());
        if (option == Rod_Of_Sorcery)
            model->addMissileWeapon(&m_rodOfSorcery);
        else if (option == Baleful_Sword)
            model->addMeleeWeapon(&m_sword);
        else if (option == Curved_Beak_And_Talons)
            model->addMeleeWeapon(&m_beakAndTalons);
        model->addMeleeWeapon(&m_staff);
        addModel(model);

        m_knownSpells.push_back(std::make_unique<InfernalGateway>(this));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
        m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
        m_knownSpells.push_back(std::make_unique<MysticShield>(this));
    }

    void LordOfChange::onRestore() {
        TzeentchBase::onRestore();
        // Reset table-drive attributes
        onWounded();
    }

    void LordOfChange::onWounded() {
        TzeentchBase::onWounded();

        const auto damageIndex = getDamageTableIndex();
        m_staff.setToWound(g_damageTable[damageIndex].m_staffToWound);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    size_t LordOfChange::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int LordOfChange::rollCasting(UnmodifiedCastingRoll &unmodifiedRoll) const {
        // Mastery of Magic
        auto r0 = Dice::RollD6();
        auto r1 = Dice::RollD6();
        unmodifiedRoll.d1 = std::max(r0, r1);
        unmodifiedRoll.d2 = std::max(r0, r1);
        return unmodifiedRoll + castingModifier();
    }

    int LordOfChange::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

    int LordOfChange::getInfernalGatewayValue() const {
        return g_damageTable[getDamageTableIndex()].m_infernalGateway;
    }

} // Tzeentch