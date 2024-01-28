import { selectedCheckboxesJSON } from './devices.js';
import { isDeviceSelected } from './dashboard.js';

const restartBtnElem = document.querySelector('#btnRestart');
restartBtnElem.addEventListener('click', async (e) => {
  console.log(selectedCheckboxesJSON);
  console.log(isDeviceSelected);

  if(isDeviceSelected) {
    const response = await fetch('/restart', {
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({_id: selectedCheckboxesJSON})
    });
  }
});