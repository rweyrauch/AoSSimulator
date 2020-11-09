/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2019 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

#include <seraphon/SaurusVeteranCarnosaur.h>
#include <UnitFactory.h>
#include <Board.h>
#include "SeraphonPrivate.h"

namespace Seraphon {

    static const int g_basesize = 120; // x92 oval
    static const int g_wounds = 12;
    static const int g_pointsPerUnit = 210;

    struct TableEntry {
        int m_move;
        int m_forelimbToHit;
        int m_jawsDamage;
    };

    const size_t g_numTableEntries = 5;
    static int g_woundThresholds[g_numTableEntries] = {2, 4, 7, 9, g_wounds};
    static TableEntry g_damageTable[g_numTableEntries] =
            {
                    {10, 3, 5},
                    {9,  4, 4},
                    {8,  4, 3},
                    {7,  5, 2},
                    {6,  5, 1}
            };

    bool SaurusScarVeteranOnCarnosaur::s_registered = false;

    SaurusScarVeteranOnCarnosaur::SaurusScarVeteranOnCarnosaur() :
            SeraphonBase("Saurus Scar-Veteran on Carnosaur", 10, g_wounds, 8, 4, false),
            m_warblade(Weapon::Type::Melee, "Celestite Warblade", 1, 6, 3, 3, 0, 1),
            m_warspear(Weapon::Type::Melee, "Celestite Warspear", 2, 6, 3, 3, 0, 1),
            m_greatblade(Weapon::Type::Melee, "Celestite Greatblade", 1, 3, 4, 3, -1, 2),
            m_forelimbs(Weapon::Type::Melee, "Clawed Forelimbs", 2, 2, 3, 3, 0, 2),
            m_jaws(Weapon::Type::Melee, "Massive Jaws", 2, 3, 4, 3, -1, 5) {
        m_keywords = {ORDER, SERAPHON, CARNOSAUR, SAURUS, MONSTER, HERO, SCAR_VETERAN};
        m_weapons = {&m_warblade, &m_warspear, &m_greatblade, &m_forelimbs, &m_jaws};
        m_battleFieldRole = Leader_Behemoth;
        m_hasMount = true;

        s_globalBraveryMod.connect(this, &SaurusScarVeteranOnCarnosaur::terror, &m_connection);
    }

    SaurusScarVeteranOnCarnosaur::~SaurusScarVeteranOnCarnosaur() {
        m_connection.disconnect();
    }

    bool SaurusScarVeteranOnCarnosaur::configure(WeaponOption option) {
        auto model = new Model(g_basesize, wounds());
        if (option == Celestite_Warblade) model->addMeleeWeapon(&m_warblade);
        else if (option == Celestite_Warspear) model->addMeleeWeapon(&m_warspear);
        else if (option == Celestite_Greatblade) model->addMeleeWeapon(&m_greatblade);
        model->addMeleeWeapon(&m_forelimbs);
        model->addMeleeWeapon(&m_jaws);
        addModel(model);

        m_points = ComputePoints(1);

        return true;
    }

    void SaurusScarVeteranOnCarnosaur::onRestore() {
        // Reset table-drive attributes
        onWounded();
    }

    Unit *SaurusScarVeteranOnCarnosaur::Create(const ParameterList &parameters) {
        auto unit = new SaurusScarVeteranOnCarnosaur();
        auto option = (WeaponOption) GetEnumParam("Weapon", parameters, Celestite_Warblade);

        auto way = (WayOfTheSeraphon) GetEnumParam("Way of the Seraphon", parameters, g_wayOfTheSeraphon[0]);
        auto constellation = (Constellation) GetEnumParam("Constellation", parameters, g_constellation[0]);
        unit->setWayOfTheSeraphon(way, constellation);

        auto trait = (CommandTrait) GetEnumParam("Command Trait", parameters, g_saurusCommandTrait[0]);
        auto artefact = (Artefact) GetEnumParam("Artefact", parameters, g_celestialRelicsOfTheWarrior[0]);

        unit->setArtefact(artefact);
        unit->setCommandTrait(trait);

        auto general = GetBoolParam("General", parameters, false);
        unit->setGeneral(general);

        bool ok = unit->configure(option);
        if (!ok) {
            delete unit;
            unit = nullptr;
        }
        return unit;
    }

    std::string SaurusScarVeteranOnCarnosaur::ValueToString(const Parameter &parameter) {
        if (std::string(parameter.name) == "Weapon") {
            if (parameter.intValue == Celestite_Warblade) return "Celestite Warblade";
            else if (parameter.intValue == Celestite_Warspear) return "Celestite War-spear";
            else if (parameter.intValue == Celestite_Greatblade) return "Celestite Greatblade";
        }
        return SeraphonBase::ValueToString(parameter);
    }

    int SaurusScarVeteranOnCarnosaur::EnumStringToInt(const std::string &enumString) {
        if (enumString == "Celestite Warblade") return Celestite_Warblade;
        else if (enumString == "Celestite War-spear") return Celestite_Warspear;
        else if (enumString == "Celestite Greatblade") return Celestite_Greatblade;
        return SeraphonBase::EnumStringToInt(enumString);
    }

    void SaurusScarVeteranOnCarnosaur::Init() {
        if (!s_registered) {
            static const std::array<int, 3> weapons = {Celestite_Warblade, Celestite_Warspear, Celestite_Greatblade};
            static FactoryMethod factoryMethod = {
                    Create,
                    ValueToString,
                    EnumStringToInt,
                    ComputePoints,
                    {
                            EnumParameter("Weapon", Celestite_Warblade, weapons),
                            EnumParameter("Way of the Seraphon", g_wayOfTheSeraphon[0], g_wayOfTheSeraphon),
                            EnumParameter("Constellation", g_constellation[0], g_constellation),
                            EnumParameter("Command Trait", g_saurusCommandTrait[0], g_saurusCommandTrait),
                            EnumParameter("Artefact", g_celestialRelicsOfTheWarrior[0], g_celestialRelicsOfTheWarrior),
                            BoolParameter("General")
                    },
                    ORDER,
                    {SERAPHON}
            };

            s_registered = UnitFactory::Register("Saurus Scar-Veteran on Carnosaur", factoryMethod);
        }
    }

    void SaurusScarVeteranOnCarnosaur::onWounded() {
        const int damageIndex = getDamageTableIndex();
        m_jaws.setDamage(g_damageTable[damageIndex].m_jawsDamage);
        m_forelimbs.setToHit(g_damageTable[damageIndex].m_forelimbToHit);
        m_move = g_damageTable[getDamageTableIndex()].m_move;
    }

    int SaurusScarVeteranOnCarnosaur::getDamageTableIndex() const {
        auto woundsInflicted = wounds() - remainingWounds();
        for (auto i = 0u; i < g_numTableEntries; i++) {
            if (woundsInflicted < g_woundThresholds[i]) {
                return i;
            }
        }
        return 0;
    }

    int
    SaurusScarVeteranOnCarnosaur::generateHits(int unmodifiedHitRoll, const Weapon *weapon, const Unit *unit) const {
        // Cold Ferocity
        if ((unmodifiedHitRoll == 6) &&
            ((weapon->name() == m_warblade.name()) ||
             (weapon->name() == m_warspear.name()) ||
             (weapon->name() == m_greatblade.name()))) {
            return 2;
        }
        return Unit::generateHits(unmodifiedHitRoll, weapon, unit);
    }

    int SaurusScarVeteranOnCarnosaur::toHitModifier(const Weapon *weapon, const Unit *target) const {
        auto mod = SeraphonBase::toHitModifier(weapon, target);

        // Pinned Down
        if ((weapon->name() == m_jaws.name()) && (target->wounds() >= 7)) {
            mod++;
        }

        return mod;
    }

    Wounds SaurusScarVeteranOnCarnosaur::weaponDamage(const Weapon *weapon, const Unit *target, int hitRoll,
                                                      int woundRoll) const {
        // Celestite Warspear
        if (m_charged && (weapon->name() == m_warspear.name())) {
            return {weapon->damage() + 1, 0};
        }
        return Unit::weaponDamage(weapon, target, hitRoll, woundRoll);
    }

    int SaurusScarVeteranOnCarnosaur::terror(const Unit *target) {
        // Terror
        if ((target->owningPlayer() != owningPlayer()) && (distanceTo(target) <= 3.0)) {
            return -1;
        }
        return 0;
    }

    int SaurusScarVeteranOnCarnosaur::ComputePoints(int /*numModels*/) {
        return g_pointsPerUnit;
    }

} //namespace Seraphon
