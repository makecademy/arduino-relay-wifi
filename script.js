window.onload = function(){
	buttonOn = document.getElementById('submitOn');
	buttonOn.onClick = relayOn;
	
	buttonOff = document.getElementById('submitOff');
	buttonOff.onClick = relayOff;
}

function relayOn(){
	
	hidden = document.getElementById("total");
	hidden.value = "On";
	
	form = document.getElementById("relay");
	form.method = "GET";
	form.action = "relay.php";
	form.submit();
}

function relayOff(){
	
	hidden = document.getElementById("total");
	hidden.value = "Off";
	
	form = document.getElementById("relay");
	form.method = "GET";
	form.action = "relay.php";
	form.submit();
}
