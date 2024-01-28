// Dashboard page
const updateBtnElem = document.querySelector('#btnUpdate');
const restartBtnElem = document.querySelector('#btnRestart');
const shutdownBtnElem = document.querySelector('#btnShutdown');

let isDeviceSelected = false;
const statusHandling = () => {
  const checkboxes = document.querySelectorAll('.select-one-checkbox');
  let isSelected = false;
  checkboxes.forEach((checkbox) => {
    if(checkbox.checked) {
      isSelected = true;
    } 
    isDeviceSelected = isSelected;
  });

  // Display status message
  if(!isSelected) {
    displayStatusMsg("error", "No device selected");
  } else {
    displayStatusMsg("success", "Successful operation");
  }
};

const displayStatusMsg = (status, msg) => {
  const statusMsgElem = document.querySelector("#statusMsg");
  statusMsgElem.classList = "status";
  switch (status) {
    case "error":
      statusMsgElem.classList.add("error");
      statusMsgElem.innerHTML = `
        <img class="icon error-icon" src="./img/status/error.png" alt="error">
        <h3 class="error-msg">${msg}</h3>
      `;
      break;
      
    case "success":
      statusMsgElem.classList.add("success");
      statusMsgElem.innerHTML = `
        <img class="icon success-icon" src="./img/status/success.png" alt="success">
        <h3 class="success-msg">${msg}</h3>
      `;
      break;
  
    default:
      console.error("Undefined status")
      break;
  }
  statusMsgElem.style.opacity = 1;
  setTimeout(() => {
    statusMsgElem.style.opacity = 0;
    statusMsgElem.innerHTML = '';
    statusMsgElem.classList = '';
  }, 5000);
};

updateBtnElem.addEventListener('click', statusHandling);
restartBtnElem.addEventListener('click', statusHandling);
shutdownBtnElem.addEventListener('click', statusHandling);

export { isDeviceSelected };






