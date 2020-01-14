/*
 * Warhammer Age of Sigmar battle simulator.
 *
 * Copyright (C) 2020 by Rick Weyrauch - rpweyrauch@gmail.com
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */

if (AosSim === undefined) {
    console.log("Module not found");
}
else {
    console.log("AoS module is loaded.");
}

AosSim().then(AosSim => {

    const sim = AosSim.JSInterface.prototype;

    function populateFactions(alliance, selector, unitSelector, unitRoot) {
        selector.options.length = 0;
        let factionKeywords = new Array();

        const allianceId = sim.GrandAllianceStringToKeyword(alliance);

        for (let idx = 0; idx < sim.GetNumberOfAvailableUnits(); idx++) {
            var unitInfo = new AosSim.JSUnitInfo();
            sim.GetUnitInfo(idx, unitInfo);
            if (allianceId === unitInfo.grandAlliance) {
                for (let fidx = 0; fidx < unitInfo.numberOfFactions; fidx++) {
                    factionKeywords.push(unitInfo.getFaction(fidx));
                }
            }
        }
        factionKeywords.sort();
        let uniqueKeywords = factionKeywords.filter((value, index, self) => { return self.indexOf(value) === index; });

        for (let ip of uniqueKeywords) {
            const factionName = sim.FactionKeywordToString(ip);
            let option = new Option(factionName);
            selector.add(option);
        }
        selector.selectedIndex = 0;

        if (unitSelector) {
            const factionName = selector.selectedOptions[selector.selectedIndex].text;
            populateUnits(factionName, unitSelector);
            let unitName = unitSelector.selectedOptions[0].text;
            if (unitRoot) {
                createConfigUI(unitName, unitRoot);
                return createUnit(unitName, unitRoot);
            }
        }
        return null;
    }

    function populateUnits(faction, selector) {
        selector.options.length = 0;
        let unitNames = new Array();

        const factionId = sim.FactionStringToKeyword(faction);

        for (let idx = 0; idx < sim.GetNumberOfAvailableUnits(); idx++) {
            var unitInfo = new AosSim.JSUnitInfo();
            sim.GetUnitInfo(idx, unitInfo);
            // filter based on keyword
            for (let fidx = 0; fidx < unitInfo.numberOfFactions; fidx++) {
                if (factionId === unitInfo.getFaction(fidx)) {
                    unitNames.push(unitInfo.name);
                }
            }
        }

        if (unitNames.length != 0) {
            unitNames.sort();
            let uniqueUnits = unitNames.filter((value, index, self) => { return self.indexOf(value) === index; });
            for (let ip of uniqueUnits) {
                let option = new Option(ip);
                selector.add(option);
            }
            selector.selectedIndex = 0;
        }
    }

    function on_red_alliance_selected(event) {
        const selectElem = event.target;
        let alliance = selectElem.selectedOptions[0].text;

        const redFactionSelect = document.getElementById("red-faction-select");
        const redUnitSelect = document.getElementById("red-unit-select");
        const redUnitRoot = document.getElementById("red-unit-desc");
        if (redFactionSelect && redUnitSelect) {
            g_red = populateFactions(alliance, redFactionSelect, redUnitSelect, redUnitRoot);
            let unitName = redUnitSelect.selectedOptions[0].text;
            if (redUnitRoot) {
                createConfigUI(unitName, redUnitRoot);
                g_blue = createUnit(unitName, redUnitRoot);
            }
            refreshPoints();
        }
    }

    function on_blue_alliance_selected(event) {
        const selectElem = event.target;
        let alliance = selectElem.selectedOptions[0].text;

        //console.log("Blue selected Grand Alliance " + alliance);

        const blueFactionSelect = document.getElementById("blue-faction-select");
        const blueUnitSelect = document.getElementById("blue-unit-select");
        const blueUnitRoot = document.getElementById("blue-unit-desc");
        if (blueFactionSelect && blueUnitSelect) {
            populateFactions(alliance, blueFactionSelect, blueUnitSelect, blueUnitRoot);
            let unitName = blueUnitSelect.selectedOptions[0].text;
            if (blueUnitRoot) {
                createConfigUI(unitName, blueUnitRoot);
                g_blue = createUnit(unitName, blueUnitRoot);
            }
            refreshPoints();
        }
    }

    function on_red_faction_selected(event) {
        const selectElem = event.target;
        let faction = selectElem.selectedOptions[0].text;

        const redUnitSelect = document.getElementById("red-unit-select");
        if (redUnitSelect) {
            populateUnits(faction, redUnitSelect);
            let unitName = redUnitSelect.selectedOptions[0].text;
            const redUnitRoot = document.getElementById("red-unit-desc");
            if (redUnitRoot) {
                createConfigUI(unitName, redUnitRoot);
                g_red = createUnit(unitName, redUnitRoot);
            }
            refreshPoints();
        } else {
            console.log("Red unit selector not found.");
        }
    }

    function on_blue_faction_selected(event) {
        const selectElem = event.target;
        let faction = selectElem.selectedOptions[0].text;

        const blueUnitSelect = document.getElementById("blue-unit-select");
        if (blueUnitSelect) {
            populateUnits(faction, blueUnitSelect);
            let unitName = blueUnitSelect.selectedOptions[0].text;
            const blueUnitRoot = document.getElementById("blue-unit-desc");
            if (blueUnitRoot) {
                createConfigUI(unitName, blueUnitRoot);
                g_blue = createUnit(unitName, blueUnitRoot);
            }
            refreshPoints();
        } else {
            console.log("Blue unit selector not found.");
        }
    }

    function on_red_unit_select(event) {
        const selectElem = event.target
        let unitName = selectElem.selectedOptions[0].text;

        console.log("Constructing UI for unit " + unitName);
        let unitRoot = document.getElementById("red-unit-desc");
        if (unitRoot) {
            createConfigUI(unitName, unitRoot);
            g_red = createUnit(unitName, unitRoot);
            refreshPoints();
        }
    }

    function on_blue_unit_select(event) {
        const selectElem = event.target;
        let unitName = selectElem.selectedOptions[0].text;

        console.log("Constructing UI for unit " + unitName);
        let unitRoot = document.getElementById("blue-unit-desc");
        if (unitRoot) {
            createConfigUI(unitName, unitRoot);
            g_blue = createUnit(unitName, unitRoot);
            refreshPoints();
        }
    }

    function createConfigUI(unitName, container) {
        var factory = new AosSim.JSUnitInfo();
        sim.GetUnitInfoByName(unitName, factory);

        console.log("Found factory for unit " + unitName);

        // Remove previous contents (if any)
        while (container.firstChild) {
            container.removeChild(container.firstChild);
        }

        console.log("\tNumber of parameters: " + factory.numberOfParameters);

        var params = new AosSim.Parameter();

        for (let idx = 0; idx < factory.numberOfParameters; idx++) {

            factory.getParameter(idx, params);

            const pname = params.name;
            const ptype = params.paramType;
            const pvalue = params.intValue;
            const pmin = params.minValue;
            const pmax = params.maxValue;
            const pincr = params.increment;

            console.log("\t Parameter[" + idx + "].name = " + pname + "  .type = " + ptype);

            const controlId = pname.trim().replace(' ', '-').toLowerCase();

            if (ptype === AosSim.Integer) {

                let group = document.createElement("fieldset");
                group.classList.add("form-group");

                // Create Label and Input:number HTML elements
                let label = document.createElement("label");
                label.textContent = pname;
                label.htmlFor = controlId;
                group.appendChild(label);

                let input = document.createElement("input");
                input.type = "number";
                input.name = pname;
                input.id = controlId;
                input.value = pvalue.toString();
                input.max = pmax.toString();
                input.min = pmin.toString();
                input.step = pincr.toString();
                input.classList.add("form-control");

                //input.maxLength = 10;
                group.appendChild(input);

                container.appendChild(group);
            }
            else if (ptype === AosSim.Enum) {

                let group = document.createElement("fieldset");
                group.classList.add("form-group");

                // Create Label and Select HTML elements
                let label = document.createElement("label");
                label.textContent = pname;
                label.htmlFor = controlId;
                group.appendChild(label);

                let select = document.createElement("select");
                select.id = controlId;
                for (let i = pmin; i <= pmax; i += pincr) {
                    let option = document.createElement("option");
                    option.text = sim.UnitParameterValueToString(unitName, pname, i);
                    select.appendChild(option);
                }
                select.classList.add("form-control");
                group.appendChild(select);

                container.appendChild(group);
            }
            else if (ptype === AosSim.Boolean) {
                let group = document.createElement("fieldset");
                group.classList.add("form-check");

                let input = document.createElement("input");
                input.type = "checkbox";
                input.name = pname;
                input.id = controlId;
                input.checked = (pvalue !== 0);
                input.classList.add("form-check-input");
                group.appendChild(input);

                let label = document.createElement("label");
                label.textContent = pname;
                label.htmlFor = controlId;
                label.classList.add("form-check-label");
                group.appendChild(label);

                container.appendChild(group);
            }
        }
    }

    function createUnit(unitName, unitUI) {
        var factory = new AosSim.JSUnitInfo();
        sim.GetUnitInfoByName(unitName, factory);

        if (!unitUI) return null;

        let parameters = [];

        // extract parameters from UI
        for (let ip of unitUI.children) {
            let param = new AosSim.Parameter();
            if (ip instanceof HTMLInputElement) {
                const input = ip;
                param.name = input.name;
                if (input.type === "number") {
                    param.intValue = +input.value;
                    param.minValue = +input.min;
                    param.maxValue = +input.max;
                    param.increment = +input.step;
                } else if (input.type === "checkbox") {
                    param.intValue = input.checked ? 1 : 0;
                    param.minValue = 0;
                    param.maxValue = 1;
                    param.increment = 1;
                }
                parameters.push(param);
            }
            else if (ip instanceof HTMLSelectElement) {
                const select = ip;
                param.name = select.name;
                param.intValue = select.selectedIndex;
                param.minValue = 0;
                param.maxValue = select.children.length - 1;
                parameters.push(param);
            }
        }

        return sim.CreateUnit(unitName, parameters, parameters.length);
    }

    function refreshPoints() {

        if (g_red) {
            let redPointsDisp = document.getElementById("red-points-disp");
            if (redPointsDisp) {
                redPointsDisp.value = g_red.points().toString();
            }
        }
        if (g_blue) {
            let bluePointsDisp = document.getElementById("blue-points-disp");
            if (bluePointsDisp) {
                bluePointsDisp.value = g_blue.points().toString();
            }
        }
    }

    function clicked_on_card() {
        console.log("Clicked on a card.");
    }

    function plumbCallbacks() {

        const startButton = document.getElementById("start-button");
        if (startButton) startButton.addEventListener("click", on_start_clicked);

        const redGASelect = document.getElementById("red-ga-select");
        if (redGASelect) redGASelect.addEventListener("input", on_red_alliance_selected);
        const blueGASelect = document.getElementById("blue-ga-select");
        if (blueGASelect) blueGASelect.addEventListener("input", on_blue_alliance_selected);

        const redFactionSelect = document.getElementById("red-faction-select");
        if (redFactionSelect) redFactionSelect.addEventListener("input", on_red_faction_selected);

        const blueFactionSelect = document.getElementById("blue-faction-select");
        if (blueFactionSelect) blueFactionSelect.addEventListener("input", on_blue_faction_selected);

        const redUnitSelect = document.getElementById("red-unit-select");
        if (redUnitSelect) redUnitSelect.addEventListener("input", on_red_unit_select);

        const blueUnitSelect = document.getElementById("blue-unit-select");
        if (blueUnitSelect) blueUnitSelect.addEventListener("input", on_blue_unit_select);

        const redCard = document.getElementById("red-card");
        if (redCard) redCard.addEventListener("click", clicked_on_card);

        const blueCard = document.getElementById("blue-card");
        if (blueCard) blueCard.addEventListener("click", clicked_on_card);

        // set up initial state
        if (redFactionSelect && redUnitSelect) {
            const redGA = redGASelect.selectedOptions[redGASelect.selectedIndex].text;
            let redUnitRoot = document.getElementById("red-unit-desc");
            g_red = populateFactions(redGA, redFactionSelect, redUnitSelect, redUnitRoot);
        }
        if (blueFactionSelect && blueUnitSelect) {
            const blueGA = blueGASelect.selectedOptions[blueGASelect.selectedIndex].text;
            let blueUnitRoot = document.getElementById("blue-unit-desc");
            g_blue = populateFactions(blueGA, blueFactionSelect, blueUnitSelect, blueUnitRoot);
        }

        refreshPoints();
    }

    let g_verboseLevel = AosSim.Normal;
    let g_numRounds = 5;
    let g_saveMaps = false;
    let g_numIterations = 5;
    let g_battle = null;
    let g_red = null;
    let g_blue = null;

    function runSimulation() {

        if (!g_battle || !g_red || !g_blue) {
            return;
        }

        g_battle.combatants(g_red, g_blue);

        console.log("Red Points: " + g_red.points + "  Blue Points: " + g_blue.points);

        let redVictories = 0;
        let blueVictories = 0;
        let ties = 0;

        for (let i = 0; i < g_numIterations; i++) {
            g_red.restore();
            g_blue.restore();

            g_battle.start();

            if (g_saveMaps) {
            }

            while (!g_battle.done()) {
                g_battle.simulate();

                const round = g_battle.currentRound();

                if (g_saveMaps) {
                }

                g_battle.next();
            }

            const victor = g_battle.getVictor();

            if (victor == AosSim.Blue)
                blueVictories++;
            else if (victor == AosSim.Red)
                redVictories++;
            else
                ties++;

            if (g_saveMaps) {
            }
        }

        const redPercent = redVictories * 100.0 / g_numIterations;
        const bluePercent = blueVictories * 100.0 / g_numIterations;
        const tiesPercent = ties * 100.0 / g_numIterations;

        console.log("Victor Breakdown (%):\n");
        console.log("\tRed: " + redPercent);
        console.log("\tBlue: " + bluePercent);
        console.log("\tTies: " + tiesPercent);

        let redPercentDisp = document.getElementById("red-victory-percent");
        if (redPercentDisp) {
            redPercentDisp.innerHTML = Math.round(redPercent).toString();
        }
        let bluePercentDisp = document.getElementById("blue-victory-percent");
        if (bluePercentDisp) {
            bluePercentDisp.innerHTML = Math.round(bluePercent).toString();
        }
        let tiesPercentDisp = document.getElementById("tie-percent");
        if (tiesPercentDisp) {
            tiesPercentDisp.innerHTML = Math.round(tiesPercent).toString();
        }

        fillInStatistics();
    }

    function on_start_clicked() {
        const numRoundsInput = document.getElementById("number-of-rounds");
        g_numRounds = +numRoundsInput.value;

        const numIterationsInput = document.getElementById("number-of-iterations");
        g_numIterations = +numIterationsInput.value;

        const verboseCheckbox = document.getElementById("verbose-flag");
        if (verboseCheckbox.checked) g_verboseLevel = AosSim.Narrative;
        sim.SetVerbosity(g_verboseLevel);

        const saveCheckbox = document.getElementById("savemaps-flag");
        g_saveMaps = saveCheckbox.checked;

        refreshPoints();

        runSimulation();

        //AosSim.Board.Instance().render(document.getElementById("mapViewer") as HTMLCanvasElement);
    }

    function updateStats(stats, team) {

        var woundsTaken = new AosSim.Wounds();
        var woundsInflicted = new AosSim.Wounds();
        stats.totalWoundsInflicted(woundsInflicted);
        stats.totalWoundsTaken(woundsTaken);

        let elem = document.getElementById(`${team}-rounds-moved`);
        if (elem) elem.innerHTML = stats.numberOfRoundsMoved().toString();
        elem = document.getElementById(`${team}-rounds-ran`);
        if (elem) elem.innerHTML = stats.numberOfRoundsRan().toString();
        elem = document.getElementById(`${team}-rounds-charged`);
        if (elem) elem.innerHTML = stats.numberOfRoundsCharged().toString();
        elem = document.getElementById(`${team}-total-movement`);
        if (elem) elem.innerHTML = stats.totalMovementDistance().toFixed(1);
        elem = document.getElementById(`${team}-avg-movement`);
        if (elem) elem.innerHTML = stats.averageMovementDistance().toFixed(1);
        elem = document.getElementById(`${team}-total-run`);
        if (elem) elem.innerHTML = stats.totalRunDistance().toFixed(1);
        elem = document.getElementById(`${team}-avg-run`);
        if (elem) elem.innerHTML = stats.averageRunDistance().toFixed(1);
        elem = document.getElementById(`${team}-total-charge`);
        if (elem) elem.innerHTML = stats.totalChargeDistance().toFixed(1);
        elem = document.getElementById(`${team}-avg-charge`);
        if (elem) elem.innerHTML = stats.averageChargeDistance().toFixed(1);
        elem = document.getElementById(`${team}-saves-made`);
        if (elem) elem.innerHTML = stats.totalSavesMade().toString();
        elem = document.getElementById(`${team}-saves-failed`);
        if (elem) elem.innerHTML = stats.totalSavesFailed().toString();
        elem = document.getElementById(`${team}-enemy-models`);
        if (elem) elem.innerHTML = stats.totalEnemyModelsSlain().toString();
        elem = document.getElementById(`${team}-wounds-inflicted`);
        if (elem) elem.innerHTML = (woundsInflicted.normal + woundsInflicted.mortal).toString();
        elem = document.getElementById(`${team}-wounds-taken`);
        if (elem) elem.innerHTML = (woundsTaken.normal + woundsTaken.mortal).toString();
        elem = document.getElementById(`${team}-models-fled`);
        if (elem) elem.innerHTML = stats.totalModelsFled().toString();
        elem = document.getElementById(`${team}-casualties`);
        if (elem) elem.innerHTML = stats.totalModelsSlain().toString();
    }

    function fillInStatistics() {
        var redStats = new AosSim.UnitStatistics();
        g_battle.getStatistics(AosSim.Red, redStats);
        updateStats(redStats, "red");

        var blueStats = new AosSim.UnitStatistics();
        g_battle.getStatistics(AosSim.Blue, blueStats);
        updateStats(blueStats, "blue");
    }


    console.log("App is starting....");

    sim.Initialize(sim.Debug);

    console.log("Initializing AoS module.");

    g_battle = new AosSim.ManoAMano(5);

    plumbCallbacks();

});
