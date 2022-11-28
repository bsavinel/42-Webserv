<?php
	session_start();
	echo "<br></br>";
	echo "<br></br>";
	unset($_SESSION);
	session_destroy();
?>

<!DOCTYPE html>
<html lang="fr">
<head>
	<center>
		<div class="text">
    	        <h1><a href="/session/session_start.php">
				<button class="btn btn-primary w3-black w3-padding-large w3-large w3-margin-top"> Back to Session start</button>
				</a></h1>
		<div class="text">
				>>Session has been closed<<</p>
		<title>BONUS SESSION</title>
	
	</center>
</head>
</html>