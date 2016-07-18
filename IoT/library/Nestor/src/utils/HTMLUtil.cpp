#include <utils/HTMLUtil.h>
#include <Arduino.h>

const char * HTMLUtil::_homePreHTML PROGMEM =
R"(
<html>
  <head>
		<title>Nestor</title>
  </head>
  <body style='font-family: arial; font-size: 14px;  background: #f1f1f1; '>
  <div style='width:96%; margin: auto; padding:2%'>
	<h1>Hi, I am working!</h1>
	<p style='font-size: 20px;'>	
)";

const char *HTMLUtil::_homeMidHTML PROGMEM = 
R"(
	</p></br>
	<fieldset style='margin-bottom: 20px'>
		<legend style='font-size: 16px;'><strong>Sensor Configuration</strong></legend>
		<div style='max-height:200px; overflow:auto; font-size:14px; line-height:24px;'>	
			<div>
)";

const char *HTMLUtil::_homeSensorHTML PROGMEM =
R"(
    			<div style='width:100%;float:left;word-wrap: break-word;' >%s</div>
)";

const char *HTMLUtil::_homePostHTML PROGMEM =
R"(
	    	</div>
		<div>
	</fieldset>	
	<div>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/conf'">Sensor Config Upload</button>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/update'">Firmware Upload</button>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/enable'">Enable</button>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/disable'">Disable</button>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/enabledebug'">Enable Debug</button>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/disabledebug'">Disable Debug</button>
 		<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/reboot'">Reboot</button>
	</div>

	</div>
  </body>
</html>
)";

const char* HTMLUtil::_logPreHTML PROGMEM =
R"(
<html>
  <head>
		<title>Nestor</title>
  </head>
  <body style='font-family: arial; font-size: 14px;  background: #f1f1f1; '>
  <div style='width:96%; margin: auto; padding:2%'>
	<fieldset style='margin-bottom: 20px'>
		<legend style='font-size: 16px;'><strong>Logs</strong></legend>
		<div style='max-height:250px; line-height:24px; overflow:auto'>
			<pre style='margin:5px;white-space: pre-wrap; white-space: -moz-pre-wrap;white-space: -pre-wrap; white-space: -o-pre-wrap; word-wrap: break-word;'>
)";

const char *HTMLUtil::_logPostHTML PROGMEM = 
R"(			
				</pre>	
			</div>
		</fieldset>
		<div>
 			<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/'">Home</button>
 			<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/update'">Firmware Upload</button>
 			<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/enable'">Enable</button>
 			<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/disable'">Disable</button>
 			<button style='margin:0 10px 5px 0;padding:5px;' onclick="window.location.href='/reboot'">Reboot</button>
		</div>
	</div>
  </body>
</html>
)";

const char* HTMLUtil::_confUploadHTML PROGMEM =
R"(<html><title>Nestor</title><body><form method='POST' action='/conf' enctype='multipart/form-data'>
                  <p>Choose Config File : </p> 
                  <input type='file' name='update'>
                  <input type='submit' value='Update'>
               </form>
         </body></html>
 )";