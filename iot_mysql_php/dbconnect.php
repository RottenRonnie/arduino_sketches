<?php
/*
 * dbconnect.php
 * 
 * Copyright 2017 Ron Kierstead <ron@thinkpad>
 * 
 * 
 * 
 * 
 */
$MyUsername = "USERNAME";  // enter your username for mysql

$MyPassword = "PASSWORD";  // enter your password for mysql

$MyHostname = "localhost";      // this is usually "localhost" unless your database resides on a different server



$dbh = mysql_pconnect($MyHostname , $MyUsername, $MyPassword);

$selected = mysql_select_db("iot",$dbh);



?>
