var d = 180;
var c = window.setInterval(function(){d--;document.getElementById("s").innerHTML = "Sekunden bis zum Kaffee: " + d +"."; }, 1000);
window.setTimeout(function(){clearInterval(c);document.getElementById("s").innerHTML = "Kaffe fertig!";}, 180000);
		