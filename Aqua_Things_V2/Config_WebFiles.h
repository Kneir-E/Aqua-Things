#ifndef AP_CONFIG_WEB_FILES_H
#define AP_CONFIG_WEB_FILES_H

const char* main_menu_html = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Setup Mode Main Menu</title>
  </head>
  <body>
      <h1>Main Menu</h1>
      <p>Choose a setting to configure this device</p>
      <a href="edit-wifi">Edit Wifi</a>
      <a href="edit-all">Edit All Configuration</a>
      <a href="paste">Paste Configuration</a>
      <a href="reset-confirm">Reset Device</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
  </body>
  </html>
)=====";

const char* edit_wifi_html = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Edit Wi-Fi</title>
  </head>
  <body>
      <h1>Edit Wi-Fi</h1>
      <p>Edit Wi-Fi Credentials and server IP Address. Fields that are empty will not be updated.</p>
      <h3 class="ssid_title">SSID Scan:</h3>
      <div id="ssid_container">
          <div class="ssid" onclick="ssid_click(1)"><span>SSID_NAME</span><span>strength</span></div>
          <div class="ssid" onclick="ssid_click(2)"><span>SSID_NAME</span><span>strength</span></div>
      </div>
      <form action="/update-wifi">
          <label for="STA_SSID">SSID:</label>
          <input type="text", name="STA_SSID"> 
          <label for="STA_PASS">Password:</label>
          <input type="text", name="STA_PASS"> 
          <label for="STA_HOST">Server IP:</label>
          <input type="text", name="STA_HOST"> 
          <label for="STA_PORT">Server Port:</label>
          <input type="number", name="STA_PORT"> 
          <input class="btn" type="submit" value="Submit">
      </form>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
      <script>
        let ssid_content = "";
        let ssids = [];
        let ssid_container = document.getElementById('ssid_container');

        async function ssid_scan(){
            ssid_content = ""
            console.log("scanning ssid");
            fetch('/ssid-scan')
                .then(response => response.text())
                .then(responsetxt => {
                  console.log(`Received: ${responsetxt}`);
                  if(responsetxt=="None Detected"){
                      ssid_container.innerHTML = "No SSID detected";
                  }else{
                      let detected_ssids = responsetxt.split(';');
                      detected_ssids.pop();
                      detected_ssids.forEach(function getSSID(value, index){
                          const [ssidname, strength] = value.split(':');
                          ssids.push(ssidname);
                          ssid_content += `<div class="ssid" onclick="ssid_click(${index})"><span>${ssidname}</span><span>${strength}</span></div>`
                      })
                      ssid_container.innerHTML=ssid_content;
                  }
                }).catch(error =>{
                    console.error('There was a problem with the fetch operation');
                    console.log(error);
                })
        }
        
        function ssid_click(index){
            document.getElementsByName('STA_SSID')[0].value = ssids[index];
        }
        
        ssid_scan();
        setInterval(ssid_scan, 10000);
      </script>
  </body>
  </html>
)=====";

const char* edit_all_html = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Edit All Configuration</title>
  </head>
  <body>
      <h1>Edit All Configuration</h1>
      <p>Edit all device configuration variables. <br>Fields that are empty will not be updated.</p>
      <h3 class="ssid_title">SSID Scan:</h3>
      <div id="ssid_container">
          <div class="ssid" onclick="ssid_click(1)"><span>SSID_NAME</span><span>strength</span></div>
          <div class="ssid" onclick="ssid_click(2)"><span>SSID_NAME</span><span>strength</span></div>
      </div>
      <form action="/update-all">
          <h2>Wi-Fi Credentials</h2>
          <p>This is the Wi-Fi that the device will connect to. Check local server in your computer for hints.</p>
          <label for="STA_SSID">SSID:</label>
          <input type="text", name="STA_SSID"> 
          <label for="STA_PASS">Password:</label>
          <input type="text", name="STA_PASS"> 
          <label for="STA_HOST">Server IP:</label>
          <input type="text", name="STA_HOST"> 
          <label for="STA_PORT">Server Port:</label>
          <input type="number", name="STA_PORT"> 

          <h2>Device Wi-Fi</h2>
          <p>These are the Wi-Fi credentials that the device will use in configuration mode. This is where you connect to when configuring the device.</p>
          <label for="AP_SSID">SSID:</label>
          <input type="text", name="AP_SSID"> 
          <label for="AP_PASS">Password:</label>
          <input type="text", name="AP_PASS"> 

          <h2>Advanced</h2>
          <p>These are advanced settings that can be set in the local server. Leave blank if functioning under default settings</p>
          <label for="PKT_Head">Packet Header:</label>
          <input type="text", name="PKT_Head">
          <label for="PKT_Trail">Packet Trailer:</label>
          <input type="text", name="PKT_Trail">
          <input class="btn" type="submit" value="Submit">
      </form>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
      <script>
        let ssid_content = "";
        let ssids = [];
        let ssid_container = document.getElementById('ssid_container');

        async function ssid_scan(){
            ssid_content = ""
            console.log("scanning ssid");
            fetch('/ssid-scan')
                .then(response => response.text())
                .then(responsetxt => {
                  console.log(`Received: ${responsetxt}`);
                  if(responsetxt=="None Detected"){
                      ssid_container.innerHTML = "No SSID detected";
                  }else{
                      let detected_ssids = responsetxt.split(';');
                      detected_ssids.pop();
                      detected_ssids.forEach(function getSSID(value, index){
                          const [ssidname, strength] = value.split(':');
                          ssids.push(ssidname);
                          ssid_content += `<div class="ssid" onclick="ssid_click(${index})"><span>${ssidname}</span><span>${strength}</span></div>`
                      })
                      ssid_container.innerHTML=ssid_content;
                  }
                }).catch(error =>{
                    console.error('There was a problem with the fetch operation');
                    console.log(error);
                })
        }
        
        function ssid_click(index){
            document.getElementsByName('STA_SSID')[0].value = ssids[index];
        }
        
        ssid_scan();
        setInterval(ssid_scan, 10000);
      </script>
  </body>
  </html>
)=====";

const char* paste_html = R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Paste Configuration</title>
  </head>
  <body>
      <h1>Paste Configurations</h1>
      <p>Paste the configuration data copied from the local server application.</p>
      <form action="/update-paste">
          <label for="All_Config">Config:</label>
          <textarea name="All_Config"> </textarea>
          <input class="btn" type="submit" value="Submit">
      </form>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
  </body>
  </html>
)=====";

const char* reset_confirm_html =  R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Reset Confirmation</title>
  </head>
  <body>
      <h1>Fully Reset Device</h1>
      <p>Are you sure you want to reset the device? This will erase all configurations and set the configuration mode wifi credentials back to default</p>
      <a href="reset-confirmed">Confirm</a>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
  </body>
  </html>
)=====";

const char* error_html =  R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Error</title>
  </head>
  <body>
      <h1>Error</h1>
      <p>There was an error in the input</p>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
  </body>
  </html>
)=====";

const char* success_html =  R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Success</title>
  </head>
  <body>
      <h1>Configuration Successful</h1>
      <p>Configuration was successful.<br>Exitting setup mode now</p>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
  </body>
  </html>
)=====";

  const char* not_found_html =  R"=====(
  <!DOCTYPE html>
  <html lang="en">
  <head>
      <meta charset="UTF-8">
      <meta name="viewport" content="width=device-width, initial-scale=1.0">
      <link rel="stylesheet" href="style">
      <title>Reset Confirmation</title>
  </head>
  <body>
      <h1>Error</h1>
      <p>The page requested can't be found.</p>
      <a href="main-menu">Main Menu</a>
      <a href="exit">Exit Setup Mode</a>
      <div class="spacer"></div>
  </body>
  </html>
)=====";

const char* style_css = R"=====(
  :root{
    --bg-main: #1f1f1f;
    --bg-panel: #282828;
    --btn-ina: #5d0085;
    --btn-act: #822ba7;
    --tx-lght: #343a40;
    --tx-dark: #ffffff;
    --shadow: #41005d;
  }
  *{
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol";
  }
  body{
      height: 100vh;
      width: 100vw;
      display: flex;
      flex-direction: column;
      justify-content: flex-start;
      margin: 0;
      font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, "Helvetica Neue", Arial, sans-serif, "Apple Color Emoji", "Segoe UI Emoji", "Segoe UI Symbol";
      font-size: 1rem;
      font-weight: 400;
      line-height: 1.5;
      text-align: left;
      background-color: var(--bg-main);
  }
  body > *{
      display: block;
      width: min(85%, 400px);
      text-align: center;
      align-self: center;
  }
  body > *:last-child{
      box-sizing: border-box;
      margin-bottom: 5em;
  }
  h1{
      font-family: inherit;
      color: var(--tx-dark);
  }
  p{
      font-family: inherit;
      font-size: inherit;
      line-height: inherit;
      overflow: visible;
      text-decoration: none;
      text-transform: none;
      text-align: justify;
      color: var(--tx-dark);
      margin: 0;
  }
  button, a, .btn{
      background-color: var(--btn-ina);
      color: var(--tx-dark);
      padding-top: .4em;
      padding-bottom: .4em;
      border-radius: .5em;
      border: none;
      font-family: inherit;
      font-size: 1em;
      font-weight: 500;
      line-height: inherit;
      overflow: visible;
      box-shadow: 0 .5em 1.5em .2em rgba(0,0,0,0.24), 0 .5em .5em 0 rgba(0,0,0,0.19);
      vertical-align: middle;
      text-decoration: none;
      text-transform: none;
      text-align: center;
      margin-top: 1.5em;
  }
  button:focus, a:focus, .btn:focus{
      outline: 1px dotted;
      outline: 5px auto -webkit-focus-ring-color;
  }
  button:hover, a:hover, .btn:hover{
      background-color: var(--btn-act);
      box-shadow: 0 .5em .5em 0 var(--shadow), 0 .2em .2em 0 var(--shadow);
  }
  button.close {
      padding: 0;
      background-color: transparent;
      border: 0;
  }
  form{
    display: flex;
    flex-direction: column;
  }
  form > * {
    font-family: inherit;
    font-size: inherit;
    line-height: inherit;
    overflow: visible;
    text-decoration: none;
    text-transform: none;
    justify-content: flex-start;
    text-align: start;
    width: 100%;
    margin: 0;
}
  form > label {
    margin-top: .4em;
    width: 100%;
    color: var(--btn-act);
  }
  form > input[type="text"], form > textarea, form > input[type="number"]{
    width: 98%;
    color: var(--tx-dark);
    background-color: var(--bg-panel);
  }
  form > textarea {
    height: 8em;
    color: var(--tx-dark);
    background-color: var(--bg-panel);
  }
  form > h2 {
    font-size: 1.5em;
    margin-top: 1.5em;
    margin-bottom: 0em;
    color: var(--btn-act);
  }
  .spacer{
    flex: 0 0 5em
  }
  #ssid_container{
    display: flex;
    flex-direction: column;;
  }
  #ssid_container > .ssid{
    width: 100%;
    text-align: start;
    display: flex;
    flex-direction: row;
    justify-content: space-between;
    cursor:pointer;
    color: var(--tx-dark);
  }
  .ssid_title{
    text-align: start;
    color: var(--btn-act);
  }
  script{
    display: none;
  }
)=====";

#endif