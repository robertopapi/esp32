////////////////////////////////////////////////////////////////////////////
// modulo che contiene la definizione di variabili array contenenti il codice HTML
// va definito all'interno di un modulo .h e non .ino perché
// - sembra ci sia un problema con il numero dei file .ino che costituiscono lo sketch
// - il file va incluso nel modulo webserver.ino che altrimenti non riconosce le singole variabili array
////////////////////////////////////////////////////////////////////////////

const static char HTML_head[] PROGMEM = R"~~(
<!DOCTYPE html>
<html lang="en">
<head>
<title>FTP Server</title>
<meta name="format-detection" content="telephone=no">
<meta charset="UTF-8">
<meta http-equiv="content-type" content="text/html; charset=UTF-8">
<meta  name="viewport" content="width=device-width,initial-scale=1,user-scalable=no">
<link href="data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsIAAA7CARUoSoAAAAB5SURBVDhP1Y4BCoAwDAP3/0/PrilZVovCFMQDtzRp1NYfEOUmpNE4L4BZhlCWvUq/VCZwjKRJWFqGUJa9ciGuz8p7zC+X775mFDZqoC77f4QJwRGEyYFxEp6EyKc9hAFQk2CM051AA3JTNgVG6OioPpgRrj1+We79ANteyGKyAY2fAAAAAElFTkSuQmCC" rel="icon" type="image/x-icon" />
<style>
body
{
 font-family:arial;
}
button
{
 width:110px;
 background-color:silver;
 border-radius:10px;
}
.theme
{
 background-color:cadetblue;
 padding:10px;
 border-radius:10px;
 border:2px solid silver;
}
input,select,label
{
 font-family:arial;
 font-size:12px;
}
label,.group,.title
{
 color:white;
}
.group,.title
{
 font-weight:bold;
 font-size:16px;
}
.category
{
 cursor: pointer;
 -webkit-user-select: none;
 -moz-user-select: none;
 -ms-user-select: none;
 user-select: none;
 background-color:rgba(255, 255, 255, 0.0);
 padding:10px;
 border-radius:10px;
}
.category-click
{
position: absolute;
right: 30px;
}
.close::before
{
 content: "\2228";
}
.open::before
{
 content: "\2227";
}
.category:hover
{
 background-color:rgba(255, 255, 255, 0.2);
}
.nested
{
  display: none;
}
.active
{
  display: block;
}
.c
{
 margin-left: auto;
 margin-right: auto;
 text-align: center;
}
.vh
{
 visibility: hidden;
}
</style>
</head>
<body onload="onLoad()">
)~~";

const static char HTML_start_script[] PROGMEM = R"~~(
<script type="text/javascript">
<!--
var preloadedJSON="";
var urldatasave = "";
var urlreset = "";
)~~";

const static char HTML_global_script[] PROGMEM = R"~~(
var outputJSON="";
function debug(txt)
{
 if (txt=="")
  document.getElementById("debug").value = "";
 else
  document.getElementById("debug").value = document.getElementById("debug").value+"\n"+txt;
}
function showtag(id)
{
 document.getElementById(id).style.visibility="visible";
}
function hiddentag(id)
{
 document.getElementById(id).style.visibility="hidden";
}
function isJSON(str)
{
 try
 {
  JSON.parse(str);
 }
 catch (e)
 {
  return false;
 }
 return true;
}
function callServer()
{
 var obj;
 var url="/";
 var param="";
 switch (arguments.length)
 {
  case 3:
   param=arguments[2];
  case 2:
   url=arguments[1];
  case 1:
   obj=arguments[0];
   break;
  default:
   return(-1);
 }
 debug("callServer");
 debug("url="+url+" param="+param);
 var xhttp = new XMLHttpRequest();
 xhttp.onreadystatechange = function()
  {
   if (this.readyState == 4 && this.status == 200)
    obj.innerHTML=this.responseText;
  };
 xhttp.open("POST", url, true);
 if (isJSON(param))
  xhttp.setRequestHeader('Content-Type', 'application/json');
 if (param!="")
  xhttp.send(param);
 else
  xhttp.send();
}
function findchild(obj)
{
 for (const child of obj.children)
 {
  if (child.tagName=="INPUT" || child.tagName=="SELECT")
   outputJSON+='{"attrname":"'+child.id+'","attrvalue":"'+child.value+'"},';
  else if (child.hasChildNodes())
   findchild(child);
 }
}
function buildresponse()
{
 outputJSON="";
 var obj=document.getElementById("confpane");
 if (!obj.hasChildNodes())
  return;
 outputJSON="{\"attr\":[";
 findchild(obj);
 if (outputJSON.length <= 7)
 {
  outputJSON="";
  return;
 }
 outputJSON = outputJSON.substring(0, outputJSON.length-1);
 outputJSON += "]}";
 debug(outputJSON);
}
function sendData()
{
 if (urldatasave=="")
 {
  debug("sendData() - no urldatasave");
  return;
 }
 debug("sendData() -> " + urldatasave);
 debug(outputJSON);
 var resp=document.getElementById("serverresp");
 resp.innerHTML="";
 showtag("serverresp");
 if (callServer(resp, urldatasave, outputJSON))
  debug("sendData() - callServer error");
}
function requestReset()
{
 if (urlreset=="")
 {
  debug("requestReset() - no urlreset");
  return;
 }
 var resp=document.getElementById("serverresp");
 resp.innerHTML="";
 showtag("serverresp");
 if (callServer(resp, urlreset))
  debug("requestReset() - callServer error");
}
function onLoad()
{
 var HTMLoutput="";
 if (preloadedJSON==="")
  return;
 if ((urldatasave+urlreset)!="")
 {
  var buttonpane=document.getElementById("buttonpane");
  HTMLoutput = "<center><table><tbody><tr>";
  HTMLoutput += '<td><button id="btn_save" type="button" onclick="javascript:buildresponse();">Save (local)</button></td>';
  if (urldatasave!="")
   HTMLoutput += '<td><button id="btn_send" type="button" style="margin-left:20px;" onclick="javascript:sendData();">Send to Host</button></td>';
  if (urlreset!="")
   HTMLoutput += '<td><button id="btn_reset" type="button" style="margin-left:20px;" onclick="javascript:requestReset();">Host Reset</button></td>';
  HTMLoutput += "</tr></tbody></table></center>";
  buttonpane.innerHTML=HTMLoutput;
 } 
 var confpane=document.getElementById("confpane");
 if (!isJSON(preloadedJSON))
 { 
  confpane.innerHTML=preloadedJSON;
  return;
 }
 var datajson = JSON.parse(preloadedJSON);
 HTMLoutput = "<center><table><tbody>";
 for (let g = 0; g < datajson.grp.length; g++)
 {
  HTMLoutput += "<tr><td colspan=\"2\"><p class=\"group\">";
  HTMLoutput += datajson.grp[g].grpname;
  HTMLoutput += "</p></td></tr>";
  for (let i = 0; i < datajson.grp[g].attr.length; i++)
  {
   var name = datajson.grp[g].attr[i].attrname;
   HTMLoutput += "<tr><td><label for=\"";
   HTMLoutput += name.replace(" ", "_");
   HTMLoutput += "\">";
   HTMLoutput += name;
   HTMLoutput += ":</label></td><td>";
   if (datajson.grp[g].attr[i].attrtype.toLowerCase()=="select")
   {
    HTMLoutput += "<select id=\"";
    HTMLoutput += name.replace(" ", "_");
    HTMLoutput += "\">";
    HTMLoutput += "<optgroup label=\"current value\">";
    HTMLoutput += "<option value=\"";
    HTMLoutput += datajson.grp[g].attr[i].attrvalue[0];
    HTMLoutput += "\" disabled>";
    HTMLoutput += datajson.grp[g].attr[i].attrvalue[0];
    HTMLoutput += "</option></optgroup>";
    HTMLoutput += "<optgroup label=\"available values\">";
    for (let opt = 1; opt < datajson.grp[g].attr[i].attrvalue.length; opt++)
    {
     HTMLoutput += "<option value=\"";
     HTMLoutput += datajson.grp[g].attr[i].attrvalue[opt];
     HTMLoutput += "\">";
     HTMLoutput += datajson.grp[g].attr[i].attrvalue[opt];
     HTMLoutput += "</option>";
    }
    HTMLoutput += "</optgroup>";
    HTMLoutput += "</select>";
   }
   else
   {
    HTMLoutput += "<input id=\"";
    HTMLoutput += name.replace(" ", "_");
    HTMLoutput += "\" type=\"";
    HTMLoutput += datajson.grp[g].attr[i].attrtype;
    HTMLoutput += "\" value=\"";
    HTMLoutput += datajson.grp[g].attr[i].attrvalue;
    HTMLoutput += "\" size=\"";
    HTMLoutput += datajson.grp[g].attr[i].attrvalue.length;
    HTMLoutput += "\">";
   }
   HTMLoutput += "</td></tr>";
  }
 }
 HTMLoutput += "</tbody></table></center>"
 confpane.innerHTML=HTMLoutput;
 var inputs = document.querySelectorAll("input");
 inputs.forEach((input) => { input.addEventListener("change", (event) => { buildresponse(); } ); } ); 
 var selects = document.querySelectorAll("select");
 selects.forEach((select) => { select.addEventListener("change", (event) => { buildresponse(); } ); } );
}
function toggler(obj)
{
 if (obj.classList.contains("category"))
 {
  obj.parentElement.querySelector(".nested").classList.toggle("active");
  
  if (obj.querySelector(".category-click").classList.contains("close")) // close &#8744;
  {
   obj.querySelector(".category-click").classList.remove("close"); // close &#8744;
   obj.querySelector(".category-click").classList.add("open"); // open &#8743;
  }
  else
  {
   obj.querySelector(".category-click").classList.add("close"); // close &#8744;
   obj.querySelector(".category-click").classList.remove("open"); // open &#8743;
  }
 }
}
)~~";
const static char HTML_end_script[] PROGMEM = R"~~(
//-->
</script>
)~~";
const static char HTML_main_body[] PROGMEM = R"~~(
 <h1 style="text-align:center;">FTP Server</h1>
 <div class="theme">
  <div class="category title" onclick="toggler(this);">
   <label class="title">Configuration</label>
   <span class="category-click close">&nbsp;</span>
  </div>
  <div class="nested">
   <div id="confpane">
   </div>
   <br>
   <div id="buttonpane">
   </div>
  </div>
 </div>
 <div class="theme">
  <div class="category title" onclick="toggler(this);">
   <label class="title">Log</label>
   <span class="category-click close">&nbsp;</span>
  </div>
  <div class="nested">
   <div id="logpane">
    <textarea rows="10" cols="80"></textarea>
   </div>
  </div>
 </div>
 <div class="theme">
  <div class="category title" onclick="toggler(this);">
   <label class="title">Data</label>
   <span class="category-click close">&nbsp;</span>
  </div>
  <div class="nested">
   <div id="datapane">
    <textarea rows="10" cols="80"></textarea>
   </div>
  </div>
 </div>
 <div class="theme">
  <div class="category title" onclick="toggler(this);">
   <label class="title">Statistics</label>
   <span class="category-click close">&nbsp;</span>
  </div>
  <div class="nested">
   <div id="statpane">
    <textarea rows="10" cols="80"></textarea>
   </div>
  </div>
 </div>
)~~";

const static char HTML_tail[] PROGMEM = R"~~(
<center>
<div class="vh" id="serverresp">&nbsp;</div>
<textarea id="debug" name="debug" rows="10" cols="80"></textarea>
</center>
</body>
</html>
)~~";

//**********************************************************************************
const char CONFIG_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<head>
<meta http-equiv='content-type' content='text/html; charset=UTF-8'>
<meta name='viewport' content='width=320' />
<link href="data:image/x-icon;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsIAAA7CARUoSoAAAAB5SURBVDhP1Y4BCoAwDAP3/0/PrilZVovCFMQDtzRp1NYfEOUmpNE4L4BZhlCWvUq/VCZwjKRJWFqGUJa9ciGuz8p7zC+X775mFDZqoC77f4QJwRGEyYFxEp6EyKc9hAFQk2CM051AA3JTNgVG6OioPpgRrj1+We79ANteyGKyAY2fAAAAAElFTkSuQmCC" rel="icon" type="image/x-icon" />
<style>
body
{
 width:300px;
 font-family:arial;
}
button
{
 width:110px;
 background-color:silver;
 border-radius:10px;
}
.theme
{
 background-color:cadetblue;
 padding:10px;
 border-radius: 10px;
 border: 2px solid silver;
}
.confpane
{
 width:280px;
 height:75px;
}
.txtinput
{
 position:absolute;
 left:90px;
 width:200px;
}
.label
{
 color:white;
 font-family:arial;
}
</style>
</head>
<body>
 <h1 style="text-align:center;">FTP Server</h1>
 <form>
  <div class="confpane theme">
   <div>
    <span class="label">SSID:</span>
    <input name="conf_ssid" class="txtinput"/>
   </div>
   <div style="padding-top:10px;">
    <span class="label">PKEY:</span> 
    <input name="conf_pkey" class="txtinput" type="password"/>
   </div>
   <div style="padding-top:10px;text-align:center">
    <button type="submit" name="reset">OK</button>
   </div>
  </div>
 </form>
</body>
</html>
)=====";
