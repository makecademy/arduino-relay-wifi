<?php
 
    // Answer
    $myFile = "relay_state.txt";
    $fh = fopen($myFile, 'r');
    $answer = fgets($fh);
    fclose($fh);

    echo $answer;
?>