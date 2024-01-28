const { ipcRenderer } = require('electron');

function fetchData() {
    ipcRenderer.send('fetch-data');
}

ipcRenderer.on('fetch-data-response', (event, data) => {
    const responseElement = document.getElementById('response');
    responseElement.innerHTML = data;
});
