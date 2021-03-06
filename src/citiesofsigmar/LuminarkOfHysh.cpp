/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <UnitFactory.h>
#include <Board.h>
#include <spells/MysticShield.h>
#include "citiesofsigmar/LuminarkOfHysh.h"
#include "CitiesOfSigmarPrivate.h"
#include "CoSLore.h"

namespace CitiesOfSigmar {

    class BurningGaze : public Spell {
    public:
        explicit BurningGaze(Unit *caster) :
                Spell(caster, "Burning Gaze", 6, 18) {
            m_allowedTargets = Abilities::Target::Enemy;
            m_effect = Abilities::EffectType::Damage;
        }

    protected:
        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, Unit *target) override {
            if (target == nullptr)
                return Spell::Result::Failed;


            Wounds wounds = {0, Dice::RollD3(), Wounds::Source::Spell};
            if (target->remainingModels() >= 20) {
                wounds.mortal *= 3;
            } else if (target->remainingModels() >= 10) {
                wounds.mortal *= 2;
            }
            target->applyDamage(wounds, m_caster);

            return Spell::Result::Success;
        }

        Result apply(int castingValue, const UnmodifiedCastingRoll &unmodifiedCastingValue, double x,
                     double y) override { return Spell::Result::Failed; }
    };

    static const int g_basesize = 105;
    static const int g_wounds = 11;
    static const int g_pointsPerUnit = 210;
    static const int g_pointsPerUnitWithBattlemage = 270;

    struct TableEntry {
        int m_move;
        int m_auraRange;
        int m_beamOfLight;
    };

    const size_t g_numTableEntries = 5;
    const int g_woundThresholds[g_numTableEntries] = {2, 4, 6, 8, g_wounds};
    const TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 10, 2},
                    {9,  8,  3},
                    {8,  6,  4},
                    {7,  4,  5},
                    {6,  2,  6}
            };

    bool LuminarkOfHysh::s_registered = false;

    Unit *LuminarkOfHysh::Create(const ParameterList &parameters) {
        auto city = (City) GetEnumParam("City", parameters, g_city[0]);
        auto battlemage = GetBoolParam("Battlemage", parameters, true);
        auto lore = (Lore) GetEnumParam("Lore", parameters, g_lore[0]);
        auto drug = (Narcotic) GetEnumParam("Narcotic", parameters, g_narcotic[0]);
        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_commandTraits[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_artefacts[0]);
        auto general = GetBoolParam("General", parameters, false);
        return new LuminarkOfHysh(city, battlemage, lore, drug, trait, artefact, general);
    }

    std::string LuminarkOfHysh::ValueToString(const Parameter &parameter) {
        return CitizenOfSigmar::ValueToString(parameter);
    }

    int LuminarkOfHysh::EnumStringToInt(const std::string &enumString) {
        return CitizenOfSigmar::EnumStringToInt(enumString);
    }

    void LuminarkOfHysh::Init() {
        if (!s_registered) {
            static FactoryMethod factoryMethod = {
                    LuminarkOfHysh::Create,
                    LuminarkOfHysh::ValueToString,
                    LuminarkOfHysh::EnumStringToInt,
                    LuminarkOfHysh::ComputePoints,
                    {
                            EnumParameter("City", g_city[0], g_city),
                            BoolParameter("Battlemage"),
                            EnumParameter("Command Trait", g_commandTraits[0], g_commandTraits),
                            EnumParameter("Artefact", g_artefacts[0], g_artefacts),
                            EnumParameter("Narcotic", g_narcotic[0], g_narcotic),
                            BoolParameter("General")
                    },
                    ORDER,
                    {CITIES_OF_SIGMAR}
            };
            s_registered = UnitFactory::Register("Luminark of Hysh", factoryMethod);
        }
    }

    LuminarkOfHysh::LuminarkOfHysh(City city, bool battlemage, Lore lore, Narcotic narcotic, CommandTrait trait, Artefact artefact, bool isGeneral) :
            CitizenOfSigmar(city, "Luminark of Hysh", 10, g_wounds, 6, 4, false, battlemage ? g_pointsPerUnitWithBattlemage : g_pointsPerUnit),
            m_beamOfLight(Weapon::Type::Missile, "Searing Beam of Light", 30, 1, 0, 0, 0, 0),
            m_wizardsStaff(Weapon::Type::Melee, "Wizard's Staff", 2, 1, 4, 3, -1, RAND_D3),
            m_arcaneTools(Weapon::Type::Melee, "Arcane Tools", 1, 4, 5, 5, 0, 1),
            m_hooves(Weapon::Type::Melee, "Steel-shod Hooves", 1, 4, 4, 4, 0, 1) {
        m_keywords = {ORDER, HUMAN, CITIES_OF_SIGMAR, COLLEGIATE_ARCANE, LUMINARK_OF_HYSH};
        m_weapons = {&m_beamOfLight, &m_wizardsStaff, &m_arcaneTools, &m_hooves};
        m_hasMount = true;
        m_hooves.setMount(true);
        m_battleFieldRole = Role::Behemoth;

        s_globalUnbindMod.connect(this, &LuminarkOfHysh::locusOfHysh, &m_locusSlot);

        if (battlemage) {
            addKeyword(WIZARD);
            addKeyword(HERO);
            m_battleFieldRole = Role::Leader_Behemoth;
            m_totalSpells = 1;
            m_totalUnbinds = 1;

            setCommandTrait(trait);
            setArtefact(artefact);
            setGeneral(isGeneral);
            setNarcotic(narcotic);
        }

        auto model = new Model(g_basesize, wounds());
        model->addMissileWeapon(&m_beamOfLight);
        model->addMeleeWeapon(&m_arcaneTools);
        model->addMeleeWeapon(&m_hooves);
        if (battlemage) {
            model->addMeleeWeapon(&m_wizardsStaff);
        }
        addModel(model);

        if (battlemage) {
            m_knownSpells.push_back(std::make_unique<BurningGaze>(this));
            m_knownSpells.push_back(std::make_unique<BuffModifierSpell>(this, "Pha's Protection", 5, 18,
                                                                        std::vector<std::pair<Attribute, int>>{{Attribute::Target_To_Hit_Missile, -1},
                                                                                                               {Attribute::Target_To_Hit_Melee,   -1}},
                                                                        Abilities::Target::SelfAndFriendly));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateLore(lore, this)));
            m_knownSpells.push_back(std::unique_ptr<Spell>(CreateArcaneBolt(this)));
            m_knownSpells.push_back(std::make_unique<MysticShield>(this));
        }
    }

    LuminarkOfHysh::~LuminarkOfHysh() {
        m_locusSlot.disconnect();
    }

    void LuminarkOfHysh::onRestore() {
        CitizenOfSigmar::onRestore();

        // Restore table-driven attributes
        onWounded();
    }

    void LuminarkOfHysh::onWounded() {
        m_move = g_damageTable[getDamageTableIndex()].m_move;

        CitizenOfSigmar::onWounded();
    }

    size_t LuminarkOfHysh::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int LuminarkOfHysh::castingModifier() const {
        auto mod = CitizenOfSigmar::castingModifier();

        // White Battlemage
        if (Board::Instance()->getRealm() == Realm::Hysh) mod++;

        return mod;
    }

    int LuminarkOfHysh::locusOfHysh(const Unit *caster) {
        if (caster->hasKeyword(COLLEGIATE_ARCANE) && caster->hasKeyword(WIZARD) &&
            ((caster->owningPlayer() == owningPlayer()) && (distanceTo(caster) <= 12.0))) {
            return 1;
        }
        return 0;
    }

    int LuminarkOfHysh::ComputePoints(const ParameterList& /*parameters*/) {
        return g_pointsPerUnit;
    }

} // namespace CitiesOfSigmar