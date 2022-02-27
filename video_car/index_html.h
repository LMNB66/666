const char INDEX_HTML[]=R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta charset="UTF-8">
    <title>遥控小车Pro</title>
    <style>
        .face {
            overflow: hidden;
        }
        #face_set {
            width: 250px;
            height: 500px;
            border: 1px solid #000;
            float: left;
            background-color: #ffccff;
            transition: height 0.3s;
        }
        #face_help {
            width: 250px;
            height: 500px;
            line-height: 50px;
            border: 1px solid #000;
            float: right;
            background-color: #ffccff;
            transition: height 0.3s;
        }       
        #face_stream {
            width: auto;
            height: 500px;
            border: 1px solid #000;
            margin: 0 auto;
            display: flex;
          align-items: center;
          justify-content: center; 
          background-color: #ccffff;
        } 
        .sf{
      width: 120px;
      height: 30px;
      float: right;
      margin-right: 10px;
      margin-top: 26.666px;
    }
    .sc{
      height: 83.333px;
      line-height: 83.333px;
      border-bottom: 1px solid #000;
    }  
    .sp{
      
      margin-left: 10px;
    } 
    .wp{
      font-size: 18px;
      font-family: 宋体;
    }
    .oc{
      width: 80px;
      height: 80px;
      color:#666666;
      font-size: 50px;
      font-family: 宋体;
      background-color: #cccc99;
    }
    .dc{
      text-align: center;
    }
    #b1{
      margin-right: 25px;
    }
    #b2{
      margin-right: 300px;
    }
    #b4{
      margin-left: 300px;
    }
    #b5{
      margin-left: 25px;
    }
    .fc{
      background-color: #ffcc99;
      height: 50px;
      line-height: 50px;
      overflow: hidden;
    }
    .doc{
      background-color: #ffcc99;
      font-size: 15px;
      border: 0px;
      width: 100px;
      height: 50px;
      border-left: 0px;
      border-right: 1px solid #000;
      border-bottom: 0px;
      border-top: 0px;
    }
    </style>
</head>
<body  style="background-color: #ff9999">
  <div class="fc">
    <button id="b6" class="doc" onclick="Set();">设置</button>
    <button id="b7" class="doc" onclick="Help();">帮助</button>
  </div>
  
  
    <div class="face">
    
        <div id="face_set">
        
          <div class="sc">
        <span class="sp">分辨率</span>
        <select id="s1" class="sf">
            <option value="8" selected="selected">XGA(1024x768)</option>
            <option value="7">SVGA(800x600)</option>
          </select>
      </div>  
      
      <div class="sc">  
        <span class="sp">质量</span>
        <input id="s2" class="sf" type="range" min="10" max="63" value="10">
      </div>  
      
      <div class="sc">  
        <span class="sp">亮度</span>
        <input id="s3" class="sf" type="range" min="-2" max="2" value="-2">
      </div>
        
      <div class="sc">  
        <span class="sp">反差</span>
        <input id="s4" class="sf" type="range" min="-2" max="2" value="0">
      </div>
        
      <div class="sc">  
        <span class="sp">饱和</span>
        <input id="s5" class="sf" type="range" min="-2" max="2" value="0">
      </div>
        
      <div class="sc">  
        <span class="sp">特殊效果</span>
        <select id="s6" class="sf">
            <option value="0" selected="selected">无效果</option>
              <option value="1">阴性</option>
              <option value="2">灰度</option>
              <option value="3">红色色调</option>
              <option value="4">绿色色调</option>
              <option value="5">蓝色色调</option>
            </select>
      </div>
      
        </div>
        
        <div id="face_help">
          <p class="wp">前进:按住'W'或'⇑'或'Z'键</p>
          <p class="wp">后退:按住'S'或'⇓'或'C'键</p>
          <p class="wp">左转:按住'A'或'⇐'键</p>
          <p class="wp">右转:按住'D'或'⇒'键</p>
          <p class="wp">闪关灯:点击'F'或'X'键</p>
          <p class="wp">设置:点击'G'键</p>
          <p class="wp">帮助:点击'H'键</p>
        </div>
        
        <div id="face_stream">
          <img id="img" src=""/>
        </div>
        
    </div>
    
    <br/>
    
    <div class="dc">
    
      <button id="b1" class="oc" onclick="up();">⇑</button>
    <button id="b2" class="oc" onclick="down();">⇓</button>
    <button id="b3" class="oc" onclick="">💡</button>
    <button id="b4" class="oc" onclick="left();">⇐</button>
    <button id="b5" class="oc" onclick="right();">⇒</button>
    
    </div>
    
<script type="text/javascript">
var set=document.getElementById("face_set");
var help=document.getElementById("face_help");
var s1=document.getElementById("s1");
var s2=document.getElementById("s2");
var s3=document.getElementById("s3");
var s4=document.getElementById("s4");
var s5=document.getElementById("s5");
var s6=document.getElementById("s6");
var b1=document.getElementById("b1");
var b2=document.getElementById("b2");
var b3=document.getElementById("b3");
var b4=document.getElementById("b4");
var b5=document.getElementById("b5");
var b6=document.getElementById("b6");
var b7=document.getElementById("b7");
var a=1;
var b=1;
function Set() { 
  set.style.overflow="hidden";
  if(a==1){
    set.style.height = "0px" ;
    a=0;
  }
  else{
    set.style.height = "500px" ;
    a=1;
  }
  
}
function Help() { 
  help.style.overflow="hidden";
  if(b==1){
    help.style.height = "0px" ;
    b=0;
  }
  else{
    help.style.height = "500px" ;
    b=1;
  }
}   
function send_data(key,value){
  var xhttp = new XMLHttpRequest();
  console.log(`control?var=${key}&val=${value}`);
  xhttp.open("GET", `${window.location.href.slice(0, -1)}/control?var=${key}&val=${value}`, true);
  xhttp.send();
}
s1.onchange=function(){
  send_data(s1.id,s1.value);
}
s2.onchange=function(){
  send_data(s2.id,s2.value);
}
s3.onchange=function(){
  send_data(s3.id,s3.value);
}
s4.onchange=function(){
  send_data(s4.id,s4.value);
}
s5.onchange=function(){
  send_data(s5.id,s5.value);
}
s6.onchange=function(){
  send_data(s6.id,s6.value);
}

function key_down(id){
  var a=document.getElementById(id);
  a.style.backgroundColor="#9966ff";
}
function key_up(id){
  var a=document.getElementById(id);
  a.style.backgroundColor="#cccc99";
}

function up(){
  send_data(b1.id,1);
}
function down(){
  send_data(b2.id,1);
}
function left(){
  send_data(b4.id,1);
}
function right(){
  send_data(b5.id,1);
}
var sta=[1,1,1,1,1];
document.onkeydown = function (e) {
  var e = e||window.event;
  if ((e && e.keyCode == 38||e && e.keyCode ==87||e && e.keyCode ==90) && sta[0]==1) { 
      up();
      key_down("b1");
      sta[0]=0;
  }
  if ((e && e.keyCode == 40||e && e.keyCode ==83||e && e.keyCode ==67) && sta[1]==1) { 
      down();
      key_down("b2");
      sta[1]=0;
  }
  if ((e && e.keyCode == 37||e && e.keyCode ==65) && sta[2]==1) { 
      left();
      key_down("b4");
      sta[2]=0;
  }
  if ((e && e.keyCode == 39||e && e.keyCode ==68) && sta[3]==1) { 
      right();
      key_down("b5");
      sta[3]=0;
  }
  if (e && e.keyCode == 70||e && e.keyCode == 88) { 
    if(sta[4]==1){
      send_data(b3.id,1);
      sta[4]=0;
      return;
    }
    else if(sta[4]==0){
      send_data(b3.id,0);
      sta[4]=1;
    }
  }
  if (e && e.keyCode == 71) { 
      Set();
  }
  if (e && e.keyCode == 72) { 
      Help();
  }  
}
document.onkeyup = function (e) {
    var e = e||window.event;
    if (e && e.keyCode == 38||e && e.keyCode ==87||e && e.keyCode ==90) { 
      send_data(b1.id,0);
      key_up("b1");
      sta[0]=1;
    }
    if (e && e.keyCode == 40||e && e.keyCode ==83||e && e.keyCode ==67) { 
      send_data(b2.id,0);
      key_up("b2");
      sta[1]=1;
    }
    if (e && e.keyCode == 37||e && e.keyCode ==65) { 
      send_data(b4.id,0);
      key_up("b4");
      sta[2]=1;
    }
    if (e && e.keyCode == 39||e && e.keyCode ==68) { 
      send_data(b5.id,0);
      key_up("b5");
      sta[3]=1;
    }
}
window.onload = document.getElementById("img").src = window.location.href.slice(0, -1) + ":81/stream";
</script>
    
</body>

</html>

)rawliteral";