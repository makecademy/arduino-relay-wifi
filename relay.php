<!DOCTYPE html>
<html>
<head>
	<script src="jquery-2.0.3.min.js"></script>
	<script src="script.js"></script>
	<link rel="stylesheet" type="text/css" href="style.css">
	<title>Relay Control</title>
</head>

<body>
	<form id="relay" action="remote_relay.php" method="GET">
    	<input type="button" id="submitOn" class="relayButton" value="On" onClick="relayOn()"/>
    	<input type="button" id="submitOff" class="relayButton" value="Off" onClick="relayOff()"/>
		<input type="hidden" name="total" id="total" value="">
	</form>

<?php

    $state = $_GET["total"];
    $myFile = "relay_state.txt";

    $fh = fopen($myFile, 'w');
    fwrite($fh, $state);
    fclose($fh);    

?>

</body>

</html>

