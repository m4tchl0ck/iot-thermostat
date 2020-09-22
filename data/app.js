const mainView = document.getElementById('main');
mainView.hidden = true;
const settingsView = document.getElementById('settings');
settingsView.hidden = true;

const temperatureError = document.getElementById('temperature-error');
const currentTemperatureSpan = document.getElementById('current-temperature');
const requiredTemperatureInput = document.getElementById('required-temperature');
const activateButton = document.getElementById('activate');
const deactivateButton = document.getElementById('deactivate');
const onButton = document.getElementById('on');
const offButton = document.getElementById('off');

const ssidInput = document.getElementById('ssid');
const passInput = document.getElementById('pass');

getTemperature()
    .then(getState)
    .then(getHeaterState)
    .then(getRequiredTemperature)
    .then(showMain)
    .then(refreshTemperature);

function getTemperature() {
    return fetch("api/temperature")
        .then(response => response.json())
        .then(data => {
            currentTemperatureSpan.innerText = data.temperature;
            temperatureError.hidden = !data.isError;
        });
}

function getConfig() {
    return fetch("api/config")
        .then(response => response.json())
        .then(data => {
            ssidInput.value = data.ssid;
            passInput.value = data.pass;
        });
}

function getRequiredTemperature() {
    return fetch("/api/requiredTemperature")
        .then(response => response.json())
        .then(data => requiredTemperatureInput.value = data.requiredTemperature);
}

function setRequiredTemperature() {
    return fetch("/api/requiredTemperature",
    {
        method: "PUT",
        headers: {
            'Content-Type': 'text/plain'
        },
        body: requiredTemperatureInput.value
    })
    .then(getRequiredTemperature);
}

function activate() {
    return setState(true);
}

function deactivate() {
    return setState(false);
}

function on() {
    return setHeaterState(true);
}

function off() {
    return setHeaterState(false);
}

function setConfig(config) {
    return fetch("api/config",
        {
            method: "PATCH",
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify(config)
        })
        .then(getConfig);
}

function setHeaterState(state) {
    return fetch("api/heater",
        {
            method: "PUT",
            headers: {
                'Content-Type': 'text/plain'
            },
            body: state
        })
        .then(getHeaterState)
        .then(getState);
}

function getHeaterState() {
    return fetch("api/heater")
        .then(response => response.text())
        .then(data => {
            onButton.hidden = data != '0';
            offButton.hidden = data != '1';
        });
}

function setState(state) {
    return fetch("api/state",
        {
            method: "PUT",
            headers: {
                'Content-Type': 'text/plain'
            },
            body: state
        })
        .then(getState)
        .then(getHeaterState);
}

function getState() {
    return fetch("api/state")
        .then(response => response.text())
        .then(data => {
            activateButton.hidden = data != '0';
            deactivateButton.hidden = data != '1';
        });
}

function showSettings() {
    getConfig()
        .then( _ => {
            settingsView.hidden = false;
            mainView.hidden = true;
        });
}

function showMain() {
    settingsView.hidden = true;
    mainView.hidden = false;
}

function saveSettings() {
    return setConfig({
        ssid: ssidInput.value,
        pass: passInput.value
    })
    .then(_ => showMain());
}

function refreshTemperature() {
    setTimeout(_ => getTemperature().then(refreshTemperature, refreshTemperature), 5000);
}
