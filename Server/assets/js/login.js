// Login page
const loginBtnElem = document.querySelector('#btnLogin');
loginBtnElem.addEventListener('click', async (e) => {
  e.preventDefault();

  const usernameElem = document.querySelector('#username');
  const passwordElem = document.querySelector('#password');

  try {
    const response = await fetch('/login', { 
      method: 'POST',
      headers: {
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        username: usernameElem.value,
        password: passwordElem.value
      })
    });

    // TODO: URL of the server needed
    if (response.status === 200) {
      window.location.replace('http://10.13.170.30:8080/dashboard.html');
    } else {
      const errorMsgElem = document.querySelector('#errorMsg');
      errorMsgElem.style.opacity = 1;
      setTimeout(() => {errorMsgElem.style.opacity = 0}, 5000);
    }
  } catch (error) {
    console.error('A POST error occurred:', error);
  }
});
