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

    let g_verboseLevel = AosSim.Normal;
    let g_numRounds = 5;
    let g_saveMaps = false;
    let g_numIterations = 5;
    let g_battle = null;
    let g_red = null;
    let g_blue = null;

    function populateFactions(alliance, selector, unitSelector, unitRoot, team) {
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
                createConfigUI(unitName, unitRoot, team);
            }
        }
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
            populateFactions(alliance, redFactionSelect, redUnitSelect, redUnitRoot, "red");
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
            populateFactions(alliance, blueFactionSelect, blueUnitSelect, blueUnitRoot, "blue");
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
                createConfigUI(unitName, redUnitRoot, "red");
            }
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
                createConfigUI(unitName, blueUnitRoot, "blue");
            }
        } else {
            console.log("Blue unit selector not found.");
        }
    }

    function on_model_count_changed(event) {
        console.log("Got model count changed event. " + event.target.id + "  Value: " + event.target.value);

        if (event.target.id.startsWith('red-')) {
            refreshPoints('red', event.target.value);
        }
        else if (event.target.id.startsWith('blue-')) {
            refreshPoints('blue', event.target.value);
        }
    }

    function on_red_unit_select(event) {
        const selectElem = event.target
        let unitName = selectElem.selectedOptions[0].text;

        console.log("Constructing UI for unit " + unitName);
        let unitRoot = document.getElementById("red-unit-desc");
        if (unitRoot) {
            createConfigUI(unitName, unitRoot, "red");
        }
    }

    function on_blue_unit_select(event) {
        const selectElem = event.target;
        let unitName = selectElem.selectedOptions[0].text;

        console.log("Constructing UI for unit " + unitName);
        let unitRoot = document.getElementById("blue-unit-desc");
        if (unitRoot) {
            createConfigUI(unitName, unitRoot, "blue");
        }
    }

    function createConfigUI(unitName, container, team) {
        var factory = new AosSim.JSUnitInfo();
        sim.GetUnitInfoByName(unitName, factory);

        console.log("Found factory for unit " + unitName + " on team " + team);

        // Remove previous contents (if any)
        while (container.firstChild) {
            container.removeChild(container.firstChild);
        }

        console.log("\tNumber of parameters: " + factory.numberOfParameters);

        var params = new AosSim.Parameter();

        var defaultModelCount = 0;

        for (let idx = 0; idx < factory.numberOfParameters; idx++) {

            factory.getParameter(idx, params);

            const pname = params.name;
            const ptype = params.paramType;
            const pvalue = params.intValue;
            const pmin = params.minValue;
            const pmax = params.maxValue;
            const pincr = params.increment;

            console.log("\t Parameter[" + idx + "].name = " + pname + "  .type = " + ptype);

            const controlId = (team + "-" + pname).trim().replace(' ', '-').toLowerCase();

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

                if (pname.includes('Model')) {
                    input.addEventListener("change", on_model_count_changed);
                    defaultModelCount = pvalue;
                }

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
                select.name = pname;
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

        refreshPoints(team, defaultModelCount);
    }

    function createUnit(unitName, unitUI) {
        var factory = new AosSim.JSUnitInfo();
        sim.GetUnitInfoByName(unitName, factory);

        if (!unitUI) return null;

        sim.ClearUnitParameters();

        // extract parameters from UI
        for (let ip of unitUI.childNodes) {
            if (ip instanceof HTMLFieldSetElement) {
                for (let iip of ip.childNodes) {
                    if (iip instanceof HTMLInputElement) {
                        const input = iip;
                        if (input.type === "number") {
                            let param = new AosSim.Parameter();
                            param.paramType = AosSim.Integer;
                            param.name = input.name;
                            param.intValue = +input.value;
                            param.minValue = +input.min;
                            param.maxValue = +input.max;
                            param.increment = +input.step;
                            sim.AddUnitParameter(param);
                        } else if (input.type === "checkbox") {
                            let param = new AosSim.Parameter();
                            param.paramType = AosSim.Boolean;
                            param.name = input.name;
                            param.intValue = input.checked ? 1 : 0;
                            param.minValue = 0;
                            param.maxValue = 1;
                            param.increment = 1;
                            sim.AddUnitParameter(param);
                        }
                    }
                    else if (iip instanceof HTMLSelectElement) {
                        const select = iip;
                        let param = new AosSim.Parameter();
                        param.paramType = AosSim.Enum;
                        param.name = select.name;
                        param.intValue = select.selectedIndex;
                        param.minValue = 0;
                        param.maxValue = select.children.length - 1;
                        sim.AddUnitParameter(param);
                    }
                }
            }
        }

        var unit = sim.CreateUnit(unitName);

        console.log("Unit points: " + unit.points() +  "  Models: " + unit.remainingModels());

        return unit;
    }

    function refreshPoints(team, numModels) {

        const unitSelect = document.getElementById(team+'-unit-select');
        if (unitSelect) {
            const unitName = unitSelect.selectedOptions[0].text;
            let pointsDisp = document.getElementById(team+'-points-disp');
            if (pointsDisp) {
                pointsDisp.value = sim.GetUnitPoints(unitName, numModels);
            }
        }
    }

    function plumbCallbacks() {

        $('#start-button').click(on_start_clicked);

        const redGASelect = document.getElementById("red-ga-select");
        if (redGASelect) redGASelect.addEventListener("change", on_red_alliance_selected);
        const blueGASelect = document.getElementById("blue-ga-select");
        if (blueGASelect) blueGASelect.addEventListener("change", on_blue_alliance_selected);

        const redFactionSelect = document.getElementById("red-faction-select");
        if (redFactionSelect) redFactionSelect.addEventListener("change", on_red_faction_selected);

        const blueFactionSelect = document.getElementById("blue-faction-select");
        if (blueFactionSelect) blueFactionSelect.addEventListener("change", on_blue_faction_selected);

        const redUnitSelect = document.getElementById("red-unit-select");
        if (redUnitSelect) redUnitSelect.addEventListener("change", on_red_unit_select);

        const blueUnitSelect = document.getElementById("blue-unit-select");
        if (blueUnitSelect) blueUnitSelect.addEventListener("change", on_blue_unit_select);

        // set up initial state
        if (redFactionSelect && redUnitSelect) {
            const redGA = redGASelect.selectedOptions[redGASelect.selectedIndex].text;
            let redUnitRoot = document.getElementById("red-unit-desc");
            populateFactions(redGA, redFactionSelect, redUnitSelect, redUnitRoot, "red");
        }
        if (blueFactionSelect && blueUnitSelect) {
            const blueGA = blueGASelect.selectedOptions[blueGASelect.selectedIndex].text;
            let blueUnitRoot = document.getElementById("blue-unit-desc");
            populateFactions(blueGA, blueFactionSelect, blueUnitSelect, blueUnitRoot, "blue");
        }

        $('#view-unit').on('show.bs.modal', function (event) {
            var button = $(event.relatedTarget) // Button that triggered the modal
            var team = button.data('which') // Extract info from data-* attributes
            // If necessary, you could initiate an AJAX request here (and then do the updating in a callback).
            // Update the modal's content. We'll use jQuery here, but you could use a data binding library or other methods instead.
            var modal = $(this)
            modal.find('.modal-title').text('New message to ' + team)
            modal.find('.modal-body input').val(team)

            const unitSelect = document.getElementById(team + "-unit-select");
            const unitRoot = document.getElementById(team + "-unit-desc");
            if (unitSelect && unitRoot) {
                const unitName = unitSelect.selectedOptions[0].text;
                if (team === "red") {
                    g_red = createUnit(unitName, unitRoot);
                }
                else if (team === "blue") {
                    g_blue = createUnit(unitName, unitRoot);
                }
            }

            // TODO: Create unit view forms outputs.
            let unit = g_blue;
            if (team === "red") {
                unit = g_red;
            }

            if (unit) {
                console.log("Red Wounds: " + unit.wounds() + "  Move: " + unit.move() + "  Weapons: " + unit.getNumWeapons());
                for (var i = 0; i < unit.getNumWeapons(); i++) {
                    const weapon = unit.getWeapon(i);
                    console.log("\t" + weapon.name_c() + "  Range: " + weapon.attacks() + "  Hit: " + weapon.toHit() + 
                        " toWound: " + weapon.toWound() + "  Strength: " + weapon.strength().toFixed(2));
                }
            }
          })
    }

    function createChart(redVictories, blueVictories, ties) {
        var ctx = document.getElementById("results-chart").getContext('2d');
        if (ctx) {

            var data = {
              datasets: [{
                  data: [redVictories, blueVictories, ties],
                  backgroundColor: [
                      '#dc3545', '#007bff', '#ffc107'
                  ]
              }],
              labels: [
                  'Red', 'Blue', 'Ties'
              ],
             };

            var options = {
                title: {
                    display: true,
                    position: 'bottom',
                    text: 'Victories',
                    fontSize: 18,
                    fontColor: '#000'
                }
            };

            var chart = new Chart(ctx, {
                type: 'doughnut',
                data: data,
                options: options
            });
        }
    }

    function runSimulation() {

        if (!g_battle || !g_red || !g_blue) {
            return;
        }

        g_battle.combatants(g_red, g_blue);

        console.log("Red Points: " + g_red.points() + "  Blue Points: " + g_blue.points());

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

        createChart(redVictories, blueVictories, ties);

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

        const redUnitSelect = document.getElementById("red-unit-select");
        const redUnitRoot = document.getElementById("red-unit-desc");
        if (redUnitSelect && redUnitRoot) {
            const unitName = redUnitSelect.selectedOptions[0].text;
            g_red = createUnit(unitName, redUnitRoot);
        }

        const blueUnitSelect = document.getElementById("blue-unit-select");
        const blueUnitRoot = document.getElementById("blue-unit-desc");
        if (blueUnitSelect && blueUnitRoot) {
            const unitName = blueUnitSelect.selectedOptions[0].text;
            g_blue = createUnit(unitName, blueUnitRoot);
        }

        refreshPoints();

        runSimulation();

        render(document.getElementById("mapViewer"));
    }

    function updateStats(stats, team) {

        var woundsTaken = new AosSim.Wounds();
        var woundsInflicted = new AosSim.Wounds();
        stats.totalWoundsInflicted(woundsInflicted);
        stats.totalWoundsTaken(woundsTaken);
        var unit = g_red;
        if (team === "blue") {
            unit = g_blue;
        }

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
        elem = document.getElementById(`${team}-points-remaining`);
        if (elem) elem.innerHTML = unit.remainingPoints().toString();
    }

    function fillInStatistics() {
        var redStats = new AosSim.UnitStatistics();
        g_battle.getStatistics(AosSim.Red, redStats);
        updateStats(redStats, "red");

        var blueStats = new AosSim.UnitStatistics();
        g_battle.getStatistics(AosSim.Blue, blueStats);
        updateStats(blueStats, "blue");
    }

    function renderUnit(ctx, unit, modelColor, unitColor, initX, initY) {
        if (!ctx || !unit) {
            return;
        }

        ctx.save();
        ctx.lineWidth = 1.0;

        const baseSize = unit.basesizeInches();
        const radiusInches = baseSize * 0.5;

        ctx.fillStyle = modelColor;
        ctx.strokeStyle = modelColor;
        for (var i = 0; i < unit.numModels(); i++) {
            const model = unit.getModel(i);

            ctx.beginPath();
            ctx.arc(model.x() * 10.0, model.y() * 10.0, radiusInches * 10.0, 0.0, 2.0 * Math.PI);
            if (model.slain()) {
                ctx.stroke();
            } else {
                ctx.fill();
            }
        }

        const posx = unit.x();
        const posy = unit.y();
        ctx.fillStyle = unitColor;
        //ctx.beginPath();
        //ctx.arc(posx * 10.0, posy * 10.0, radiusInches * 10.0, 0.0, 2.0 * Math.PI);
        //ctx.fill();

        ctx.beginPath();
        ctx.arc(initX * 10.0, initY * 10.0, radiusInches * 10.0, 0.0, 2.0 * Math.PI);
        ctx.fill();

        ctx.restore();
    }

    function renderUnitLabel(ctx, unit, textColor) {
        if (!ctx || !unit) {
            return;
        }

        const baseSize = unit.basesizeInches();
        const radiusInches = baseSize * 0.5;

        // label with the number of remaining models
        ctx.save();
        ctx.fillStyle = textColor;
        ctx.font = '12px sans';
        const text = unit.remainingModels().toString();
        ctx.fillText(text, (unit.x() - 2.0 * radiusInches) * 10, (unit.y() - radiusInches) * 10);
        ctx.restore();
    }

    function render(canvas) {
        if (!canvas) return;

        //console.log("Canvas Size: " + canvas.width + ", " + canvas.height +
        //    "  X: " + g_battle.getInitialRedX() +"  Y: " + g_battle.getInitialRedY());

        const ctx = canvas.getContext("2d");
        if (!ctx) return;

        ctx.fillStyle = "green";
        ctx.fillRect(0, 0, canvas.width, canvas.height);

        if (g_red) {
            renderUnit(ctx, g_red, "red", "darkred", g_battle.getInitialRedX(), g_battle.getInitialRedY());
        }

        if (g_blue) {
            renderUnit(ctx, g_blue, "blue", "darkblue", g_battle.getInitialBlueX(), g_battle.getInitialBlueY());
        }

        if (g_red) {
            renderUnitLabel(ctx, g_red, "white");
        }

        if (g_blue) {
            renderUnitLabel(ctx, g_blue, "white");
        }
    }

    console.log("App is starting....");

    sim.Initialize(sim.Debug);

    console.log("Initializing AoS module.");

    g_battle = new AosSim.ManoAMano(5);

    plumbCallbacks();

});
