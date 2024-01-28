// Select all checkboxes
const selectAllCheckboxElem = document.querySelector('#selectAllCheckbox');
let checkboxesElems = []; // Initialize as empty array

// Get data about selected checkboxes
let selectedCheckboxesJSON = [];

const getCheckboxData = () => {
  const jsonData = [];
  checkboxesElems.forEach((checkbox) => {
    if (selectAllCheckboxElem.checked) {
      checkbox.checked = selectAllCheckboxElem.checked;
    }
    if (checkbox.checked) {
      jsonData.push({ _id: checkbox.value });
    }
  });
  selectedCheckboxesJSON = jsonData;
  console.log(JSON.stringify(selectedCheckboxesJSON));
};

const displayJSON = (data) => {
  const deviceTableElem = document.querySelector("#deviceTable");

  data.forEach((record) => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td><input class="select-one-checkbox" type="checkbox" name="selectOne" value="${record._id}"></td>
      <td>${record["Device name"]}</td>
      <td>${record["IP Address"]}</td>
      <td class="${record.Status === "Online" ? "status-online" : "status-offline"}">${record.Status}</td>
      <td>${record.OS}</td>
      <td>${record.Version}</td>
    `;
    deviceTableElem.appendChild(row);
  });

  // Update checkboxesElems after checkboxes are added to the DOM
  checkboxesElems = document.querySelectorAll('.select-one-checkbox');

  selectAllCheckboxElem.addEventListener('click', (e) => {
    checkboxesElems.forEach((checkbox) => {
      checkbox.checked = e.target.checked;
    });
  });

  selectAllCheckboxElem.addEventListener('change', getCheckboxData);
  checkboxesElems.forEach((checkbox) => {
    checkbox.addEventListener('change', getCheckboxData);
  });
};

// Fetch data
fetch('/devices')
  .then(response => response.json())
  .then(data => {
    // Handle the JSON data
    displayJSON(data);
  })
  .catch(error => {
    console.error('Error:', error);
  });

export { selectedCheckboxesJSON };
